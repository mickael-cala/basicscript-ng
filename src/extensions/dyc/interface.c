/*

  AUTHOR : Peter Verhas

  DATE:    December 23, 2002

  CONTENT:
  This is the interface.c file for the ScriptBasic module dyc
  
  This module makes it possible to call arbitrary C functions implemented
  in a DLL that were not developed specifically for ScriptBasic. The module
  uses the code
  
      Dynacall.c - 32-bit Dynamic function calls. Ton Plooy 1998

  included into the interface.c file.

  This module is very much Windows specific and thus can not be compiled
  or used under UNIX/Linux or Mac.

NTLIBS:  
*/
#include <windows.h>
#include <stdlib.h>   /* malloc/free : voir la note de portage de DynaCall */


#define  DC_MICROSOFT           0x0000      // Default
#define  DC_BORLAND             0x0001      // Borland compat
#define  DC_CALL_CDECL          0x0010      // __cdecl
#define  DC_CALL_STD            0x0020      // __stdcall
#define  DC_RETVAL_MATH4        0x0100      // Return value in ST
#define  DC_RETVAL_MATH8        0x0200      // Return value in ST

#define  DC_CALL_STD_BO         (DC_CALL_STD | DC_BORLAND)
#define  DC_CALL_STD_MS         (DC_CALL_STD | DC_MICROSOFT)
#define  DC_CALL_STD_M8         (DC_CALL_STD | DC_RETVAL_MATH8)

#define  DC_FLAG_ARGPTR         0x00000002
#define  DC_FLAG_FLOATARG       0x00000004  /* x64 : argument de classe flottante (XMM) ; ignore en 32 bits */
#define  DC_FLAG_PTRVAL         0x00000008  /* l'union pArg EST l'argument a passer ('z') ;
                                                   en 32 bits ce cas passait deja par le
                                                   chevauchement dwArg/pArg, le flag rend
                                                   l'intention explicite et rend le
                                                   comportement correct en 64 bits, ou
                                                   l'union ne se chevauche plus entierement */

/* THISCALL : la convention Microsoft thiscall (methodes C++ non varargs,
   interfaces COM) adjoint le pointeur d'objet comme premier "argument
   registre", DEVANT la pile : ECX = this, et TOUS les autres arguments
   restent empiles — EDX n'y prend PAS part (definition MSDN, verifiee sur
   des objets COM reels, voir test/dyc_com.bas). Sous Windows x64, il n'y
   a RIEN a faire : le this d'une methode C++ est simplement le premier
   argument de la convention commune, que le moteur x64 place deja dans
   RCX. Le 32 bits, lui, reorganise son bloc asm : fn voyage dans EBX
   (l'appele le preserve), l'ESP d'entree dans EBP (push/movl ebp,esp
   avant la bascule, movl ebp,esp/pop apres), EDX devient libre pour lire
   les DWORD de sortie, et ECX est recharge APRES le rep movsl (qui
   l'utilise comme compteur). FINI LE SLOT DE RESERVE : une premiere
   version rangeait E dans 4 octets du bas du bloc, mais ces 4 octets
   occupaient precisement la position du PREMIER argument de pile de
   l'appele (il lit [esp+4] une fois l'adresse de retour empilee, soit
   [esp] au moment du call) — le chemin NORMAL recevait ses arguments
   decales d'un slot : lstrlenA rendait 3 au lieu de 11 (mesure). Avec E
   dans ebp, l'image de pile copiee est EXACTEMENT celle des arguments,
   pour le thiscall comme pour les conventions normales. */
#define  DC_CALL_THIS           0x0400      /* thiscall : ECX = this */

/* ALIGNEMENT DE PILE 32 BITS : ESP DOIT VALOIR 0 (mod 16) AU POINT DU CALL.
   La convention x86 de Windows (MSVC) l'exige, et tout appele moderne s'en
   sert : un simple movaps vers un local aligne(16) fait tomber le processus
   d'une violation d'acces (0xC0000005) si l'appelant a menti. Le moteur, lui,
   posait ESP = E - 4 - dwStSize, ou dwStSize n'est qu'un multiple de 4 : la
   pile livree dependait donc de la FORME de l'appel. Mesure — logs/
   sonde_esp.dll, asm de portee fichier qui lit esp & 15 a son entree, sans
   aucun prologue susceptible de la corriger : 0 argument -> 0, 1 -> 12
   (conforme par accident), 2 -> 8, 3 -> 4, 4 -> 0 ; et logs/sonde_stricte.bas,
   dont l'appele fait un VRAI movaps, mourait en 0xC0000005 des le premier
   appel — le code meme de la campagne de charge 32 bits (502/1000 puis
   violation d'acces, logs/validation-32-final4.log).
   La correction arrondit vers le bas, a 16, la BASE du bloc d'arguments dans
   EDI, puis bascule ESP : la copie part de cette base, ESP vaut donc
   0 (mod 16) au call, et l'appele retrouve ses arguments a [esp+4..]
   exactement comme avant. La marge ainsi consommee (au plus 15 octets) est
   sous ESP, donc dans la pile libre.
   Comme en x64, l'arrondi NE s'applique PAS directement a ESP — `andl $-16,
   %esp` est un encodage que TCC n'honore pas (meme lecon que `andq $-16,
   %rsp`, documentee dans PORTAGE_DYC64.md) : il se calcule dans EDI, qui est
   justement la destination du `rep movsl`.

   FIXTURE DU DEFAUT CORRIGE. Sous -DDYC_ESP_NONALIGN, l'arrondi est retire et
   le moteur recalcule EXACTEMENT l'ancien `ESP = E - 4 - dwStSize` : le defaut
   corrige reste donc reconstructible a volonte, et la phase 2ter de la campagne
   le reconstruit pour verifier que le `movaps` de test/dyc_probe_align.c le fait
   bien tomber en 0xC0000005. Le module LIVRE, lui, est toujours construit sans
   cette macro : la fixture est un instrument de mesure, jamais un etat du
   depot.

   DIRECTION FLAG (DF). `rep movsl` copie vers les adresses CROISSANTES ssi DF
   est a 0 ; avec DF a 1 il copie vers les decroissantes et ecrit SOUS la base
   du bloc, c'est-a-dire dans la pile libre — corruption silencieuse. L'ABI
   x86 exige DF = 0 a l'entree de toute fonction, et le x64 du meme fichier fait
   deja `cld` avant son `rep movsq` : le 32 bits l'avait oublie. Deux `cld`
   bordent donc le call dans les deux moteurs : celui d'avant protege la copie,
   celui d'apres (fin de bloc) rend DF = 0 a l'APPELANT de DynaCall, qui a le
   droit de s'attendre a ce que l'appele le lui rende comme il l'a trouve. */
#ifdef DYC_ESP_NONALIGN
#define THISCALL_ALIGN_EDI   /* fixture : l'arrondi a 16 est retire */
#else
#define THISCALL_ALIGN_EDI   "andl $-16, %%edi\n\t"
#endif

/* FIXTURE DU DEFAUT DF (DYC_SANS_CLD). Les deux `cld` ci-dessous sont ceux qui
   ont ete AJOUTES par l'audit d'ABI : celui d'avant protege la copie des
   arguments (32 bits seulement : le x64 l'avait deja), celui d'apres rend DF a
   l'appelant de DynaCall. Les retirer reconstruit le moteur d'avant, ce que la
   phase 2ter de la campagne fait pour verifier que les sondes dyc_probe_dfentry
   et dyc_probe_dfexit y lisent bien 0x400 — un instrument qui ne sait pas dire
   « non » ne prouve rien. Le module livre, lui, est toujours construit avec les
   deux. */
#ifdef DYC_SANS_CLD
#define DYC_CLD
#else
#define DYC_CLD  "cld\n\t"
#endif

/* FIXTURE DU DEFAUT THISCALL (DYC_THIS_EDX, 32 bits seulement). L'ancien moteur
   detournait aussi le DEUXIEME argument vers EDX — du fastcall-avec-this, pas
   du thiscall (voir la note de la section 10 du portage : mesure sur
   QueryInterface, l'appele lisait son premier argument de pile dans le vide).
   La fixture rejoue le SYMPTOME tel que l'appele le voit : l'argument 1 quitte
   la pile sans que rien d'autre ne le remplace, donc la sonde dyc_probe_this
   (qui lit la pile, elle) ne retrouve plus *this + dw1 + dw2 = 130. Le module
   LIVRE, lui, n'est jamais construit avec cette macro : c'est un instrument de
   mesure, pas un etat du depot. */
#ifdef DYC_THIS_EDX
#define THISCALL_SKIP_EDX_ARG  if (nArgs >= 2)dwStSize -= (Parm[1].nWidth + 3) / 4 * 4;
#define THISCALL_SKIP_EDX_PUSH if ((Flags & DC_CALL_THIS) && nInd == 1)continue;
#else
#define THISCALL_SKIP_EDX_ARG
#define THISCALL_SKIP_EDX_PUSH
#endif

/* FIXTURE DU PARSEUR D'ARGUMENTS (DYC_ANCIEN_PARSEUR). Le parseur d'origine
   laissait tomber les `case '1'`, `'2'` et `'4'` l'un dans l'autre jusqu'a
   `'8'` — qui exige une chaine de 8 octets, donc TOUT argument '1' sur une
   chaine courte levait l'erreur 8 — et `'c'` dans `'s'`, `'f'` puis
   `'h/p/l'`, dont le corps ecrase dwArg avec le POINTEUR de la chaine (toupper
   ne rendait donc plus 66). Sous cette macro, les quatre `break` qui ferment
   ces `case` redeviennent absents, exactement comme dans l'original : la
   campagne peut verifier que les essais mesurent encore le defaut. */
#ifdef DYC_ANCIEN_PARSEUR
#define PARSEUR_BREAK
#else
#define PARSEUR_BREAK  break;
#endif

#define THISCALL_PREP \
    "pushl %%ebp\n\t" \
    "movl %%esp, %%ebp\n\t" \
    "movl %%esp, %%edi\n\t" \
    "subl %%ecx, %%edi\n\t" \
    THISCALL_ALIGN_EDI \
    "movl %%edi, %%esp\n\t" \
    "shrl $2, %%ecx\n\t" \
    DYC_CLD \
    "rep movsl\n\t"
#define THISCALL_LOAD_REGS   "movl %%eax, %%ecx\n\t"       /* this : apres le rep */
#define THISCALL_CALL        "call *%%ebx\n\t"
#define THISCALL_RESTORE     "movl %%ebp, %%esp\n\t" "popl %%ebp\n\t"
#define THISCALL_INPUTS      , [ecxarg] "m"(dwEcxArg)

#pragma pack(push,1)            // Set struct packing to one byte

/* PIEGE DE PORTAGE (x64) : ce pragma ne doit vivre QUE le temps de definir
   T_RESULT et DYNAPARM. Sans #pragma pack(pop), tout ce que ce fichier
   inclut ensuite (basext.h, donc la table des fonctions du noyau) est
   compile avec un packing de 1 octet DANS LE DLL SEUL, alors que le noyau
   compile les memes structures avec l'alignement naturel. En 32 bits tous
   les champs concernes font 4 octets et les offsets coïncident ; en 64
   bits un champ de 4 octets suivi d'un pointeur decale la table de 4 et
   l'appel pSt->Dereference saute a une adresse fausse (SIGSEGV, mesure).
   D'ou le push/pop ci-dessous, absent du code d'origine. Sous la macro de
   fixture DYC_SANS_PACKPOP (x64 seulement), le pop disparait : le dossier
   `case` de ce defaut est le meme que celui de DYC_ANCIEN_PARSEUR et de
   DYC_ESP_NONALIGN — la campagne reconstruit l'ancien module et exige la
   signature (mort immediate, la table SLFST etant decalee de 4). */

typedef union T_RESULT {          // Various result types
    int     Int;                // Generic four-byte type
    long    Long;               // Four-byte long
    void   *Pointer;            // 32-bit pointer
    float   Float;              // Four byte real
    double  Double;             // 8-byte real
    __int64 int64;              // big int (64-bit)
} T_RESULT;

typedef struct _DYNAPARM {
    DWORD       dwFlags;        // Parameter flags
    int         nWidth;         // Byte width
    union {                     //
        DWORD   dwArg;          // 4-byte argument
        void   *pArg;           // Pointer to argument
    };
} DYNAPARM;

#ifndef DYC_SANS_PACKPOP
#pragma pack(pop)               // restaure l'alignement naturel pour tout
                                // le reste du fichier, en-tetes comprises
#endif

static
DWORD_PTR WINAPI SearchProcAddress(HMODULE hInst, LPSTR szFunction)
{
    // Add some simple searching to the GetProcAddress function.
    // Various Win32 functions have two versions, a ASCII and
    // a Unicode version.
    DWORD_PTR dwAddr;                       // x64 : une adresse tient sur 64 bits
    char  szName[128];
#pragma warning ( disable : 4311 )
    if ((dwAddr = (DWORD_PTR)GetProcAddress(hInst, szFunction)) == 0) {
        // Function name not found, try some variants
        strcpy(szName, szFunction);
        strcat(szName, "A");            // ASCII
        dwAddr = (DWORD_PTR)GetProcAddress(hInst, szName);
    }
    return dwAddr;
}

//------------------------------------------------------------------
// OPTION DE COMPILATION PROBES_LIVE
// ---------------------------------
// Les sondes assembleur (thiscall : test/dyc_probe_this.c en 32 bits,
// test/dyc_probe_this64.c en x64 ; alignement de pile : test/dyc_probe_align.c,
// 32 bits seulement — le pourquoi de cette limite est dit dans le fichier de
// sonde et dans PORTAGE_DYC64.md ; les autres contrats d'ABI :
// test/dyc_probe_abi.c et test/dyc_probe_abi64.c — direction flag, pile x87,
// MXCSR, registres non volatils) sont incluses ICI, dans le DLL lui-meme, au
// lieu d'etre liees a part : leur cible de test est donc une exportation
// du module, resolue par le noyau via la table SLFST. Sans PROBES_LIVE,
// les fichiers ne produisent AUCUN code (le bloc est entierement sous
// #ifdef) : le module livre n'embarque pas de code de test. Le build
// (build.py) ajoute -DPROBES_LIVE et le fichier .s de l'architecture
// quand la variable d'environnement PROBES_LIVE est non vide :
//
//     PROBES_LIVE=1 python build.py ext_dlls --arch 64 --link --only dyc
//
// FIXTURES DES DEFAUTS CORRIGES (la « banque de defauts », jouee par la phase
// 2ter de test/run_all.py sur le script test/dyc_fixture.bas). Chaque macro
// repose un etat ANCIEN du moteur, celui d'avant un correctif, pour que la
// campagne verifie sur ce moteur reconstruit que ses essais lisent encore la
// SIGNATURE du defaut : un instrument qui ne sait plus dire « non » ne prouve
// rien, et une panne corrigee qui ne reste pas reproductible est une panne qui
// revient. Aucune de ces macros n'est jamais posee pour construire le module
// livre ; build.py refuse celles qui n'ont pas de sens dans l'architecture
// demandee (chaque moteur les refuse aussi a la compilation, voir plus bas).
//
//   DYC_ESP_NONALIGN    (32)    l'arrondi a 16 de la base du bloc d'appel est
//                              retire : le `movaps` de dyc_probe_align_strict
//                              doit faire tomber le processus (0xC0000005) ;
//   DYC_SANS_CLD        (32)    les deux `cld` ajoutes par l'audit d'ABI sont
//                              retires : les deux sondes DF lisent 0x400 ;
//   DYC_THIS_EDX        (32)    le deuxieme argument du thiscall quitte la pile
//                              (le fastcall-avec-this de la premiere version) :
//                              dyc_probe_this ne rend plus 130 ;
//   DYC_ANCIEN_PARSEUR  (32/64) les fallthroughs du parseur d'arguments
//                              ('1'->'8' et 'c'->'s','f','h/p/l') reviennent ;
//   DYC_SANS_MEMSET     (32/64) la mise a zero du tableau d'arguments est
//                              remplacee par un motif 0x5A : les octets hauts
//                              d'un '1'/'2' partent dans la pile d'appel (le
//                              motif rend decidable ce que le hasard du tas
//                              rendait invisible, voir le code) ;
//   DYC_ANCIEN_ZEROARG  (32/64) un format a zero argument (virgule finale)
//                              ressort sur l'erreur 17, comme avant le
//                              correctif de la lacune d'origine ;
//   DYC_SANS_PACKPOP    (64)    le `#pragma pack(pop)` disparait : la table des
//                              fonctions du noyau est lue avec un decalage de 4
//                              et le module meurt des le premier appel.
//
// Le detail des deux premieres est dit sur THISCALL_ALIGN_EDI et DYC_CLD plus
// bas ; celui des autres, a l'endroit exact du code concerne. Toutes les
// signatures sont verifiees par test/dyc_fixture.bas, qui les mesure sans
// savoir laquelle est posee : la campagne compare ensuite ses lignes de
// classification (sain / FAUTIF) a la banque de test/run_all.py.
//------------------------------------------------------------------
// La fixture d'alignement vise le 32 bits, seul moteur qui construisait sa
// base de pile sans arrondi (le x64 arrondit deja : R = (E - 192) & ~15).
#if defined(DYC_ESP_NONALIGN) && defined(_WIN64)
#error "DYC_ESP_NONALIGN n'a de sens qu'en 32 bits (voir section 12 du portage)"
#endif
/* Meme regle pour les deux autres fixtures 32 bits : le moteur x64 n'a ni
   EDX de thiscall, ni `rep movsl` a arrondir. */
#if defined(DYC_THIS_EDX) && defined(_WIN64)
#error "DYC_THIS_EDX n'a de sens qu'en 32 bits (le thiscall x64 est la convention commune)"
#endif
#if defined(DYC_SANS_CLD) && defined(_WIN64)
#error "DYC_SANS_CLD n'a de sens qu'en 32 bits (le x64 faisait deja son cld d'entree)"
#endif
/* La fixture du pack(1) est x64 SEULEMENT : en 32 bits tous les champs
   concernes font 4 octets et les offsets coincident, le defaut n'y existe pas
   (voir la note du pragma). */
#if defined(DYC_SANS_PACKPOP) && !defined(_WIN64)
#error "DYC_SANS_PACKPOP n'a de sens qu'en x64 (en 32 bits les offsets coincident)"
#endif
#ifdef PROBES_LIVE
#ifndef _WIN64
#include "../../../test/dyc_probe_this.c"      /* thiscall 32 bits */
#include "../../../test/dyc_probe_align.c"     /* alignement de pile 32 bits */
#include "../../../test/dyc_probe_abi.c"       /* DF, x87, MXCSR, registres non
                                                volatils (32 bits) */
#else
#include "../../../test/dyc_probe_this64.c"    /* thiscall x64     */
#include "../../../test/dyc_probe_abi64.c"     /* DF, x87, MXCSR, registres non
                                                volatils (x64) */
#endif
#endif

//------------------------------------------------------------------
// NOTE DE PORTAGE TinyCC
// ----------------------
// Le code d'origine (Dynacall.c, Ton Plooy 1998) construisait la pile
// d'appel avec l'asm inline de Microsoft :
//
//     _asm mov pStack, esp
//     _asm sub esp, 0x100
//     ... ecriture des arguments via pStack ...
//     _asm call [lpFunction]
//
// TinyCC ne connait pas la syntaxe _asm (verifie : "'_asm' undeclared"),
// il n'accepte que l'asm inline GNU (__asm__). La traduction conserve la
// semantique exacte, en separant les deux responsabilites que l'original
// entrelacait :
//
//   1. la construction des arguments se fait en C, dans un tampon du tas
//      dont l'agencement memoire reproduit exactement celui des push
//      successifs (argument 0 a l'adresse la plus basse) ;
//   2. le basculement de ESP, l'appel et la restauration tiennent dans UN
//      SEUL bloc __asm__.
//
// Cette separation est le point important : entrelacer du C et un ESP
// deplace n'est correct que si le compilateur adresse toutes les locales
// via EBP. En isolant l'appel, le resultat ne depend plus de ce detail,
// et le tampon du tas reste adressable quel que soit l'etat de ESP.
//
// Le bloc d'asm garde ESP dans ebx : ebx est preserve par l'appele, aussi
// bien en cdecl qu'en stdcall, donc aucune variable locale n'est lue ni
// ecrite pendant que ESP est deplace.
//------------------------------------------------------------------
#ifndef _WIN64

/* L'export explicite sert a l'audit de performance (test/bench_dyc.c : le
   bench mesure EXACTEMENT le code embarque dans le DLL en le chargeant par
   LoadLibrary/GetProcAddress) ; le module lui-meme n'expose DynaCall que
   derriere sa SLFST. Le static d'origine laisse place a l'export — en lien
   statique (cible ext_native), le symbole non statique se resoluit
   exactement de la meme facon. */
__declspec(dllexport)
T_RESULT WINAPI DynaCall(int Flags, DWORD_PTR lpFunction,
                                  int nArgs, DYNAPARM Parm[],
                                  LPVOID pRet, int nRetSiz)
{
    // Call the specified function with the given parameters. Build a
    // proper stack and take care of correct return value processing.
    T_RESULT  Res = { 0 };
    int     i, nInd, nSize;
    DWORD   dwEAX = 0, dwEDX = 0, dwVal, dwStSize = 0;
    DWORD  *pStack;
    BYTE   *pArg, *pArgBlock;
    DWORD   dwEcxArg = 0;   /* this du thiscall, lu par les memes trois cas */

    // Push args onto the stack. Every argument is aligned on a
    // 4-byte boundary. We start at the rightmost argument.
    //
    // Premier passage : mesurer la place necessaire. Chaque argument est
    // aligne sur 4 octets, donc la pile totale est un multiple de 4.
    for (i = 0; i < nArgs; i++) {
        nInd  = (nArgs - 1) - i;
        nSize = (Parm[nInd].nWidth + 3) / 4 * 4;
        dwStSize += (DWORD)nSize; // Count no of bytes on stack
    }

    /* THISCALL : la convention MS adjoint le pointeur d'objet comme
       premier "argument registre", DEVANT la pile — ECX porte this, qui
       QUITTE donc la pile, et les arguments suivants y restent tous
       (definition MSDN, verifiee sur des objets COM reels) : on retire
       les 4 octets alignes de l'argument 0. EDX n'y prend PAS part — une
       premiere version detournait le deuxieme argument vers EDX : c'est
       du fastcall-avec-this, pas du thiscall (mesure sur
       QueryInterface). L'ESP d'entree du bloc voyage dans EBP : l'image
       empilee est EXACTEMENT celle des arguments restants, pour le
       thiscall comme pour les conventions normales. Le rechargement de
       ecx=this a lieu APRES le rep movsl, qui l'utilise comme compteur. */
    if (Flags & DC_CALL_THIS) {
        if (nArgs >= 1)dwStSize -= (Parm[0].nWidth + 3) / 4 * 4;
        THISCALL_SKIP_EDX_ARG   /* fixture DYC_THIS_EDX : rien hors de la macro */
        }

    if ((pRet != NULL) && ((Flags & DC_BORLAND) || (nRetSiz > 8))) {
        // Return value isn't passed through registers, memory copy
        // is performed instead. Pass the pointer as hidden arg.
        dwStSize += 4;          // Add stack size
    }

    /* Valeur du registre du thiscall : this par les MEMES trois cas que
       le marshalling de pile (PTRVAL / ARGPTR / scalaire) — une STRING
       passee comme this donne l'adresse de son tampon, exactement comme
       pour 'L'/'p'/'h'. this existe des nArgs=1 ; sans 't', dwEcxArg
       reste 0 et le movl eax->ecx du bloc est un no-op qui fait le menage
       avant l'appel (ecx n'a pas de role definie en cdecl/stdcall). */
    if (Flags & DC_CALL_THIS) {
        if (nArgs >= 1) {
            if (Parm[0].dwFlags & DC_FLAG_PTRVAL)
                dwEcxArg = (DWORD)Parm[0].pArg;
            else if (Parm[0].dwFlags & DC_FLAG_ARGPTR)
                dwEcxArg = *(DWORD *)Parm[0].pArg;
            else
                dwEcxArg = Parm[0].dwArg;
            }
        }

    pArgBlock = (BYTE *)malloc(dwStSize ? dwStSize : 4);
    if (pArgBlock == NULL) return Res;
    pStack = (DWORD *)(pArgBlock + dwStSize);   // SS:[ESP] du premier push

    // Second passage : empiler les arguments, du dernier au premier.
    for (i = 0; i < nArgs; i++) {
        nInd  = (nArgs - 1) - i;
        /* THISCALL : l'argument 0 (this) ne va PAS sur la pile — il part
           dans ECX au moment du call. C'est le DERNIER push (le plus bas
           de pile) qui saute ; les suivants gardent leur place. */
        if ((Flags & DC_CALL_THIS) && nInd == 0)continue;
        THISCALL_SKIP_EDX_PUSH  /* fixture DYC_THIS_EDX : rien hors de la macro */
        // Start at the back of the arg ptr, aligned on a DWORD
        nSize = (Parm[nInd].nWidth + 3) / 4 * 4;
        pArg  = (BYTE *)Parm[nInd].pArg + nSize - 4;
        while (nSize > 0) {
            // Copy argument to the stack
            if (Parm[nInd].dwFlags & DC_FLAG_PTRVAL) {
                // Le pointeur EST l'argument ('z'). En 32 bits, dwArg et pArg
                // se chevauchent dans l'union, donc lire dwArg donnait deja le
                // pointeur entier ; le test explicite ne change rien au
                // resultat 32 bits mais reste correct tel quel en 64 bits.
                dwVal = (DWORD)Parm[nInd].pArg;
            }
            else if (Parm[nInd].dwFlags & DC_FLAG_ARGPTR) {
                // Arg has a ptr to a variable that has the arg
                dwVal = *(DWORD *)pArg; // Get first four bytes
                pArg -= 4;              // Next part of argument
            }
            else {
                // Arg has the real arg
                dwVal = Parm[nInd].dwArg;
            }
            // Do push dwVal
            pStack--;           // ESP = ESP - 4
            *pStack = dwVal;    // SS:[ESP] = dwVal
            nSize -= 4;
        }
    }
    if ((pRet != NULL) && ((Flags & DC_BORLAND) || (nRetSiz > 8))) {
        pStack--;               // ESP = ESP - 4
        *pStack = (DWORD)pRet;  // SS:[ESP] = pMem, argument cache
    }

    // Stack is now properly built, we can call the function.
    //   - ESP est descendu de dwStSize, le bloc d'arguments y est copie ;
    //   - l'appele trouve donc sa pile sous le bloc et ne peut pas
    //     atteindre les locales de DynaCall ;
    //   - ESP est restaure inconditionnellement, ce qui couvre d'un coup
    //     le nettoyage cdecl (l'appele) et stdcall (l'appelant) ;
    //   - eax/edx sont releves apres restauration, donc la lecture des
    //     operandes de sortie ne depend pas de la position de ESP.
    __asm__ volatile(
        "movl %[size], %%ecx\n\t"
        "movl %[src], %%esi\n\t"
        "movl %[ecxarg], %%eax\n\t"     /* this -> eax, en attente du post-rep */
        "movl %[fn], %%ebx\n\t"         /* fn -> ebx (conserve par l'appele) */
        THISCALL_PREP
        THISCALL_LOAD_REGS
        THISCALL_CALL
        THISCALL_RESTORE
        "movl %%eax, %[eax]\n\t"
        "movl %%edx, %[edx]\n\t"
        DYC_CLD                       /* DF rendu a 0 a l'appelant de DynaCall */
        : [eax] "=m"(dwEAX), [edx] "=m"(dwEDX)
        : [size] "m"(dwStSize), [src] "m"(pArgBlock), [fn] "m"(lpFunction)
          THISCALL_INPUTS
        : "eax", "ebx", "ecx", "edx", "esi", "edi", "memory", "cc");
    free(pArgBlock);

    // Possibly read return values. Le resultat flottant est laisse dans ST
    // par l'appele, comme dans le code d'origine (fstp dword/qword ptr).
    if (Flags & DC_RETVAL_MATH4) {
        __asm__ volatile("fstps %0" : "=m"(Res.Float));
    }
    else if (Flags & DC_RETVAL_MATH8) {
        __asm__ volatile("fstpl %0" : "=m"(Res.Double));
    }
    else if (pRet == NULL) {
        // Prix : la bascule de ESP faisait aussi le menage, maitenant que
        // ESP est restaure d'un bloc. Placement explicite de eax/edx :
        // T_RESULT est une union, donc le premier DWORD est a l'offset 0
        // quelle que soit la ligne alignee par #pragma pack(1).
        *(DWORD *)&Res        = dwEAX;
        *((DWORD *)&Res + 1)  = dwEDX;
    }
    else if (((Flags & DC_BORLAND) == 0) && (nRetSiz <= 8)) {
        // Microsoft optimized less than 8-bytes structure passing
        ((DWORD *)pRet)[0] = dwEAX;
        ((DWORD *)pRet)[1] = dwEDX;
    }
    return Res;
}

#if defined(_DEBUG) && defined(PROBES_LIVE)
/* INSTRUMENT DE PREUVE (voir la note « INSTRUMENT MEMOIRE » plus bas) : un
   appel dyc 32 bits se fait via DynaCall, qui passe par malloc/free du CRT
   du DLL — c'est-a-dire, sous -D_DEBUG, par le distributeur de diagnostic
   testa_Alloc/testa_Free du noyau (voir rsp/scriba_dbg.rsp). L'export
   rend le solde de ce distributeur : a l'equilibre, il vaut 0, ce que le
   test test/mt_leak.bas verifie apres ses appels, PENDANT l'execution —
   la preuve ne repose pas seulement sur le bilan final du processus. */
DLL_EXPORT long dyc_alloc_balance(void)
{
  return testa_ReportLeak();
  }
#endif

#else /* _WIN64 */

//------------------------------------------------------------------
// MOTEUR X86_64 (Windows x64) -- portage TinyCC
// ---------------------------------------------
// Deux contraintes contraignent l'ecriture :
//
// 1. L'ABI Windows x64 passe les quatre premiers arguments entiers dans
//    RCX, RDX, R8, R9 (indexes par la POSITION de l'argument : un flottant
//    en position 1 part dans XMM1, pas dans RDX), les suivants sur la pile,
//    et exige 32 octets de shadow space plus un RSP aligne sur 16 au `call`.
//
// 2. TCC x64 adresse les operandes memoire d'un bloc asm RELATIVEMENT A
//    RSP (mesure : des que le bloc deplace RSP, les lectures lisent
//    ailleurs). Toute lecture d'operande se fait donc a la PREMIERE
//    instruction, et tout l'etat de l'appel vit dans une structure unique
//    que le bloc parcourt a travers un seul registre (R10), jamais par une
//    operande une fois RSP deplace.
//
// 3. Le movups de TCC x64 a operande memoire est INVERSE (le « load »
//    mem->xmm encode un store : mesure, sonde gen/tpdyc.c v13) et le
//    movq xmm->registre segfaulte (le GPR est traite comme une adresse).
//    Les flottants ne circulent donc JAMAIS directement entre la memoire
//    et xmm : chaque xmm est charge par relais GPR (movq mem->rax puis
//    movq rax->xmm), et le retour est capture DANS le bloc (movq xmm0 ->
//    184(%r11) sur la zone reservee, relu dans rbx). rbx est une sortie
//    « =b » du bloc : le compilateur sait qu'elle est detruite, rien a
//    sauvegarder.
//------------------------------------------------------------------

typedef struct _DYC64 {
    UINT_PTR pile[16];   /* +0   : arguments au-dela du quatrieme */
    UINT_PTR npile;      /* +128 : combien */
    UINT_PTR p0;         /* +136 : les quatre premiers arguments entiers */
    UINT_PTR p1;         /* +144 */
    UINT_PTR p2;         /* +152 */
    UINT_PTR p3;         /* +160 */
    struct { UINT_PTR bits; UINT_PTR pad; } xf[4]; /* +168..231 : bits bruts
       des flottants (double sur ses 64 bits ; float zero-etendu dans les
       32 bits bas, exactement comme l'ABI le fait passer) */
    UINT_PTR fn;         /* +232 : adresse de la fonction */
} DYC64;

/* Meme export explicite que le moteur 32 bits (voir plus haut) : l'audit
   de performance charge le DLL livre et mesure exactement son code. */
__declspec(dllexport)
T_RESULT WINAPI DynaCall(int Flags, DWORD_PTR lpFunction,
                                  int nArgs, DYNAPARM Parm[],
                                  LPVOID pRet, int nRetSiz)
{
    T_RESULT  Res = { 0 };
    DYC64     B;
    UINT_PTR  uInt;           // RAX au retour : resultat entier (64 bits)
    UINT_PTR  uBits;          // RBX au retour : bits du double (XMM0)
    UINT_PTR  uVal;           // valeur 64 bits de l'argument courant
    int       POS;            // position de l'argument courant (les deux files
                              // de registres sont indexes par la MEME position)
    int       POS0;           // position du PREMIER argument de l'appelant :
                              // 1 en retour par memoire (la position 0 porte
                              // alors le pointeur cache du tampon)
    int       i;

    B.npile = 0;
    B.p0 = B.p1 = B.p2 = B.p3 = 0;
    B.xf[0].bits = B.xf[1].bits = B.xf[2].bits = B.xf[3].bits = 0;
    B.fn = (UINT_PTR)lpFunction;

    /* RETOUR PAR MEMOIRE (structure de plus de 8 octets) : l'ABI x64 impose
       le pointeur du tampon comme PREMIER argument entier (RCX), et RAX ne
       porte alors que ce pointeur au retour — il n'y a plus de resultat
       registre a capturer, tout est dans le tampon. Les arguments de
       l'appelant sont decales d'une position. En 32 bits, le code d'origine
       poussait deja le pointeur en bas de pile : rien a changer la-bas. */
    POS0 = 0;
    if( (pRet != NULL) && (nRetSiz > 8) ){
      POS0 = 1;
      B.p0 = (UINT_PTR)pRet;
      }

    for( POS = POS0, i = 0 ; i < nArgs ; i++, POS++ ){
      if( Parm[i].dwFlags & DC_FLAG_FLOATARG ){
        /* Un float passe dans les 32 bits BAS de son xmm (bits bruts, sans
           conversion en double) ; un double passe sur ses 64 bits. Sur la
           pile, un float n'occupe que les 4 octets bas de son slot : la
           zero-extension des bits du float donne exactement cela. */
        UINT_PTR bits = 0;
        if( Parm[i].nWidth == 4 )
          memcpy(&bits, &Parm[i].dwArg, 4);
        else
          bits = *(UINT_PTR *)Parm[i].pArg;
        if( POS < 4 )
          B.xf[POS].bits = bits;
        else
          B.pile[B.npile++] = bits;
        continue;
        }
      /* classes entieres et pointeurs. En 64 bits l'union DYNAPARM ne se
         chevauche plus : dwArg fait 4 octets, pArg en fait 8. Lire dwArg
         pour un pointeur tronquerait l'adresse, d'ou les trois cas. */
      if( Parm[i].dwFlags & DC_FLAG_PTRVAL )       /* 'z' : pArg EST l'argument */
        uVal = (UINT_PTR)Parm[i].pArg;
      else if( Parm[i].dwFlags & DC_FLAG_ARGPTR )  /* '8' : 8 octets pointes */
        uVal = *(UINT_PTR *)Parm[i].pArg;
      else                                         /* scalaire 4 octets */
        uVal = (UINT_PTR)Parm[i].dwArg;
      if( POS < 4 ){
        switch( POS ){
          case 0: B.p0 = uVal; break;
          case 1: B.p1 = uVal; break;
          case 2: B.p2 = uVal; break;
          case 3: B.p3 = uVal; break;
          }
        }
      else
        B.pile[B.npile++] = uVal;
      }

    /* BORNE DE SECURITE : la struct reserve seize slots de pile (la sonde
       add4/sum6 et les cas du test en tiennent compte) ; au-dela, on refuse
       l'appel plutot que d'ecraser la memoire qui suit — dix-neuf arguments
       d'appelant au plus (le pointeur cache compte dans la position). */
    if( B.npile > 16 )return Res;   /* appelle un tampon non rempli : Res=0 */

    // Ordre impose : la lecture de l'unique operande d'abord (avant tout
    // deplacement de RSP), puis la reservation, la copie, et le chargement
    // des registres d'arguments EN DERNIER. Tout l'etat est lu a travers
    // R10, jamais par une operande, donc rien ne depend de RSP.
    //
    // Geometrie de la zone reservee, apres la bascule rsp = R
    // (E = rsp d'entree, R = E - 192 arrondi vers le bas a 16) :
    //   R+0   .. R+31   shadow space ;
    //   R+32  .. R+159  arguments de pile copies ici (16 slots au plus) ;
    //   R+160           E, l'adresse d'entree (slot de restauration) ;
    //   R+168, R+176    rsi, rdi sauves (non-volatils, restaures apres).
    // Au moment du call, rsp vise le bas du shadow space et est aligne
    // sur 16 : le callee voit rsp%16 == 8 apres son adresse de retour
    // (valide par la sonde pour npile pair et impair).
    //
    // Le bloc a ete valide instruction par instruction par la sonde
    // gen/tpdyc.c : registres, arguments de pile, alignement du callee,
    // cible destructive sur rsi/rdi/r10/r11, conservation de rsi/rdi.
    // Les pieges qui ont coute le plus cher :
    //   * « andq $-16, rsp » produit chez TCC un encodage bogus
    //     (reproduit hors DLL, mesure) : l'arrondi se calcule sur rax et
    //     revient par movq rax, rsp ;
    //   * la restauration de rsp passe par mem -> rax -> rsp (chargement
    //     direct mem -> rsp non valide) ;
    //   * r10 et r11 sont VOLATILS sous Windows x64 : apres le call,
    //     aucune lecture ne passe par eux (un passage par syscall, comme
    //     Sleep, les ecrase -- constate) ; E transite par r11 AVANT le
    //     call puis vit dans le slot R+160 ; apres le call, r11 est
    //     reconstruit depuis rsp (R) le temps de la capture du retour ;
    //   * rsi/rdi sont NON-VOLATILS et le rep movsq les massacre, d'ou
    //     leur sauvegarde dans la zone morte de R ;
    //   * le resultat double est capture DANS le bloc (xmm0 -> la zone,
    //     relu dans rbx) : toute relecture de xmm0 APRES le bloc est
    //     perdue, TCC reutilise les registres sans clobber declarable ;
    //     rbx est une sortie « =b » du bloc, rien a sauvegarder.
    __asm__ volatile(
        "movq %0, %%r10\n\t"             /* r10 = &B, lu AVANT de bouger rsp */
        "movq %%rsp, %%r11\n\t"          /* r11 = E, l'adresse d'entree */
        "movq %%r11, %%rax\n\t"
        "subq $192, %%rax\n\t"
        "andq $-16, %%rax\n\t"           /* rax = R : aligne SOUS E, jamais
                                            sur rsp (encodage bogus TCC) */
        "movq %%rax, %%rsp\n\t"          /* bascule : rsp = R */
        "movq %%r11, 160(%%rsp)\n\t"     /* E sauvegarde, r11 encore vivant */
        "movq %%rsi, 168(%%rsp)\n\t"
        "movq %%rdi, 176(%%rsp)\n\t"
        "leaq 32(%%rsp), %%rdi\n\t"      /* destination : au-dessus du shadow */
        "movq %%r10, %%rsi\n\t"          /* source : la struct */
        "movq 128(%%r10), %%rcx\n\t"     /* npile */
        "cld\n\t"
        "rep movsq\n\t"
        "movq 136(%%r10), %%rcx\n\t"     /* les quatre arguments entiers */
        "movq 144(%%r10), %%rdx\n\t"
        "movq 152(%%r10), %%r8\n\t"
        "movq 160(%%r10), %%r9\n\t"
        "movq 168(%%r10), %%rax\n\t"     /* les quatre flottants par relais
                                            GPR : movups a operande memoire
                                            est inverse chez TCC (mesure) */
        "movq %%rax, %%xmm0\n\t"
        "movq 184(%%r10), %%rax\n\t"
        "movq %%rax, %%xmm1\n\t"
        "movq 200(%%r10), %%rax\n\t"
        "movq %%rax, %%xmm2\n\t"
        "movq 216(%%r10), %%rax\n\t"
        "movq %%rax, %%xmm3\n\t"
        /* Fonction VARIADIQUE (sprintf, wsprintfA...) : l'ABI impose a
           l'appelant de copier xmm0..xmm3 dans le shadow space — l'appele
           lit ses arguments flottants varargs dans CETTE copie, jamais
           dans les registres. Sans elle, un "%f" de sprintf lisait un
           slot vide. Pour une fonction normale la copie est sans effet :
           le shadow space appartient a l'appelant et peut servir de
           brouillon. Stores xmm->mem par movq (la forme movups a
           operande memoire est inversee chez TCC, mesuree). */
        "movq %%xmm0, 0(%%rsp)\n\t"
        "movq %%xmm1, 8(%%rsp)\n\t"
        "movq %%xmm2, 16(%%rsp)\n\t"
        "movq %%xmm3, 24(%%rsp)\n\t"
        "call *232(%%r10)\n\t"           /* a partir d'ici r10/r11 sont morts */
        "movq %%rsp, %%r11\n\t"          /* r11 = R, reconstruit apres l'appel */
        "movq %%xmm0, 184(%%r11)\n\t"    /* capture du retour sur la zone */
        "movq 184(%%r11), %%rbx\n\t"     /* rbx = bits du double (sortie) */
        "movq 168(%%rsp), %%rsi\n\t"
        "movq 176(%%rsp), %%rdi\n\t"
        "movq 160(%%rsp), %%rcx\n\t"     /* rcx = E, relu dans le slot */
        "movq %%rcx, %%rsp\n\t"          /* restauration : rsp = E */
        "cld\n\t"                        /* DF rendu a 0 a l'appelant de
                                            DynaCall (l'appele peut l'avoir
                                            laisse a 1) : le `cld` d'avant
                                            protegeait la copie, celui-ci
                                            protege l'appelant. */
        : "=a"(uInt), "=b"(uBits)
        : "r"(&B)
        : "rcx", "rdx", "r8", "r9", "r10", "r11", "rsi", "rdi",
          "memory", "cc");

    // Valeur de retour.
    //   * float  -> les 32 bits bas de XMM0 (uBits) ;
    //   * double -> XMM0 entier (uBits) ;
    //   * int/long/pointeur/int64 -> RAX (uInt) : l'ABI x64 n'a pas de
    //     paire edx:eax. DC_BORLAND et le retour a memoire (nRetSiz > 8)
    //     n'existent pas sous cette ABI.
    if( Flags & DC_RETVAL_MATH4 ){
      UINT_PTR u = uBits & 0xFFFFFFFFULL;    /* le float vit en bas */
      memcpy(&Res.Float, &u, 4);
      }
    else if( Flags & DC_RETVAL_MATH8 ){
      memcpy(&Res.Double, &uBits, 8);
      }
    else {
      Res.int64 = (__int64)uInt;   /* Long lit les 32 bits bas de l'union */
      }
    return Res;
}

#endif /* _WIN64 */

#include <stdio.h>
#ifdef _DEBUG
#include "testalloc.h"     /* testa_ReportLeak : le solde du distributeur */
#endif
#include "../../basext.h"


/*
*/
besSUB_ERRMSG

  switch( iError ){
    case 0x00080000: return "ERROR HAS HAPPENED";
    }
  return "Unknown dyc module error.";
besEND


besSUB_START
  pModuleObject p;

  besMODULEPOINTER = besALLOC(sizeof(ModuleObject));
  if( besMODULEPOINTER == NULL )return COMMAND_ERROR_MEMORY_LOW;
  p = besMODULEPOINTER;
  p->pDllList = NULL; /* no Dlls are loaded (by the module) when the module starts */
  return COMMAND_ERROR_SUCCESS;
besEND

besSUB_FINISH
  pModuleObject p;
  pDllLoaded pDll, pSuivant;

  p = (pModuleObject)besMODULEPOINTER;
  if( p == NULL )return 0;

  /* FUITE CORRIGEE (constat du chantier memoire) : la boucle d'origine
     liberait les DLL (FreeLibrary) mais JAMAIS les noeuds DllLoaded ni les
     pszName — a chaque fin d'interpreteur en multithread (sbhttpd : un
     bootmodu/finimodu PAR requete), un ModuleObject, autant de noeuds que
     de DLL nommees et leurs chaines rejoindaient le tas. free(NULL) est
     un no-op qui couvre le ModuleObject jamais alloue. */
  pDll = p->pDllList;
  while( pDll ){
    FreeLibrary(pDll->hDll);
    pSuivant = pDll->next;
    besFREE(pDll->pszName);
    besFREE(pDll);
    pDll = pSuivant;
    }
  p->pDllList = NULL;

  /* Le noyau appelle finimodu puis ne relit JAMAIS ModuleInternalParameters
     (finimodu est aussi une fonction ordinaire de la SLFST, comme ici) :
     le ModuleObject peut donc etre libere par le module lui-meme, et la
     double desallocation reste impossible — le pointeur NULL est le
     second signal d'arret d'un cycle finimodu qui passerait deux fois. */
  besFREE(p);
  besMODULEPOINTER = NULL;
  return 0;
besEND

/* This function loads the requested DLL safely. This means that the DLL will
   only be loaded calling LoadLibrary only if it was not loaded yet. In other
   cases the function returns without calling the system function. Thus a DLL
   will only be loaded only once by the module.
   
   The function returns TRUE if the module was loaded successfully and FALSE
   otherwise.
   
   The function can be called using the macro bLibraryLoaded(x) without the
   special arguments.
   
   The function can be called from within any besFUNCTION function. The arguments
   passed to this function allow the function to call the bes macros from
   within this function especially to call the memory allocation functions.
   
   Note that this function does not check if a library is specified with
   different names. For example if the library is named 'simple.dll' and also
   'c:\\winnt\\system32\\simple.dll' the function will load the dll twice.
   
*/
#define LibraryLoaded(pszName) F_LibraryLoaded(pSt,ppModuleInternal,pszName)
static HMODULE F_LibraryLoaded(pSupportTable pSt,
                            void **ppModuleInternal,
                            char *pszName){
  pDllLoaded pDll,pNew;
  pModuleObject p = (pModuleObject)besMODULEPOINTER;

  pDll = p->pDllList;

  while( pDll ){
    if( ! stricmp(pszName,pDll->pszName) )break;
    pDll = pDll->next;
    }
  if( pDll )
    return pDll->hDll;

  pNew = (pDllLoaded)besALLOC(sizeof(DllLoaded));
  /* If there is no memory then the DLL can not be loaded. The function
     design does not allow any more specific error handling. */
  if( NULL == pNew )return NULL;
  
  /* allocate space for the name of the library to be loaded */
  pNew->pszName = besALLOC(strlen(pszName)+1);
  if( NULL == pNew->pszName ){
    besFREE(pNew);
    return NULL;
    }
  /* copy the name of the library into the structure thus it can be checked
     against library names to avoid double loading */
  strcpy(pNew->pszName,pszName);
  
  /* link the new structure to the head of the list */
  pNew->next = p->pDllList;
  p->pDllList = pNew;
  pNew->hDll = LoadLibrary(pNew->pszName);
  return pNew->hDll;
  }

/**
=section dyc
.function dyc
=H DYC("format", arguments)

=verbatim
import dyc.bas
print dyc::dyc("ms,i,USER32.DLL,MessageBox,PZZL",0,"test message","title",3)
=noverbatim

This function calls an arbitrary function from an arbitrary dll. The first argument to the function has to be a format string
and the rest of the arguments are the arguments for the function to be called. The format string has to specify the calling
convention of the function, the return value, the name of the DLL and the function to call and the argument types. These have
to be specified one after the other separated by commas. The format string should not contain space.

The format string has the following format:

T<"Xc,Xr,DllName,FunctionName,Xargs">

where

=itemize
=item T<Xc> specifies the calling convention
=item T<Xr> specifies the return value
=item T<DllName> is the name of the DLL
=item T<FunctionName> is the name of the function
=item T<Xargs> specifies the arguments
=noitemize

When the function is called the arguments are converted from their BASIC value and the function is called according to the
format specification. However note that a misformed format string can cause access violation in the program and thus stopping
the process. Therefore it is recommended that you fully debug your code and the way you use this function. It may be a wise
idea not to install this module on a server where different programmers can develop their programs and run in shared process
n multiple threads. For example a hosted web server running the Eszter SB Application Engine can be stopped by a BASIC program
using this external module.

In the following I describe the format string specifiers.

=itemize
=item T<Xc> CALLING CONVENTION
=noitemize

The calling convention can be one, two or at most three characters. The character T<m> or T<M> means that the code was compiled
using Microsoft compiler. This is the default behavour, thus there is no need to specify this. The opposite is T<b> or T<B>
meaning that the code was compiled using Borland compiler. The difference between these two compilers is how the return value
is passed back to the caller. You should not use both T<b> and T<m> at a time. Actually T<m> will be ignored.

The calling convention can also be T<s> or T<S> meaning standard callign convention or T<c> or T<C> meaning language C
calling convention. Only one of them is to be used in a function call. If you are callign some function from a Windows system
DLL then it is certainly T<s>. If you do not know which to use write a small test program and experiment.

The difference between standard and C calling convention is the order of the arguments placed on the stack and also who the
responsible is to clean the arguments from the stack (the called function or the calling code).

Finally you can specify T<4> or T<8> to specify that the function is returning a four or eight-byte floating point number.
Although this is a kind of return value specification, it is stated here, because this affects the calling convention. These
values are returned not in a memory place from the function but rather in the co-processor register and function T<dyc> has
to know to fetch them from there rather than expection the function to return a four or eight-byte memory chunk.

=itemize
=item T<Xr> RETURN VALUE
=noitemize

The return value should be specified using a single character. This can be:

T<i> or T<I> T<int>, T<l> or T<L> T<long>, T<p> or T<P> pointer, T<f> or T<F> T<float>,
T<d> or T<D> for T<double> or T<v> or T<V> for T<__int64>.

The int and long types are converted to a BASIC integer number, which is stored as a long in ScriptBasic. float and double values
are returned as real number, which is stored as double in ScriptBasic. A pointer value is converted to long and is returned in an
integer value. An T<__int64> value is returned as an 8 byte string copiing the bytes of the original T<__int64> value to the bytes
of the BASIC string.

=itemize
=item T<DllName>
=noitemize

This parameter has to specify the name of the DLL. This name will be used to load the DLL calling the system function T<LoadLibrary>.
This means that the name can but also may not include the full path to the file. In the latter case the system function will search
the path for the DLL as specified int he Microsoft provided documentation for the function T<LoadLibrary>.

When a function from a certain DLL is called first the module loads the DLL and when the BASIC program finishes and the module is
unloaded it unloads all DLLs it loaded. Any DLL by the module will only be loaded once. Whent he module is used in a multi-thread
environment the interpreter threads load and unload the DLLs independently. If you do not understand what it means then just ignore
this explanation: nothing to worry about.

=itemize
=item T<FunctionName>
=noitemize

The name of the function to be called from the certain DLL. If the function is not present in the DLL then the program tries to
use the function with the original name with an 'A' appended to it. Many system functions happen to have this format in the
Windows librares.

=itemize
=item T<Xargs> argument types
=noitemize

This parameter should specify the arguments. It has to have as many character as many arguments there are. Each character should specify
exactly one argument and will control how the actual BASIC arguments are converted to their native format. For each argument one of the
following characters can be used:

=itemize
=item T<1>
=item T<2>
=item T<4>
=item T<8> specifies that the argument is an arbitrary 1-, 2-, 4- or 8-byte argument. The BASIC argument should be string value and
should have at least as many characters as needed (1, 2, 4 or 8 as specified).

It is possible to use undefined, integer or real value for 1-, 2- or 4-byte values. In this case the value will be converted to integer and
the bytes of the value will be used as argument. In case of 8-byte argument the BASIC argument is converted to string.

=item T<c> specifies that the argument is a single character. If the BASIC argument is a string then the first character of the string is
used. If the argument is a real number or an integer number then the value will be used as ASCII code. If the argument is T<undef> or if the
string has no characters in it then the value will be zero.

=item T<s> specifies that the argument is a short(2-byte) value. The BASIC argument is converted to an integervalue if needed and
truncated to two bytes if needed.

=item T<f> specifies that the argument is a float value. The BASIC argument is converted to a real value and its precision is decreased
from double to float.
=item T<h>
=item T<p>
=item T<l> specifies that the argument is a handle, pointer or long. In these cases the BASIC argument is converted to an integer value if
needed.

=item T<z> specifies that the argument is a pointer that should point to a zero terminated string. The BASIC argument is converted to string and
a pointer to the start of the string is passed as actual argument. Note that BASIC strings are not zero terminated and the function T<dyc>
does not append the terminating zero character to the string. Thus you have to append a zero character to the BASIC string before you
pass it as zero terminated string. For example:

=verbatim
import dyc.bas

a$ = "message"
a$ &= " text"

REM Make the string zero character terminated
a$ &= chr$(0)

print dyc::dyc("ms,i,USER32.DLL,MessageBox,PZZL",0,a$,"title",3)
=noverbatim

On the other hand you can safely use string constants as argument like in the example above T<"title"> because string constants in ScriptBasic
contain an extra zero character following their normal characters.

=item T<d> specifies that the argument is a double value. The BASIC argument is converted to a real value if needed, which is stored in BASIC
internally as double and is passed to the function.

=item T<S> as the LAST character of the format (majuscule, so a legitimate final short argument 's' stays what it is) requests a STRUCTURE
return: the function fills a caller-provided buffer instead of returning a register value. Announce it with the return code 's' (lowercase)
and pass as the last BASIC argument that buffer, a string of at least 16 bytes (create it with string(n,chr$(0))). The dyc call returns 0
and the caller reads the filled string. Example: lldiv of ucrtbase.dll returns a 16-byte lldiv_t (quotient and remainder as 64-bit integers):

=verbatim
import dyc.bas
' long long passed as 8-byte binary strings (type 8), 16-byte buffer last
a$ = chr$(100) & string(7,chr$(0))
b$ = chr$(7) & string(7,chr$(0))
res$ = string(16,chr$(0))
dyc::dyc("cs,s,ucrtbase.dll,lldiv,88S",a$,b$,res$)
=noverbatim

=noitemize

Note that this is a wise idea to write a wrapper function in BASIC that gets the arguments performs some checks if needed and calls the
module T<dyc> instead of putting the dyc call into the main BASIC code.

*/
besFUNCTION(dyc)
  pModuleObject p;
  VARIABLE Argument;
  int Flags;
  DWORD_PTR lpFunction;
  int nArgs;
  DYNAPARM *Parm;
  int nRetSiz;
  char *pszFormat;
  DWORD cbFormat;
  int iParseState;
  char cRet;
  char cArg;        /* dernier caractere de la section des arguments :
                       'S' distingue le retour de structure du type 's' (short) */
  /* nArgs < 0 tant que la section des arguments n'a pas ete traitee : le
     cas "zero argument" se reconnait ainsi APRES la boucle de parse (voir
     plus bas : l'etat 4 n'est jamais atteint quand la section est vide). */
  char szDll[MAX_PATH];
  char szFunction[MAX_PATH];
  char *pszS;
  int i;
  HMODULE hDll;
  T_RESULT RetVal;
  float fTmp;
  int AllocSlots = 0;   /* instrument memoire : slots DYNAPARM actifs, voir le bilan final */

  p = (pModuleObject)besMODULEPOINTER;
  besRETURNVALUE = NULL;
  cArg = 0;
  nRetSiz = 0;
  nArgs = -1;   /* sentinel : l'etat 4 seul pose nArgs ; voir apres la boucle */
  Parm = NULL;
  /* Sortie d'erreur unique : les chemins d'echec APRES l'allocation de Parm
     (type d'argument inconnu, fonction absente, tampon trop court) fuyaient
     le tableau ; la macro libere quand il existe, sans effet avant. */
#define DYC_ERROR(e) do{ if( Parm )besFREE(Parm), Parm = NULL, AllocSlots--; return (e); }while( 0 )

  Argument = besARGUMENT(1);
  besDEREFERENCE(Argument);

  /* if argument is undef then raise the error */
  if( Argument == NULL )return COMMAND_ERROR_FEW_ARGS;
  /* make sure that the format argument is a string */
  Argument = besCONVERT2STRING(Argument);
  pszFormat = STRINGVALUE(Argument);
  cbFormat = STRLEN(Argument);
  if( cbFormat < 1 )return COMMAND_ERROR_ARGUMENT_TYPE;
  iParseState = 0;
  Flags = 0;
  while( cbFormat ){
    switch( iParseState ){
      case 0: //define the call type
        switch( *pszFormat ){
          case 'm': case 'M': Flags |= DC_MICROSOFT;    break;
          case 'b': case 'B': Flags |= DC_BORLAND;      break;
          case 'c': case 'C': Flags |= DC_CALL_CDECL;   break;
          case 's': case 'S': Flags |= DC_CALL_STD;     break;
          /* 't' : thiscall Microsoft (methodes C++ non varargs, interfaces
             COM) : ECX = this, TOUS les autres arguments sur la pile —
             c'est la definition MSDN, verifiee sur des objets COM reels
             (CoGetMalloc/IMalloc, methode Alloc). Une premiere version
             detournait aussi le deuxieme argument vers EDX : c'est du
             fastcall-avec-this, pas du thiscall (mesure sur QueryInterface
             : un arg2 vers EDX laisse l'appele lire son premier argument
             de pile dans le vide). L'option est insensible a PROBES_LIVE :
             elle sert les objets reels du test test/dyc_com.bas. */
          case 't': case 'T': Flags |= DC_CALL_STD | DC_CALL_THIS; break;
          case '4':           Flags |= DC_RETVAL_MATH4; break;
          case '8':           Flags |= DC_RETVAL_MATH8; break;
          case ',' : iParseState++; break;
          }
        cbFormat --;
        pszFormat++;
        break;
      case 1: // define the return value using a single character
        if( cbFormat < 1 )return COMMAND_ERROR_ARGUMENT_TYPE;
        cRet = tolower(*pszFormat);
        switch( cRet ){
          case 'I': case 'i': nRetSiz = sizeof(int);    break;
          case 'L': case 'l': nRetSiz = sizeof(long);   break;
          case 'P': case 'p': nRetSiz = sizeof(void *); break;
          case 'F': case 'f': nRetSiz = sizeof(float);  break;
          case 'D': case 'd': nRetSiz = sizeof(double); break;
          case 'V': case 'v': nRetSiz = sizeof(__int64);break;
          /* RETOUR 8 OCTETS PAR REGISTRES ('r'/'R') : la structure de
             8 octets est rendue dans la PAIRE de registres — EAX:EDX en
             32 bits (le chemin existait deja dans le moteur d'origine :
             ((DWORD*)pRet)[0] = eax ; [1] = edx — mais aucune lettre ne
             l'exposait), RAX:RDX en 64 bits. nRetSiz = 8 declenche le
             chemin pRet != NULL sans pointeur cache (le moteur x64 ne le
             pose que si nRetSiz > 8, le 32 bits que si DC_BORLAND en plus). */
          case 'r': case 'R': nRetSiz = 8;              break;
          case 's': case 'S': nRetSiz = 16; /* retour de structure ; voir
                                               le 'S' final de la section */ break;
          default: DYC_ERROR(COMMAND_ERROR_ARGUMENT_RANGE);
          }
        cbFormat --;
        pszFormat++;
        if( cbFormat < 1 )return COMMAND_ERROR_ARGUMENT_TYPE;
        if( *pszFormat != ',' )DYC_ERROR(COMMAND_ERROR_ARGUMENT_RANGE);
        cbFormat --;
        pszFormat++;
        iParseState++;
        break;
      case 2: // define the name of the dll
      case 3: //define the name of the function
        if( iParseState == 2 )pszS = szDll; else pszS = szFunction;
        i = 0;
        while( cbFormat && *pszFormat != ',' ){
          pszS[i] = *pszFormat;
          pszFormat ++;
          i++;
          cbFormat --;
          if( i >= MAX_PATH )DYC_ERROR(COMMAND_ERROR_ARGUMENT_RANGE);
          }
        if( cbFormat < 1 )DYC_ERROR(COMMAND_ERROR_ARGUMENT_RANGE);
        cbFormat--;
        pszFormat++;
        pszS[i] = (char)0;
        iParseState++;
        break;
      case 4: // define the arguments
        //first of all get sure that the library loads
        if( NULL == (hDll = LibraryLoaded(szDll)) )DYC_ERROR(COMMAND_ERROR_MODULE_NOT_LOADED);
        // the rest of the format string should define the arguments, each character
        // should specify exactly one argument, thus the number of characters left
        // should give exactly the number of arguments
        /* RETOUR DE STRUCTURE (plus de 8 octets) : un 'S' FINAL — majuscule
           uniquement, pour ne pas ambiguer un type 's' (short) qui serait
           legitiment le dernier argument — marque la convention "la fonction
           rend sa structure dans un tampon fourni par l'appelant", annoncee
           par le code de retour 's'. Le tampon est une chaine BASIC passee
           en dernier argument, grande d'AU MOINS nRetSiz octets ; le moteur
           lui-meme place le pointeur en argument cache. Le 'S' n'est pas un
           argument, il est retire de la section avant le parse. */
        cArg = pszFormat[cbFormat-1];
        if( cArg == 'S' ){
          /* 16 octets = retour de structure par pointeur cache ; si nRetSiz
             vaut deja 8 (retour 'r' par registres), on ne l'ecrase pas. */
          if( nRetSiz != 8 )nRetSiz = 16;
          cbFormat -= 1;
          nArgs = (int)cbFormat;
          if( nArgs < 1 )DYC_ERROR(COMMAND_ERROR_ARGUMENT_RANGE);
          /* Le tampon doit etre LE dernier argument : tout decalage ferait
             parser le tampon comme argument, et l'argument reel (ou rien)
             comme tampon. Strict ici, laissant la tolerance de l'original
             aux types ordinaires. */
          if( (int)besARGNR != nArgs + 2 )DYC_ERROR(COMMAND_ERROR_FEW_ARGS);
          }
        else
          nArgs = cbFormat;
        Parm = besALLOC(nArgs*sizeof(DYNAPARM));   /* libere par DYC_ERROR/besFREE final */
        if( NULL == Parm )DYC_ERROR(COMMAND_ERROR_MEMORY_LOW);
        AllocSlots++;   /* instrument : une seule entree, une seule sortie par appel */
        /* Les types d'un ou deux octets n'ecrivent qu'un champ partiel de
           dwArg (memcpy de 1 octet, cast short) : sans mise a zero, les
           octets hauts restent aux dechets du tas et partent dans la pile
           d'appel (mesure d'origine : toupper("b") recevait 0x64636262).

           FIXTURE DU DEFAUT (DYC_SANS_MEMSET). La mise a zero est remplacee
           par un REMPLISSAGE A 0x5A, et non simplement retiree : mesure
           faite, un bloc rendu par le distributeur du noyau sur cette machine
           etait NUL, si bien que le defaut ne se manifestait pas du tout —
           une fixture dont la signature depend du hasard du tas ne prouverait
           rien. 0x5A tient le role des dechets sans en avoir l'inconstance :
           c'est le meme defaut (les octets hauts d'un '1'/'2' ne sont pas a
           zero) rendu DECIDABLE. La ligne « abs('b') par '1' » de
           test/dyc_fixture.bas en mesure alors 0x5A5A5A62 au lieu de 98. */
#ifdef DYC_SANS_MEMSET
        memset( Parm, 0x5A, nArgs*sizeof(DYNAPARM) );
#else
        memset( Parm, 0, nArgs*sizeof(DYNAPARM) );
#endif
        i = 0; // count the arguments
        while( cbFormat ){
          if( i <= besARGNR-1 ){
            Argument = besARGUMENT(i+2);
            besDEREFERENCE(Argument);
            }else{
            Argument = NULL;
            }
          switch( *pszFormat ){
            case '1': // just any 1-byte argument
              Parm[i].nWidth  = 1;
              Parm[i].dwFlags = 0;
              if( NULL == Argument ){
                Parm[i].dwArg = 0;
                }else
                switch( TYPE(Argument) ){
                  case VTYPE_STRING:
                    if( STRLEN(Argument) < 1 )
                      Parm[i].dwArg = 0;
                    else
                      memcpy(&(Parm[i].dwArg),STRINGVALUE(Argument),1);
                    break;
                  case VTYPE_UNDEF:
                  case VTYPE_LONG:
                  case VTYPE_DOUBLE: Parm[i].dwArg = 0xFF & (unsigned long)besGETLONGVALUE(Argument); break;
                  default: DYC_ERROR(COMMAND_ERROR_ARGUMENT_RANGE);
                  }
              /* FALLTHROUGH DE L'ORIGINAL, CORRIGE : sans break, '1' tombait
                 dans '2', '4' puis '8' — ce dernier exigeant une chaine de
                 8 octets, TOUT argument '1' sur une chaine courte levait
                 l'erreur 8. Le type n'a probablement jamais fonctionne.
                 PARSEUR_BREAK (fixture DYC_ANCIEN_PARSEUR) : vide quand la
                 campagne reconstruit ce parseur-la. */
              PARSEUR_BREAK
            case '2': // just any 2-byte argument
              Parm[i].nWidth  = 2;
              Parm[i].dwFlags = 0;
              if( NULL == Argument ){
                Parm[i].dwArg = 0;
                }else
                switch( TYPE(Argument) ){
                  case VTYPE_STRING:
                    if( STRLEN(Argument) < 2 )
                      Parm[i].dwArg = 0;
                    else
                      memcpy(&(Parm[i].dwArg),STRINGVALUE(Argument),2);
                    break;
                  case VTYPE_UNDEF:
                  case VTYPE_LONG:
                  case VTYPE_DOUBLE: Parm[i].dwArg = 0xFFFF & (unsigned long)besGETLONGVALUE(Argument); break;
                  default: DYC_ERROR(COMMAND_ERROR_ARGUMENT_RANGE);
                  }
              PARSEUR_BREAK /* fallthrough de l'original corrige : '2' ecrasait nWidth a 4 */
            case '4': // just any 4-byte argument
              Parm[i].nWidth  = 4;
              Parm[i].dwFlags = 0;
              if( NULL == Argument ){
                Parm[i].dwArg = 0;
                }else
                switch( TYPE(Argument) ){
                  case VTYPE_STRING:
                    if( STRLEN(Argument) < 4 )
                      Parm[i].dwArg = 0;
                    else
                      memcpy(&(Parm[i].dwArg),STRINGVALUE(Argument),4);
                      break;
                  case VTYPE_UNDEF:
                  case VTYPE_LONG:
                  case VTYPE_DOUBLE: Parm[i].dwArg = (long)besGETLONGVALUE(Argument); break;
                  default: DYC_ERROR(COMMAND_ERROR_ARGUMENT_RANGE);
                  }
              PARSEUR_BREAK /* fallthrough de l'original corrige : '4' tombait dans '8' */
            case '8':
              Argument = besCONVERT2STRING(Argument);
              if( STRLEN(Argument) < 8 )DYC_ERROR(COMMAND_ERROR_ARGUMENT_RANGE);
              Parm[i].nWidth  = 8;
              Parm[i].dwFlags = DC_FLAG_ARGPTR;
              Parm[i].pArg = STRINGVALUE(Argument);
              break;

            case 'c': case 'C': //char
              Parm[i].nWidth  = 1;
              Parm[i].dwFlags = 0;
              if( NULL == Argument ){
                Parm[i].dwArg = 0;
                }else
                switch( TYPE(Argument) ){
                  case VTYPE_STRING:
                    if( STRLEN(Argument) < 1 )
                      Parm[i].dwArg = 0;
                    else
                      Parm[i].dwArg = *STRINGVALUE(Argument);
                    break;
                  case VTYPE_UNDEF:
                  case VTYPE_LONG:
                  case VTYPE_DOUBLE: Parm[i].dwArg = (char)besGETLONGVALUE(Argument); break;
                  default: DYC_ERROR(COMMAND_ERROR_ARGUMENT_RANGE);
                  }
              PARSEUR_BREAK /* fallthrough de l'original corrige : 'c' tombait dans
                        's','f','h/p/l' — le dernier ecrasait dwArg avec le
                        POINTEUR de la chaine ('c' par toupper ne rendait plus 66) */
            case 's': case 'S': //short
              Parm[i].nWidth  = 2;
              Parm[i].dwFlags = 0;
              Parm[i].dwArg = (short)besGETLONGVALUE(Argument); break;
              break;

            case 'f': case 'F': //float
              Parm[i].nWidth  = 4;
              Parm[i].dwFlags = DC_FLAG_FLOATARG;
              fTmp = (float)besGETDOUBLEVALUE(Argument);
              memcpy(&(Parm[i].dwArg) , &fTmp, sizeof(float)) ; break;
              break;
              
            case 'h': case 'H': //handle
            case 'p': case 'P': //pointer
            case 'l': case 'L': //long
#ifdef _WIN64
              /* x64 : une adresse fait 8 octets ; une STRING passee en 'L'
                 est l'adresse de son tampon (l'ancien chevauchement
                 v.l/buffer, que le cast DWORD tronquait en 64 bits). Le
                 flag PTRVAL dit au marshalling que pArg EST la valeur a
                 passer. En 32 bits, le chemin d'origine est conserve. */
              Parm[i].nWidth  = 8;
              Parm[i].dwFlags = DC_FLAG_PTRVAL;
              if( NULL == Argument )
                Parm[i].pArg = NULL;
              else if( TYPE(Argument) == VTYPE_STRING )
                Parm[i].pArg = STRINGVALUE(Argument);
              else
                Parm[i].pArg = (void *)besGETLONGVALUE(Argument);
              break;
#else
              /* 32 bits : meme semantique que le chemin x64 — une STRING
                 passee en 'L'/'p'/'h' donne l'adresse de son tampon. Le
                 chevauchement historique v.l/buffer le garantissait ;
                 GetLongValue, lui, convertit le texte en nombre ("abc" -> 0),
                 ce qui rendait le chemin inutilisable — voire crashant, ex.
                 wsprintfA avec un tampon de sortie en premier argument. */
              Parm[i].nWidth  = 4;
              Parm[i].dwFlags = 0;
              if( NULL == Argument )
                Parm[i].dwArg = 0;
              else if( TYPE(Argument) == VTYPE_STRING )
                Parm[i].dwArg = (DWORD)STRINGVALUE(Argument);
              else
                Parm[i].dwArg = (DWORD)besGETLONGVALUE(Argument); break;
              break;
#endif

            case 'z': case 'Z': //ZCHAR
              Parm[i].nWidth  = 4;
              Parm[i].dwFlags = DC_FLAG_PTRVAL;
              Argument = besCONVERT2STRING(Argument);
              Parm[i].pArg = STRINGVALUE(Argument); break;
              break;

            case 'd': case 'D': //double
              Argument = besCONVERT2DOUBLE(Argument);
              Parm[i].nWidth  = 8;
              Parm[i].dwFlags = DC_FLAG_ARGPTR | DC_FLAG_FLOATARG;
              Parm[i].pArg = & (DOUBLEVALUE(Argument));
              break;
            default: DYC_ERROR(COMMAND_ERROR_ARGUMENT_RANGE);
            }
          i++;
          cbFormat--;
          pszFormat++;
          }
        break;
      }
    }

  /* LACUNE DE L'ORIGINAL, CORRIGEE : si le format se termine par la virgule
     des arguments ("ms,i,KERNEL32.DLL,GetTickCount," — zero argument), la
     boucle s'arrete AVANT d'entrer dans l'etat 4 : hDll, szFunction, nArgs
     et Parm restaient non initialises et l'appel echouait sur l'erreur 17
     alors que la fonction existe (ou pire). La section vide est desormais
     traitee ici : charge de la DLL et appel avec nArgs = 0, tableau NULL —
     ce que les deux moteurs gerent naturellement. Un format qui s'arrete
     AVANT la section des arguments reste une erreur. */
  if( nArgs < 0 ){
    if( iParseState != 4 )DYC_ERROR(COMMAND_ERROR_ARGUMENT_RANGE);
#ifdef DYC_ANCIEN_ZEROARG
    /* FIXTURE DU FORMAT A ZERO ARGUMENT (DYC_ANCIEN_ZEROARG) : avant le
       correctif, un format qui se terminait par la virgule des arguments
       ("ms,i,KERNEL32.DLL,GetTickCount,") n'entrait jamais dans l'etat 4 — la
       boucle s'y arretait — et l'appel sortait sur l'erreur 17 (le nom de
       fonction, jamais renseigne, ne se resolvait pas : erreur « fonction du
       module ») alors que la fonction existe. La fixture rejoue ce symptome. */
    DYC_ERROR(COMMAND_ERROR_MODULE_FUNCTION);
#endif
    if( NULL == (hDll = LibraryLoaded(szDll)) )DYC_ERROR(COMMAND_ERROR_MODULE_NOT_LOADED);
    nArgs = 0;
    Parm = NULL;
    }

  /* Le retour de structure s'annonce par le code de retour 's' ET le 'S'
     final : les deux moities doivent coherer. Le retour 8 octets par
     registres ('r'/'R') recupere aussi un tampon en dernier argument,
     rempli depuis la PAIRE de registres — on le traite par le meme canal
     'S', apres avoir verifie que nRetSiz est bien 8. */
  if( cRet == 'r' || cRet == 'R' ){
    if( nRetSiz != 8 )DYC_ERROR(COMMAND_ERROR_ARGUMENT_RANGE);
    cArg = 'S';
    }
  /* Cohérence des deux moitiés : un tampon final ('S') exige un code de
     retour qui l'annonce — 's' (structure par pointeur cache) ou 'r'/'R'
     (8 octets par registres) ; réciproquement, 's' sans 'S' est une erreur. */
  if( cArg == 'S' ){
    if( cRet != 's' && cRet != 'r' && cRet != 'R' )
      DYC_ERROR(COMMAND_ERROR_ARGUMENT_RANGE);
    }
  else if( cRet == 's' )DYC_ERROR(COMMAND_ERROR_ARGUMENT_RANGE);

  /* CIBLE ADRESSE ('@' en tete du nom de fonction) : lpFunction est donne
     PAR LE PROGRAMME, en hexadecimale dans szFunction. C'est le moyen
     d'appeler une methode lue dans une VTABLE : l'appelant lit la vtable
     (par ex. via memcpy), construit "@%x" avec lpvtable[i], et le moteur
     n'a plus qu'a appeler ce pointeur. Le passage par l'hexadecimale
     (au lieu d'un argument) preserve la grammaire du format — l'adresse
     est une CIBLE, pas un argument, et nArgs reste le nombre de vrais
     arguments. Le champ DLL reste requis par la grammaire (il est charge
     puis ignore — nommer KERNEL32.DLL convient). AVANT SearchProcAddress :
     un nom commençant par @ n'est pas un symbole. */
  if( szFunction[0] == '@' ){
    char *pszFin;
    lpFunction = (DWORD_PTR)strtoull(szFunction+1,&pszFin,16);
    if( pszFin == szFunction+1 || *pszFin != '\0' || lpFunction == 0 )
      DYC_ERROR(COMMAND_ERROR_MODULE_FUNCTION);
    }
  else{
    lpFunction = (DWORD_PTR)SearchProcAddress(hDll,szFunction);
    if( lpFunction == 0 )DYC_ERROR(COMMAND_ERROR_MODULE_FUNCTION);
    }
  if( cArg == 'S' ){
    /* Retour de structure : le dernier argument BASIC est le tampon de
       reception (chaine d'au moins nRetSiz octets, typiquement creee par
       string(n,chr$(0))). On le convertit en chaine, on verifie la taille,
       et DynaCall place lui-meme le pointeur cache. */
    VARIABLE Buf;
    Buf = besARGUMENT(nArgs+2);
    besDEREFERENCE(Buf);
    if( Buf == NULL )DYC_ERROR(COMMAND_ERROR_FEW_ARGS);
    Buf = besCONVERT2STRING(Buf);
    if( STRLEN(Buf) < (unsigned long)nRetSiz )
      DYC_ERROR(COMMAND_ERROR_ARGUMENT_RANGE);
    RetVal = DynaCall(Flags,lpFunction,nArgs,Parm,STRINGVALUE(Buf),nRetSiz);
    /* Retour 8 octets par registres : EAX:EDX (32 bits) remplissent les
       deux DWORD du tampon par le chemin d'origine du moteur 32 bits ; en
       x64, une structure de 8 octets (UN eightbyte) revient dans RAX seul
       — l'ABI ne fait pas de paire RAX:RDX — et le moteur laisse l'adresse
       du tampon dans Res (pointeur cache). On copie donc RAX dans le
       tampon pour donner au BASIC les 8 octets. */
    if( cRet == 'r' || cRet == 'R' ){
#ifndef _WIN64
      memcpy(&(RetVal.Int),STRINGVALUE(Buf),8);
#else
      memcpy(STRINGVALUE(Buf),&(RetVal.int64),8);
#endif
      }
    }
  else
    RetVal = DynaCall(Flags,lpFunction,nArgs,Parm,NULL,0);
  if( Parm )besFREE(Parm), Parm = NULL, AllocSlots--;   /* double free impossible (DYC_ERROR idem) */
  switch( cRet ){
    case 'i':
      besALLOC_RETURN_LONG;
      LONGVALUE(besRETURNVALUE) = (long)RetVal.Int;
      break;    case 'l':
      besALLOC_RETURN_LONG;
      LONGVALUE(besRETURNVALUE) = RetVal.Long;
      break;
    case 'p':
      besALLOC_RETURN_LONG;
      LONGVALUE(besRETURNVALUE) = (long)RetVal.Pointer;
      break;
    case 'f':
      besALLOC_RETURN_DOUBLE;
      DOUBLEVALUE(besRETURNVALUE) = (double)RetVal.Float;
      break;
    case 'd':
      besALLOC_RETURN_DOUBLE;
      DOUBLEVALUE(besRETURNVALUE) = (double)RetVal.Double;
      break;
    case 'v':
      besALLOC_RETURN_STRING(8);
      memcpy(STRINGVALUE(besRETURNVALUE),&(RetVal.int64),8);
      break;
    case 's':
      /* Retour de structure : le resultat vit dans le tampon de l'appelant,
         pas dans RetVal — le retour de dyc reste le code de mise en oeuvre
         et l'appelant relit sa chaine (voir test/dyc.bas, lldiv). */
      besALLOC_RETURN_LONG;
      LONGVALUE(besRETURNVALUE) = 0;
      break;
    case 'r': case 'R':
      /* Retour 8 octets par registres : la moitie BASSE (quot de div) est
         lisible en entier BASIC via RetVal.Int ; les 8 octets complets
         restent dans le tampon, en lecture directe ou via 'v'. */
      besALLOC_RETURN_LONG;
      LONGVALUE(besRETURNVALUE) = (long)RetVal.Int;
      break;
    }

  /* Bilan de l'instrument memoire : le solde doit revenir a zero apres
     CHAQUE appel, quelle que soit la sortie (succes ou erreur propre). Le
     test test/mt_leak.bas lit ce bilan dans les logs du serveur. */
  if( AllocSlots != 0 )
    fprintf(stderr,"dyc : %d slots DYNAPARM non restitues au tas du noyau\n",AllocSlots);

  return COMMAND_ERROR_SUCCESS;
besEND

#ifdef PROBES_LIVE
/* FONCTION DE SONDE PROBES_LIVE : une fonction C ORDINAIRE ajoutee a la
   table SLFST quand l'option est active. La sonde BASIC l'appelle via le
   mecanisme standard de resolution des fonctions de module
   (modu_GetModuleFunctionByName) et en attend la valeur
   INTERFACE_VERSION : la table enrichie est donc prouvee lisible par le
   noyau, et la resolution par SLFST fonctionne pour une entree ajoutee.
.function ProbeLive
*/
besFUNCTION(ProbeLive)
  /* Convention des fonctions de module : la valeur C de retour est un
     CODE D'ERREURE (0 = succes), la valeur utile passe par besRETURNVALUE.
     Rendre INTERFACE_VERSION (= 11) directement etait pardefinition une
     erreur 11 pour l'interpreteur — la premiere execution l'a montre. */
  besALLOC_RETURN_LONG;
  LONGVALUE(besRETURNVALUE) = INTERFACE_VERSION;
  return COMMAND_ERROR_SUCCESS;
besEND
#endif

/* FONCTION D'AUDIT : rend le solde courant du tas audite — 0 a
   l'equilibre, sinon les octets que testa_Alloc a distribues et que
   testa_Free n'a pas repris (voir src/testalloc.c et rsp/scriba_dbg.rsp).
   En 32 bits, DynaCall malloc/free son bloc d'arguments : son solde est
   DONC visible ici, ce que test/mt_leak.bas verifie apres ses appels.
   Sans _DEBUG, testa_ReportLeak n'existe pas : la fonction rend 0 (pas
   d'audit demande), de sorte que le test passe dans les deux mondes.
.function LeakReport
*/
besFUNCTION(LeakReport)
  besALLOC_RETURN_LONG;
#if defined(_WIN32) && defined(_DEBUG) && defined(PROBES_LIVE)
  LONGVALUE(besRETURNVALUE) = (long)testa_ReportLeak();
#else
  LONGVALUE(besRETURNVALUE) = 0;
#endif
  return COMMAND_ERROR_SUCCESS;
besEND


SLFST DYC_SLFST[] ={

{ "versmodu" , versmodu },
{ "bootmodu" , bootmodu },
{ "finimodu" , finimodu },
{ "emsgmodu" , emsgmodu },
{ "dyc" , dyc },
{ "LeakReport" , LeakReport },
#ifdef PROBES_LIVE
{ "ProbeLive" , ProbeLive },
#endif
{ NULL , NULL }
  };
