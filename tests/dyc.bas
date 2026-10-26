' dyc.bas - test fonctionnel de l'extension dyc
'
' dyc est le module qui construit une pile d'appel a la main pour invoquer une
' fonction quelconque d'une DLL quelconque. C'est le seul module dont le code
' d'origine utilisait l'asm inline de Microsoft (_asm mov pStack, esp ...), que
' TinyCC refuse : son DynaCall a donc ete reecrit en asm inline GNU. Ce test
' verifie la traduction sur les SEPT chemins de retour de valeur (entier,
' long, pointeur, float, double, int64, structure/registres via tampon).
'
' Format : dyc::dyc("convention,retour,dll,fonction,types_arguments", args...)
'   m / s  Microsoft, stdcall        c  convention C (cdecl)
'   4 / 8  retour flottant dans ST (simple / double)
'   i l p f d v   type du retour : entier, long, pointeur, float, double, int64
' Types d'arguments :
'   L  entier 32 bits      Z  chaine terminee par zero (passee par pointeur)
'   D  double (8 octets, passe par pointeur)     P  pointeur
'   8  argument de 8 octets, passe par ADRESSE d'une chaine binaire de 8 octets
'   c  caractere (1 octet)  s  entier 16 bits  1/2/4  tailles explicites
'   F  flottant 32 bits (le retour exige la lettre '4')
'   S  (final, majuscule) retour de structure : le DERNIER argument BASIC est
'      le tampon de reception, et le code de retour doit etre 's' (structure
'      > 8 octets par pointeur cache, ABI 32 et 64) ou 'r' (8 octets par
'      REGISTRES : EAX:EDX en 32 bits, RAX seul en x64).
'
' La lacune du format a ZERO ARGUMENT signalee dans la distribution 1.0 build
' 2020 (section d'arguments non traitee, hDll/nArgs/Parm non initialises,
' erreur 17 sur tout format finissant par la virgule) est CORRIGEE dans ce
' depot : le cas est teste plus bas avec GetTickCount. Les fallthroughs du
' parseur d'origine ('1'->'2'->'4'->'8', 'c'->'s'->'f'->'h/p/l') et le
' dwArg non initialise sont corriges aussi — tout cela est verifie ici.

import dyc.bas

ok = 1

' ---------------------------------------------------------------- stdcall ---
' lstrlenA("ScriptBasic") doit valoir exactement 11. Cela valide le passage
' d'un argument chaine par pointeur (le format 'Z' place l'adresse de la chaine
' dans la pile) et la restauration de ESP apres un appel stdcall.
n1 = dyc::dyc("ms,i,KERNEL32.DLL,lstrlenA,Z","ScriptBasic")
print "lstrlenA(\"ScriptBasic\")          = ", n1, "  (attendu 11)\n"
if n1 <> 11 then ok = 0

' Chaine vide : zero caractere, ce qui valide aussi que la valeur 0 traversee
' par la pile n'est pas confondue avec une absence d'argument.
n2 = dyc::dyc("ms,i,KERNEL32.DLL,lstrlenA,Z","")
print "lstrlenA(\"\")                     = ", n2, "  (attendu 0)\n"
if n2 <> 0 then ok = 0

' Un argument entier par valeur : Sleep(0) rend la main immediatement.
r1 = dyc::dyc("ms,i,KERNEL32.DLL,Sleep,L",0)
print "Sleep(0)                        = ", r1, "\n"

' Retour pointeur : le handle de kernel32.dll doit etre non nul.
' PIEGE SIGNE/ASLR (x64) : le handle y tient sur 64 bits (0x00007FF8...)
' et le retour 'p' n'en rend que les 32 bits bas, LUS COMME SIGNE : le bit
' 31 allume du hasard d'ASLR rend la valeur negative — le test d'origine
' comparait "<= 0" et tombait en panne un lancement sur deux (mesure).
' L'intention est « handle non nul » : c'est exactement ce que teste le
' code, sur les deux architectures.
h1 = dyc::dyc("ms,p,KERNEL32.DLL,GetModuleHandleA,Z","kernel32.dll")
print "GetModuleHandleA(\"kernel32.dll\")  = ", h1, "\n"
if h1 = 0 then ok = 0

' ------------------------------------------------------------------ cdecl ---
' strlen est en convention C : c'est l'appelant qui nettoie la pile. Dans le
' code d'origine, ce cas empruntait le chemin `_asm add esp, dwStSize` ; la
' traduction restaure ESP inconditionnellement, ce qui doit donner le meme
' resultat.
n3 = dyc::dyc("cs,i,msvcrt.dll,strlen,Z","abcdef")
print "strlen(\"abcdef\")                 = ", n3, "  (attendu 6)\n"
if n3 <> 6 then ok = 0

' ------------------------------------------------- retour flottant double ---
' cos(0.0) = 1.0. La convention '8' indique a DynaCall que le resultat revient
' dans le registre du coprocesseur et non dans eax/edx : c'est le chemin
' `_asm fstp qword ptr [Res]` du code d'origine, traduit en `fstpl`. Le type
' d'argument 'D' exerce en plus DC_FLAG_ARGPTR, qui copie deux mots depuis la
' valeur pointee.
d1 = dyc::dyc("c8,d,msvcrt.dll,cos,D",0.0)
print "cos(0.0)                        = ", d1, "  (attendu 1)\n"
if d1 < 0.999 or d1 > 1.001 then ok = 0

d2 = dyc::dyc("c8,d,msvcrt.dll,sin,D",0.0)
print "sin(0.0)                        = ", d2, "  (attendu 0)\n"
if d2 < -0.001 or d2 > 0.001 then ok = 0

' ------------------------------------------------------------------ x64 ---
' Les cas suivants n'exercent pas seulement le chemin a UN argument : ils
' valident l'affectation des REGISTRES par POSITION (RCX/RDX/R8/R9 selon le
' rang, quel que soit le type), les arguments au-dela du quatrieme (lus sur
' la pile avec alignement), et les flottants dans XMM.
'
' wsprintfA (USER32, cdecl) rend le texte dans le tampon passe en premier :
' chaine de 64 zero (string(16,chr$(0)) = seize doubles-mots nuls, la taille
' de la reponse + marge -- wsprintfA n'a pas de limite de longueur, le
' tampon doit ABSORBER la reponse), qu'on coupe a la longueur du retour.
buf$ = string(16, chr$(0))
' arguments : tampon (L), format (Z), 10 (L), 20 (L), 30 (L), 40 (L) :
' les quatre derniers entiers occupent RDX, R8, R9 puis la pile.
r2 = dyc::dyc("cs,i,USER32.DLL,wsprintfA,LZLLLL",buf$,"%d-%d-%d-%d",10,20,30,40)
print "wsprintfA(10-20-30-40)           = ", left$(buf$,r2), "  (attendu 10-20-30-40, rendu ", r2, ")\n"
if left$(buf$,r2) <> "10-20-30-40" then ok = 0

' Quatre entiers : le troisieme et le quatrieme passent par R8 et R9, ce que
' le test precedent ne distinguait pas d'un empilement naive.
' NB (x64) : les fonctions lstr* vivent dans KERNEL32 (elles etaient
' exportees par USER32 dans le monde 32 bits ; GetProcAddress sur USER32
' echoue en 64 bits -- constat, probe gen/tpgpa.c).
s3 = dyc::dyc("ms,i,KERNEL32.DLL,lstrcmpA,ZZ","abc","abd")
s4 = dyc::dyc("ms,i,KERNEL32.DLL,lstrcmpA,ZZ","abd","abc")
print "lstrcmpA abc/abd = ", s3, "  abd/abc = ", s4, "  (attendu -1 et 1)\n"
if s3 <> -1 or s4 <> 1 then ok = 0

' MessageBoxA avec zero argument utile : valide surtout le shadow space et
' l'alignement de pile avec quatre arguments registres (MB_SERVICE... 4 = 4,
' retour IDOK=1 quand on clique OK ; ici MB_SETFOREGROUND seul, ferme par
' la touche Entree du testeur). Le cas est garde pour un essai manuel :
' une boite de dialogue ouvrirait une attente interactive.
'
' Flottant en premier argument : arg0 doit partir dans XMM0 (pas RCX), et
' arg1 entier dans RDX -- l'affectation est par position, pas par type.
' msvcrt pow(double,double) : 2.0^10 = 1024. Deux flottants : XMM0 et XMM1.
' NB : la section des arguments du format n'a PAS de virgules -- chaque
' caractere est un argument (grammaire d'origine du module).
d3 = dyc::dyc("c8,d,msvcrt.dll,pow,DD",2.0,10.0)
print "pow(2,10)                       = ", d3, "  (attendu 1024)\n"
if d3 < 1023.5 or d3 > 1024.5 then ok = 0

' Mixte : entier, flottant, entier. L'entier 0 occupe la position 0 (RCX),
' le flottant la position 1 (XMM1) et l'entier 3 la position 2 (R8).
' fmod(x,y) lit DEUX doubles, en XMM0 et XMM1 : XMM0 n'a jamais ete charge
' (le moteur l'initialise a zero), donc la fonction calcule fmod(0.0, 7.0)
' = 0.0. Si l'affectation par position etait fausse (le flottant glissant
' en XMM0), le resultat serait fmod(7.0, 3.0) = 1.0 : l'attendu distingue
' les deux cas. pow(2,10) plus haut prouve par ailleurs que XMM0 recoit
' bien un flottant quand il est en position 0.
d4 = dyc::dyc("c8,d,msvcrt.dll,fmod,LDL",0,7.0,3)
print "fmod(0,7) avec 0 pousse devant  = ", d4, "  (attendu 0)\n"
if d4 < -0.001 or d4 > 0.001 then ok = 0

' Trois puis quatre SLOTS DE PILE : au-dela du quatrieme argument, chaque
' entier doit atterrir au-dessus du shadow space (rsp+32, rsp+40, rsp+48,
' rsp+56 au moment du call -- jamais dedans, sinon le callee le chercherait
' dans sa zone de travail). Les deux appels suivants ont 7 puis 8 arguments :
' leurs 3 puis 4 derniers entiers ne vivent qu'en pile, et l'ordre du texte
' rendu prouve que chacun arrive a sa place. (WideCharToMultiByte, inspire
' d'abord pour ce cas, est un piege ici : la source doit etre de l'UTF-16,
' et "ABC" lu comme UTF-16LE donne 0x4241 -- un caractere chinois.)
kbuf$ = string(32, chr$(0))
r5 = dyc::dyc("cs,i,USER32.DLL,wsprintfA,LZLLLLL",kbuf$,"%d-%d-%d-%d-%d",1,2,3,4,5)
print "wsprintfA(3 slots de pile)      = ", left$(kbuf$,r5), "  (rendu ", r5, ")\n"
if left$(kbuf$,r5) <> "1-2-3-4-5" or r5 <> 9 then ok = 0

kbuf$ = string(32, chr$(0))
r6 = dyc::dyc("cs,i,USER32.DLL,wsprintfA,LZLLLLLL",kbuf$,"%d.%d.%d.%d.%d.%d",1,2,3,4,5,6)
print "wsprintfA(4 slots de pile)      = ", left$(kbuf$,r6), "  (rendu ", r6, ")\n"
if left$(kbuf$,r6) <> "1.2.3.4.5.6" or r6 <> 11 then ok = 0

' sprintf est VARIADIQUE : l'ABI x64 impose a l'appelant de copier xmm0..
' xmm3 dans le shadow space, car l'appele y lit ses arguments flottants
' varargs (un callee normal n'y touche jamais). Le moteur fait cette copie
' inconditionnellement : "%s %d %f" avec "ok", 7 et 0.5 doit rendre
' "ok 7 0.500000" -- sans la copie, le %f lisait un slot vide.
sbuf$ = string(32, chr$(0))
r3 = dyc::dyc("cs,i,msvcrt.dll,sprintf,LZLLD",sbuf$,"%s %d %f","ok",7,0.5)
print "sprintf('ok 7 0.5')             = ", left$(sbuf$,r3), "  (rendu ", r3, ")\n"
if left$(sbuf$,r3) <> "ok 7 0.500000" then ok = 0

' Tampon 'L' en ECRITURE, verifie au caractere pres : wsprintfA ecrit DANS
' l'adresse recue, pas dans une copie. En x64, une chaine passee comme 'L'
' transmet l'adresse de son tampon (l'union dwArg/pArg de 32 bits tronquait
' le pointeur par un cast DWORD ; GetLongValue, lui, convertit le texte en
' nombre et ne peut pas servir a passer une adresse).
cbuf$ = string(16, chr$(0))
r4 = dyc::dyc("cs,i,USER32.DLL,wsprintfA,LZ",cbuf$,"ABCDE")
print "wsprintfA('L' en ecriture)      = ", left$(cbuf$,r4), "  (rendu ", r4, ")\n"
if r4 <> 5 or left$(cbuf$,r4) <> "ABCDE" or asc(mid$(cbuf$,6,1)) <> 0 then ok = 0

' ------------------------- zero argument et retour de structure (les deux) ---
' GetTickCount ne prend AUCUN argument : le format se termine par la virgule
' de la section des arguments. La lacune de la distribution d'origine (etat
' 4 jamais atteint, variables non initialisees) est corrigee dans le moteur.
t2 = dyc::dyc("ms,i,KERNEL32.DLL,GetTickCount,")
print "GetTickCount()                  = ", t2, "\n"
if t2 <= 0 then ok = 0

' Retour de STRUCTURE de plus de 8 octets : l'ABI Microsoft place, sur les
' deux architectures, un pointeur cache vers un tampon fourni par l'APPELANT
' (RCX en x64, bas de pile en 32 bits). lldiv(long long,long long) rend un
' lldiv_t de 16 octets {quot,rem} ; ucrtbase l'exporte. Les arguments long
' long passent par le type '8' (chaine binaire de 8 octets, passee par
' adresse), le tampon de 16 octets par le 'S' final, et le code de retour
' 's' dit au moteur que le resultat vit dans le tampon de l'appelant.
a8$ = chr$(100) & string(7, chr$(0))
b8$ = chr$(7) & string(7, chr$(0))
res$ = string(16, chr$(0))
r7 = dyc::dyc("cs,s,ucrtbase.dll,lldiv,88S",a8$,b8$,res$)
q7 = asc(mid$(res$,1,1)) + 256*asc(mid$(res$,2,1)) + 65536*asc(mid$(res$,3,1)) + 16777216*asc(mid$(res$,4,1))
m7 = asc(mid$(res$,9,1)) + 256*asc(mid$(res$,10,1)) + 65536*asc(mid$(res$,11,1)) + 16777216*asc(mid$(res$,12,1))
print "lldiv(100,7)                    = quotient ", q7, ", reste ", m7, "  (attendu 14 et 2)\n"
if q7 <> 14 or m7 <> 2 then ok = 0

' ----------------- robustesse syscall : Sleep + NtQueryInformationProcess ---
' Chaque appel dyc vers ces fonctions traverse un syscall qui detruit RCX et
' R11 (l'instruction syscall y pose l'adresse de retour et les RFLAGS) et
' saccage R10 (transport de l'argument noyau) — signature mesuree par
' test/probe_sysclobber.c. Le moteur survit parce que tout son etat vit dans
' la struct DYC64, en memoire, jamais dans un registre relu apres le call :
' la boucle alterne 50 fois les deux chemins et verifie le PID rendu.
'
' NtQueryInformationProcess(-1, 0, &pbi, taille, &retlen) : 5 arguments dont
' le dernier sur la pile, retour NTSTATUS. PIEGE 32/64 BITS, celebre :
' PROCESS_BASIC_INFORMATION fait 24 octets en 32 bits, 48 en 64 (les
' pointeurs doublent), et le noyau 32 bits EXIGE exactement sa taille
' (STATUS_INFO_LENGTH_MISMATCH sinon) tandis que le 64 bits accepte plus
' grand mais ECHOUE la longueur demandee dans ReturnLength. La taille est
' donc auto-identifiee ici : on tente 24 (offset du PID : 16) ; si le noyau
' refuse, c'est le monde 64 bits, on retente 48 (offset du PID : 32, car
' l'alignement naturel des pointeurs 8 octets decal les champs).
pbi$ = string(64, chr$(0))
rl$ = string(8, chr$(0))
qlen = 24
qoff = 17
r9 = dyc::dyc("cs,p,ntdll.dll,NtQueryInformationProcess,LPLLL",-1,0,pbi$,24,rl$)
if r9 <> 0 then
  qlen = 48
  qoff = 33
  r9 = dyc::dyc("cs,p,ntdll.dll,NtQueryInformationProcess,LPLLL",-1,0,pbi$,48,rl$)
  end if
pid0 = -1
bad = 0
for i = 1 to 50
  r8 = dyc::dyc("ms,i,KERNEL32.DLL,Sleep,L",0)
  r9 = dyc::dyc("cs,p,ntdll.dll,NtQueryInformationProcess,LPLLL",-1,0,pbi$,qlen,rl$)
  pid = asc(mid$(pbi$,qoff,1)) + 256*asc(mid$(pbi$,qoff+1,1)) + 65536*asc(mid$(pbi$,qoff+2,1)) + 16777216*asc(mid$(pbi$,qoff+3,1))
  if pid0 < 0 then pid0 = pid
  if r9 <> 0 or pid <> pid0 or pid <= 0 then bad = bad + 1
next i
print "Sleep+NtQueryInfoProcess x50     = pid ", pid0, " (taille PBI ", qlen, "), ", bad, " iterations fautives\n"
if bad <> 0 then ok = 0

' ---------------- syscall a 6 arguments (3 sur la pile en x64) ------------
' NtQueryVirtualMemory(-1, addr, 0, &mbi, 64, &retlen) : le noyau lit SES
' arguments dans la pile utilisateur pointee par R10 (64 bits) ou EDX
' (32 bits) — ici les deux derniers passent obligatoirement par la pile,
' que la cible soit en x64 (5e et 6e arguments) ou en 32 bits (tout est
' empile, plus le pointeur cache du retour 'p'). Retour NTSTATUS, et
' MEMORY_BASIC_INFORMATION dont la taille discrimine encore l'architecture
' (48 en 64 bits, 28 en 32) et dont l'octet State (MEM_COMMIT = 0x1000)
' suit les pointeurs : offset 32 en 64 bits, 17 en 32 bits.
' La region interrogee est celle du tampon lui-meme : engagee par
' definition, donc State = 0x1000 a chaque tour.
mb$ = string(64, chr$(0))
rl$ = string(8, chr$(0))
moff = 17
bad = 0
for i = 1 to 20
  r8 = dyc::dyc("ms,i,KERNEL32.DLL,Sleep,L",0)
  r9 = dyc::dyc("cs,p,ntdll.dll,NtQueryVirtualMemory,LLLLLL",-1,mb$,0,mb$,64,rl$)
  mlen = asc(mid$(rl$,1,1)) + 256*asc(mid$(rl$,2,1)) + 65536*asc(mid$(rl$,3,1)) + 16777216*asc(mid$(rl$,4,1))
  if mlen = 48 then
    moff = 33
    else
    moff = 17
    end if
  state = asc(mid$(mb$,moff,1)) + 256*asc(mid$(mb$,moff+1,1)) + 65536*asc(mid$(mb$,moff+2,1)) + 16777216*asc(mid$(mb$,moff+3,1))
  if r9 <> 0 or state <> 4096 then bad = bad + 1
next i
print "NtQueryVirtualMemory x20 (6 args) = taille MBI ", mlen, ", State ", hex$(state), ", ", bad, " iterations fautives\n"
if bad <> 0 or (mlen <> 28 and mlen <> 48) then ok = 0

' ----------------- couverture des formats restants (l, c, s, F, f, v, r) ---
' 'l' : meme registre de retour que 'i', autre lecture (long).
nl = dyc::dyc("ms,l,KERNEL32.DLL,lstrlenA,Z","ScriptBasic")
print "lstrlenA en retour 'l'           = ", nl, "  (attendu 11)\n"
if nl <> 11 then ok = 0

' 'c' : argument d'un octet, lu comme premier caractere d'une chaine.
' toupper(98) = 66 (msvcrt, cdecl).
nc = dyc::dyc("cs,i,msvcrt.dll,toupper,c","b")
print "toupper('b') via argument 'c'    = ", nc, "  (attendu 66)\n"
if nc <> 66 then ok = 0

' 's' : arguments 16 bits (zero-etalement sans effet sur les positifs),
' et retour 'r' : structure de 8 octets rendue par REGISTRES. En 32 bits
' EAX:EDX remplissent les deux DWORD du tampon (chemin d'origine, enfin
' expose) ; en x64 la structure 8 octets revient dans RAX seul (un seul
' eightbyte, l'ABI ne fait pas de paire RAX:RDX) et le moteur recopie
' RAX dans le tampon. Le retour dyc donne la moitie basse (quot).
dres$ = string(8, chr$(0))
ns = dyc::dyc("cs,r,msvcrt.dll,div,ssS",100,7,dres$)
nq = asc(mid$(dres$,1,1)) + 256*asc(mid$(dres$,2,1)) + 65536*asc(mid$(dres$,3,1)) + 16777216*asc(mid$(dres$,4,1))
nr = asc(mid$(dres$,5,1)) + 256*asc(mid$(dres$,6,1)) + 65536*asc(mid$(dres$,7,1)) + 16777216*asc(mid$(dres$,8,1))
print "div(100,7) args 's' retour 'r'   = quot ", nq, ", reste ", nr, "  (attendu 14 et 2)\n"
if ns <> 14 or nq <> 14 or nr <> 2 then ok = 0

' 'F' (argument float 32 bits) et 'f' (retour float) : ucrtbase powf,
' exporte sous les deux architectures (sqrtf, lui, manque en 32 bits).
' Le retour 'f' EXIGE la lettre '4' : les deux moteurs ne lisent le
' flottant que via le chemin MATH4 (ST(0) en 32 bits, XMM0 en x64).
nf = dyc::dyc("c4,f,ucrtbase.dll,powf,FF",2.0,10.0)
print "powf(2,10) en float 'F'/'f'      = ", nf, "  (attendu 1024)\n"
if nf < 1023.9 or nf > 1024.1 then ok = 0

' 'v' : retour int64 rendu en chaine binaire de 8 octets. _strtoi64
' traverse les pointeurs : "1234567890123" = 0x11F71FB04CB, moitie haute
' 287, moitie basse 1912276171.
sv$ = dyc::dyc("cs,v,msvcrt.dll,_strtoi64,ZPL","1234567890123",0,10)
vlo = asc(mid$(sv$,1,1)) + 256*asc(mid$(sv$,2,1)) + 65536*asc(mid$(sv$,3,1)) + 16777216*asc(mid$(sv$,4,1))
vhi = asc(mid$(sv$,5,1)) + 256*asc(mid$(sv$,6,1)) + 65536*asc(mid$(sv$,7,1)) + 16777216*asc(mid$(sv$,8,1))
print "_strtoi64 retour 'v'             = ", vhi, " * 2^32 + ", vlo, "\n"
if vlo <> 1912276171 or vhi <> 287 then ok = 0

' ---------------------- thiscall (COM) : convention « objet » ----------------
' La convention thiscall de Microsoft passe le pointeur d'objet dans ECX
' (32 bits) — premier « argument registre » DEVANT la pile, EDX recevant le
' suivant, les suivants restant empiles. Sous Windows x64, le this n'a PAS
' de registre dedie : c'est simplement le premier argument de la convention
' commune, deja place en RCX par le moteur. La demonstration s'appuie sur
' des sondes embarquees du DLL dyc, compilees SEULEMENT quand la variable
' d'environnement PROBES_LIVE est non vide au build (voir la note « OPTION
' DE COMPILATION PROBES_LIVE » de src/extensions/dyc/interface.c) :
'   ProbeLive : fonction C du module, rend INTERFACE_VERSION (11) ; son
'               appel par declare BASIC prouve que le noyau resout bien la
'               sonde via la table SLFST (l'appeler VIA dyc::dyc serait
'               circulaire : le pSt du noyau n'est pas celui du moteur) ;
'   dyc_probe_this / dyc_probe_this64 : sonde asm qui rend
'               *this + dw1 + dw2 — la lecture de (%ecx)/(%rcx) prouve que
'               le registre portait le this ; la lettre 't' du format arme
'               la convention (DC_CALL_STD|DC_CALL_THIS), 's' en x64 montre
'               l'identite des deux conventions dans ce monde.
' SANS sonde (build standard), les appels rendent l'erreur propre 17
' (export absent du DLL — constat mesure, jamais un crash) : la section
' s'efface d'elle-meme et le test reste OK dans les deux configurations.

' arch64 : la taille PBI mesuree plus haut (24 en 32 bits, 48 en 64) dit
' quel monde nous traversons. Utile car la sonde « this seul » n'est
' verifiable qu'en x64 : la pile 32 bits n'y est pas zeroisee, donc le
' cas sans argument de pile y rend du garbage — limite de la sonde, pas
' du moteur (le thiscall reel a un this seul n'a de toute facon pas
' d'argument de pile ; la preuve 32 bits est l'appel this + 2 arguments).
arch64 = 0
if qlen = 48 then arch64 = 1

dll$ = "modules/dyc.dll"
fn$ = "dyc_probe_this"
if arch64 then
  dll$ = "modules64/dyc64.dll"
  fn$ = "dyc_probe_this64"
  end if

s$ = chr$(7) & chr$(0) & chr$(0) & chr$(0) & chr$(0) & chr$(0) & chr$(0) & chr$(0)
etape = 0
rprobe = 0
b1 = 0
b3 = 0
csame = 0
errthis = 0
on error goto capthis
etape = 1
rprobe = dyc::ProbeLive()
tp1:
etape = 2
b1 = dyc::dyc("t,i," & dll$ & "," & fn$ & ",p", s$)
tp2:
etape = 3
b3 = dyc::dyc("t,i," & dll$ & "," & fn$ & ",ppp", s$, 100, 23)
tp3:
etape = 4
if arch64 then csame = dyc::dyc("s,i," & dll$ & "," & fn$ & ",ppp", s$, 100, 23)
tp4:
goto suitethis
capthis:
on error goto capthis
errthis = errthis + 1
if error <> 17 then
  print "thiscall : erreur inattendue ", error, " a l etape ", etape, "\n"
  ok = 0
  end if
if etape = 1 then goto tp1
if etape = 2 then goto tp2
if etape = 3 then goto tp3
if etape = 4 then goto tp4
suitethis:
print "ProbeLive (table SLFST)         = ", rprobe, "  (0 sans sonde, 11 avec)\n"
print "thiscall *this+dw1+dw2          = ", b3, "  (130 avec sonde)\n"
print "thiscall this seul              = ", b1, "  (7 avec sonde x64, garbage attendu en 32)\n"
print "thiscall identique en 's' (x64)  = ", csame, "  (130 avec sonde x64, 0 sinon)\n"
if (rprobe = 11 and errthis = 0) or (rprobe = 0 and errthis = 3 + arch64 and b1 = 0 and b3 = 0) then
  if rprobe = 11 then
    if b3 <> 130 then ok = 0
    if arch64 then
      if b1 <> 7 or csame <> 130 then ok = 0
      end if
    end if
  else
    ok = 0
  end if

' ------------------ alignement de pile : le contrat de l'ABI i386 ---------
' La convention x86 de Windows (MSVC) exige ESP aligne sur 16 octets AU POINT
' DU call : l'appele doit donc lire esp % 16 == 12 a son entree. Le moteur
' construisait sa pile a ESP = E - 4 - dwStSize, ou dwStSize n'est qu'un
' multiple de 4 : la valeur livree dependait donc de la FORME de l'appel,
' et un appele compile avec SSE (un simple movaps vers un local aligne(16))
' tombait d'une violation d'acces 0xC0000005 — la panne de la campagne de
' charge 32 bits (502/1000 puis violation d'acces).
' Les deux sondes embarquees (voir test/dyc_probe_align.c et
' dyc_probe_align64.c, compilees SEULEMENT avec PROBES_LIVE) tranchent :
'   dyc_probe_align        rend esp % 16 (32 bits) ou rsp % 16 (x64) lu a
'                          l'entree — 12 en 32 bits, 8 en x64 ;
'   dyc_probe_align_strict fait un VRAI movaps vers un emplacement aligne sur
'                          16 et rend 1 : si le moteur ment, le processus
'                          meurt DANS la sonde (32 bits seulement : c'est la
'                          qu'un encodeur SSE de TCC est en jeu).
' Six formes sont essayees (0, 1, 2, 3, 4 arguments et un thiscall a deux
' arguments de pile) : l'alignement livre depend de la forme, pas seulement
' du point d'appel. SANS sonde, les appels rendent l'erreur propre 17 et la
' section s'efface d'elle-meme, exactement comme la precedente.
fmta$ = "ms,i," & dll$ & ",dyc_probe_align,"
conforme = 0
ral0 = 0
ral1 = 0
ral2 = 0
ral3 = 0
ral4 = 0
ralt = 0
rstrict = 0
erral = 0
' valeur attendue : ESP % 16 == 12 en 32 bits (retour empile de 4 octets),
' RSP % 16 == 8 en x64 (retour empile de 8 octets).
ralign = 12
if arch64 then ralign = 8
' Les etiquettes alN suivent l'appel qu'elles protegent : le gestionnaire
' deroute vers le label place APRES la ligne fautive (reprendre la ligne
' fautive elle-meme bouclerait sur son erreur — c'est ce qui se voyait ici).
etape = 10
on error goto capal
etape = 11
ral0 = dyc::dyc(fmta$)
al11:
etape = 12
ral1 = dyc::dyc(fmta$ & "L", 1)
al12:
etape = 13
ral2 = dyc::dyc(fmta$ & "LL", 1, 2)
al13:
etape = 14
ral3 = dyc::dyc(fmta$ & "LLL", 1, 2, 3)
al14:
etape = 15
ral4 = dyc::dyc(fmta$ & "LLLL", 1, 2, 3, 4)
al15:
etape = 16
ralt = dyc::dyc("t,i," & dll$ & ",dyc_probe_align,LLL", 99, 1, 2)
al16:
etape = 17
' `arch64 = 0`, et non `not arch64` : en BASIC le `not` est bit a bit (not 1
' vaut -2, donc vrai pour l'interpreteur) — la sonde stricte aurait ete
' appelee aussi en x64 et comptee comme une erreur 17 de plus.
if arch64 = 0 then rstrict = dyc::dyc("ms,i," & dll$ & ",dyc_probe_align_strict,LLLL", 1, 2, 3, 4)
al17:
goto suiteal
capal:
on error goto capal
erral = erral + 1
if error <> 17 then
  print "alignement : erreur inattendue ", error, " a l etape ", etape, "\n"
  ok = 0
  end if
if etape = 11 then goto al11
if etape = 12 then goto al12
if etape = 13 then goto al13
if etape = 14 then goto al14
if etape = 15 then goto al15
if etape = 16 then goto al16
if etape = 17 then goto al17
suiteal:
print "alignement ESP%16 livre         = ", ral0, ", ", ral1, ", ", ral2, ", ", ral3, ", ", ral4, "  (12 attendu en 32, 8 en x64)\n"
print "alignement thiscall this+2 arg  = ", ralt, "  (meme valeur attendue)\n"
print "movaps strict (32 bits)         = ", rstrict, "  (1 avec sonde 32 bits)\n"
if rprobe = 11 and erral = 0 then conforme = 1
if conforme = 1 and (ral0 <> ralign or ral1 <> ralign or ral2 <> ralign or ral3 <> ralign or ral4 <> ralign or ralt <> ralign) then conforme = -1
if conforme = 1 and arch64 = 0 and rstrict <> 1 then conforme = -1
' en x64 la sonde d'alignement n'est pas embarquee (voir la note de
' test/dyc_probe_align.c) : le monde x64 est alors « sans sonde » du point de
' vue de cette section, et l'absence doit se voir comme une erreur propre 17.
erralattendu = 6 + (1 - arch64)
print "sonde d'alignement : erreurs 17 = ", erral, "  (0 avec sonde, ", erralattendu, " sans)\n"
if rprobe = 11 and arch64 = 1 and erral = 6 then conforme = 3
if rprobe = 0 and erral = erralattendu then conforme = 2
if (conforme = 2 or conforme = 3) and (ral0 <> 0 or ral1 <> 0 or ral2 <> 0 or ral3 <> 0 or ral4 <> 0 or ralt <> 0 or rstrict <> 0) then conforme = -1
if conforme = 3 then print "alignement : sonde 32 bits, hors de ce monde (temoin x64 en logs/witness64.bas)\n"
if conforme = 0 then
  print "alignement : sonde presente mais contrat d'ABI non tenu\n"
  ok = 0
  end if

' ----------------- chemins d'erreur : des erreurs, jamais un crash --------
' Chaque cas fautif doit rendre une erreur BASIC propre ; le gestionnaire
' deroute vers le label place APRES la ligne fautive (resume next
' reprendrait la ligne fautive elle-meme et bouclerait). Les cibles des
' erreurs : lettre de retour inconnue (8), fonction absente (17), tampon
' trop court pour 'S' (8) et pour 'r' (8).
errcount = 0
okerr = 0
goterr = 0
ecase = 0
on error goto cap
ecase = 1
r1 = dyc::dyc("cs,q,msvcrt.dll,strlen,Z","abc")
c1:
ecase = 2
r2 = dyc::dyc("cs,i,msvcrt.dll,pasunefonction,Z","abc")
c2:
ecase = 3
tamp$ = string(4, chr$(0))
r3 = dyc::dyc("cs,s,ucrtbase.dll,lldiv,LLS",100,7,tamp$)
c3:
ecase = 4
r4 = dyc::dyc("cs,r,msvcrt.dll,div,LLS",100,7,tamp$)
c4:
ecase = 0
goto bilan
cap:
on error goto cap
goterr = error
errcount = errcount + 1
if ecase = 1 and goterr = 8 then okerr = okerr + 1
if ecase = 2 and goterr = 17 then okerr = okerr + 1
if ecase = 3 and goterr = 8 then okerr = okerr + 1
if ecase = 4 and goterr = 8 then okerr = okerr + 1
if ecase = 1 then goto c1
if ecase = 2 then goto c2
if ecase = 3 then goto c3
if ecase = 4 then goto c4
bilan:
print "erreurs propres                  = ", okerr, " / ", errcount, "  (attendu 4/4)\n"
if errcount <> 4 or okerr <> 4 then ok = 0

' ------------------------------------------------------------------ bilan ---
if ok then
  print "\ndyc : OK - registres, pile, XMM, erreurs propres et les sept chemins de retour corrects\n"
else
  print "\ndyc : ECHEC\n"
end if
