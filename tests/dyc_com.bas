' dyc_com.bas - objets COM reels traverses par le thiscall du module dyc
'
' L'epreuve ne fabrique AUCUNE structure : chaque methode appelee est celle
' d'un objet fourni par le systeme. CoGetMalloc(1, &pmalloc) rend un objet
' IMalloc ; sa vtable est lue par memcpy (via dyc), et chaque methode est
' appelee par la cible '@adresse' du moteur — '@%x' est un pointeur brut,
' celui de lpvtable[i]. Toutes les methodes COM passent par le thiscall :
' this en ECX en 32 bits, premier argument en RCX en x64.
'
' REPRESENTATION DES ADRESSES (lecon principale du chantier) :
'   * 32 bits : une adresse tient dans un long BASIC (signe, le bit 31 du
'     hasard d'ASLR la rend negative — le cast (DWORD) du moteur restitue
'     les bits) ; le this passe en 'L' numerique.
'   * 64 bits : le long BASIC y reste 32 bits (hi*2^32 deborde) — TOUTE
'     adresse y voyage en CHAINE BINAIRE de 8 octets, passee par le type
'     '8' (adresse lue 64 bits par le callee), et la representation
'     hexadecimale se fabrique octet par octet (hex8$ plus bas).
'
' Reperes de la vtable IMalloc (psize octets par entree, 4 en 32 bits,
' 8 en 64) : 0 QueryInterface, 1 AddRef, 2 Release, 3 Alloc, 4 Realloc,
' 5 Free, 6 GetSize, 7 DidAlloc, 8 HeapMinimize — Realloc existe, et une
' premiere lecture qui l'oubliait appelait Free en croyant lire GetSize
' (rendu void, donc 0 : le faux « GetSize = 0 » du premier essai).
'
' Ordre de COM verifie ici :
'   CoInitialize    S_OK (0) ou S_FALSE (1) ;
'   CoGetMalloc(1)  S_OK (0), pointeur non nul ;
'   Alloc(64)       bloc non nul ;
'   GetSize(bloc)   observation sans verdict (MSDN : peut surestimer ou
'                   rendre -1, indeterminable) ;
'   DidAlloc(bloc)  observation sans verdict (1/0/-1 selon l'allocateur) ;
'   Free(bloc)      rend la main, puis l'interpreteur rend encore 11 a
'                   lstrlenA : sans le bon this, l'entree dans Free
'                   delirerait sur son objet (acces violent certain).
'
' Lancer : bin/scriba.exe test/dyc_com.bas      (32 bits, thiscall ECX)
'          bin64/scriba64.exe test/dyc_com.bas  (64 bits, this en RCX)

import dyc.bas

' -------- outils adresses ------------------------------------------------
function bin2hex$(v$)
  ' representation hexadecimale des octets little-endian de v$
  local h$, i, b
  h$ = ""
  for i = len(v$) to 1 step -1
    b = asc(mid$(v$,i,1))
    h$ = h$ & mid$("0123456789ABCDEF", b \ 16 + 1, 1) & mid$("0123456789ABCDEF", (b % 16) + 1, 1)
    next i
  i = 1
  while i < len(h$) and mid$(h$,i,1) = "0"
    i = i + 1
    wend
  bin2hex$ = mid$(h$,i)
  if bin2hex$ = "" then bin2hex$ = "0"
  end function

function add_bin$(v$, delta)
  ' additionne delta (petit) aux octets little-endian de v$
  local r$, i, b, c
  c = delta
  r$ = v$
  for i = 1 to len(v$)
    b = asc(mid$(r$,i,1)) + (c % 256)
    c = c \ 256 + (b > 255)
    r$ = mid$(r$,1,i-1) & chr$(b % 256) & mid$(r$,i+1)
    next i
  add_bin$ = r$
  end function

function lit_bin(v$)
  ' lit les psize premiers octets de v$ comme un entier 32 bits (0 si trop
  ' grand) — sert seulement au 32 bits, ou les adresses tiennent
  local v, i
  v = 0
  for i = psize to 1 step -1
    v = v * 256 + asc(mid$(v$,i,1))
    next i
  lit_bin = v
  end function

' ------------------------------------------------------------ largeur ----
' La taille PBI, deja eprouvee dans dyc.bas : 24 en 32 bits, 48 en 64.
pbi$ = string(64, chr$(0))
rl$ = string(8, chr$(0))
r = dyc::dyc("cs,p,ntdll.dll,NtQueryInformationProcess,LPLLL",-1,0,pbi$,24,rl$)
psize = 4
if r <> 0 then psize = 8
print "architecture                    = ", psize*8, " bits\n"

ok = 1

' ------------------------------------------------------------ COM boot ---
r = dyc::dyc("ms,i,ole32.dll,CoInitialize,L",0)
print "CoInitialize                    = ", hex$(r), "  (S_OK=0, S_FALSE=1)\n"
if r <> 0 and r <> 1 then ok = 0

' CoGetMalloc(1, &pmalloc) : le tampon passe comme 'L' transmet l'adresse
' de son premier octet — la fonction y ecrit le pointeur rendu.
pbuf$ = string(8, chr$(0))
r = dyc::dyc("ms,i,ole32.dll,CoGetMalloc,LL",1,pbuf$)
print "CoGetMalloc                     = ", hex$(r), "  (attendu 0 = S_OK)\n"
if r <> 0 then ok = 0
pmalloc$ = mid$(pbuf$,1,psize)
print "IMalloc*                        = 0x", bin2hex$(pmalloc$), "\n"
nul = 1
for i = 1 to psize
  if asc(mid$(pmalloc$,i,1)) <> 0 then nul = 0
  next i
if nul then ok = 0

' La vtable : psize octets A L'ADRESSE pmalloc — la source est l'adresse
' reconstituee : en 32 bits par sa valeur numerique, en 64 bits par le
' type '8' (la chaine binaire passee par adresse EST le pointeur source).
vt$ = string(8, chr$(0))
if psize = 4 then
  r = dyc::dyc("ms,p,msvcrt.dll,memcpy,PLL",vt$,lit_bin(pmalloc$),psize)
  else
  r = dyc::dyc("ms,p,msvcrt.dll,memcpy,P8L",vt$,pmalloc$,psize)
  end if
print "lpvtable                        = 0x", bin2hex$(vt$), "\n"
nul = 1
for i = 1 to psize
  if asc(mid$(vt$,i,1)) <> 0 then nul = 0
  next i
if nul then ok = 0

' Valeur de lpvtable[i] : memcpy a l'adresse vtable + i*psize.
function entree_vtable$(i)
  local cible$, slot$
  cible$ = add_bin$(vt$, i*psize)
  slot$ = string(8, chr$(0))
  if psize = 4 then
    dyc::dyc("ms,p,msvcrt.dll,memcpy,PLL",slot$,lit_bin(cible$),psize)
    else
    dyc::dyc("ms,p,msvcrt.dll,memcpy,P8L",slot$,cible$,psize)
    end if
  entree_vtable$ = mid$(slot$,1,psize)
  end function

' ------------------------------------------------ Alloc (entree 3) --------
palloc$ = entree_vtable$(3)
print "lpfn Alloc                      = 0x", bin2hex$(palloc$), "\n"
if psize = 4 then
  nbloc = dyc::dyc("t,p,KERNEL32.DLL,@" & bin2hex$(palloc$) & ",LL",lit_bin(pmalloc$),64)
  blk$ = chr$(nbloc % 256) & chr$((nbloc \ 256) % 256) & chr$((nbloc \ 65536) % 256) & chr$((nbloc \ 16777216) % 256)
  else
  blk$ = dyc::dyc("t,v,KERNEL32.DLL,@" & bin2hex$(palloc$) & ",8L",pmalloc$,64)
  end if
print "IMalloc::Alloc(64)              = 0x", bin2hex$(blk$), "  (bloc non nul attendu)\n"
nul = 1
for i = 1 to psize
  if asc(mid$(blk$,i,1)) <> 0 then nul = 0
  next i
if nul then ok = 0

' ------------------------------------------------ GetSize (entree 6) ------
' Observation sans verdict : MSDN documente que GetSize peut surestimer
' (alignement) ou rendre -1 (indeterminable). L'apport de l'appel est de
' prouver qu'une QUATRIEME methode COM traverse le thiscall sans accident.
pgetsize$ = entree_vtable$(6)
if psize = 4 then
  taille = dyc::dyc("t,i,KERNEL32.DLL,@" & bin2hex$(pgetsize$) & ",LL",lit_bin(pmalloc$),lit_bin(blk$))
  else
  taille = dyc::dyc("t,i,KERNEL32.DLL,@" & bin2hex$(pgetsize$) & ",8L",pmalloc$,0)
  end if
print "IMalloc::GetSize(bloc)          = ", taille, "  (-1 = indeterminable, MSDN)\n"

' ------------------------------------------------ DidAlloc (entree 7) -----
' Idem : 1 = alloue par lui, 0 = non, -1 = ne peut pas dire.
pdid$ = entree_vtable$(7)
if psize = 4 then
  did = dyc::dyc("t,i,KERNEL32.DLL,@" & bin2hex$(pdid$) & ",LL",lit_bin(pmalloc$),lit_bin(blk$))
  else
  did = dyc::dyc("t,i,KERNEL32.DLL,@" & bin2hex$(pdid$) & ",8L",pmalloc$,0)
  end if
print "IMalloc::DidAlloc(bloc)         = ", did, "  (1/0/-1, cf. MSDN)\n"

' ------------------------------------------------ Free (entree 5) ---------
pfree$ = entree_vtable$(5)
if psize = 4 then
  dyc::dyc("t,i,KERNEL32.DLL,@" & bin2hex$(pfree$) & ",LL",lit_bin(pmalloc$),lit_bin(blk$))
  else
  dyc::dyc("t,i,KERNEL32.DLL,@" & bin2hex$(pfree$) & ",8L",pmalloc$,0)
  end if
print "IMalloc::Free(bloc)             = rendu\n"
n = dyc::dyc("ms,i,KERNEL32.DLL,lstrlenA,Z","ScriptBasic")
print "lstrlenA apres Free             = ", n, "  (attendu 11 : l'interpreteur est intact)\n"
if n <> 11 then ok = 0

print "\n"
if ok then
  print "dyc_com : OK - CoGetMalloc, Alloc, GetSize, DidAlloc et Free traversent le thiscall\n"
  else
  print "dyc_com : ECHEC\n"
  end if
