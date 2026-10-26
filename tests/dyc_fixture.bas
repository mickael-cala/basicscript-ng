' dyc_fixture.bas - LE DETECTEUR DE DEFAUTS du moteur d'appel dyc
'
' Ce fichier ne juge rien du depot, et surtout : il ne sait PAS quel defaut est
' arme. C'est un instrument de MESURE. Il interroge le moteur sur huit points —
' ceux ou le portage a corrige une panne — et imprime pour chacun la valeur lue,
' la valeur du contrat, et le mot « sain » ou « FAUTIF ». C'est la phase 2ter de
' la campagne (test/run_all.py) qui confronte ces classifications a sa banque
' des defauts : chaque panne historique du portage doit y retrouver sa
' signature, et le moteur livre (aucune macro de fixture) doit toutes les
' montrer saines.
'
' Le meme script sert donc de CONTROLE dans le monde sain (phase 2bis, sur le
' DLL construit avec PROBES_LIVE seul). Si l'instrument se trompait lui-meme —
' un seuil faux, une sonde mal appelee —, le controle le dirait avant que les
' mondes defaillants ne soient accuses : c'est ce qui distingue un detecteur
' d'un accuse.
'
' LES HUIT POINTS, et la panne que chacun sait reconnaitre :
'
'   1. df vu par l'appele    - le `cld` d'AVANT la copie des arguments (moteur
'                              32 bits d'origine : `rep movsl` avec DF a 1
'                              ecrit SOUS le bloc d'arguments) ;
'   2. df rendu a l'appelant - le `cld` d'APRES le retour de l'appele, qui
'                              rend DF = 0 a l'appelant de DynaCall ;
'   3. thiscall              - *this + dw1 + dw2 = 130 : prouve a la fois que
'                              ECX/RCX portait le this et que les arguments de
'                              pile sont a leur place (la premiere version
'                              detournait dw1 vers EDX : fastcall-avec-this) ;
'   4. toupper('b') par 'c'  - le fallthrough 'c' -> 's' du parseur d'arguments
'                              (l'argument devenait (short)GetLongValue("b") =
'                              0, donc toupper rendait 0 et non 66) ;
'   5. abs('b') par '1'      - les octets hauts d'un argument d'un octet laisses
'                              a autre chose que zero (mise a zero du tableau
'                              d'arguments remplacee par un motif 0x5A : le
'                              distributeur du noyau rendait des blocs NULS sur
'                              cette machine, un defaut qui ne se manifeste que
'                              par chance n'est pas un defaut mesurable ; avec
'                              le motif, la valeur attendue cote fautif est
'                              0x5A5A5A62, soit 1515872354) ;
'   6. GetTickCount sans argument - la lacune du format a ZERO argument (la
'                              boucle de parse s'arretait avant l'etat 4 et
'                              l'appel sortait sur l'erreur 17) ;
'   7. registres non volatils - contrat d'ABI jamais enfreint : il sert de
'                              temoin dans chaque monde (une fixture qui
'                              casserait aussi cela doit le DIRE) ;
'   8. movaps strict (32 bits)- l'arrondi a 16 de la base du bloc d'appel :
'                              l'appele fait un VRAI movaps vers un emplacement
'                              aligne(16) et meurt (0xC0000005) si le moteur
'                              ment. Appel place EN DERNIER : c'est la seule
'                              mesure possible d'un processus qui meurt.
'
' Une valeur NEGATIVE dit l'echec de l'appel : -17 par exemple est l'erreur
' BASIC 17 (sondes absentes du DLL), la ou une ligne attendue porte une mesure.
' Aucune ligne ne dit quel defaut la campagne a pose — c'est le principe.
'
' Le script sert les deux architectures : les noms de sondes, les largeurs
' d'adresse et la taille de PROCESS_BASIC_INFORMATION changent, le reste non.
' Ce BASIC refuse les commentaires en fin de ligne : les explications sont sur
' leurs propres lignes.

import dyc.bas

' ------------------------------------------------ le monde : 32 ou 64 bits ?
' La taille de PROCESS_BASIC_INFORMATION le dit : 24 octets en 32 bits, 48 en 64.
pbi$ = string(64, chr$(0))
rl$ = string(8, chr$(0))
arch64 = 0
rw = dyc::dyc("cs,p,ntdll.dll,NtQueryInformationProcess,LPLLL", -1, 0, pbi$, 24, rl$)
if rw <> 0 then arch64 = 1

' -------------------------------------- les noms qui suivent l'architecture
dll$ = "modules/dyc.dll"
fmt$ = "ms,i," & dll$ & ","
fmv$ = fmt$
larg$ = "L"
fnthis$ = "dyc_probe_this"
fnv$ = "dyc_probe_nvregs"
fde$ = "dyc_probe_dfentry"
fdx$ = "dyc_probe_dfexit"
if arch64 then
  dll$ = "modules64/dyc64.dll"
  fmt$ = "cs,i," & dll$ & ","
  fmv$ = "cs,v," & dll$ & ","
  larg$ = "8"
  fnthis$ = "dyc_probe_this64"
  fnv$ = "dyc_probe_nvregs64"
  fde$ = "dyc_probe_dfentry64"
  fdx$ = "dyc_probe_dfexit64"
  end if
fad$ = fmv$ & "dyc_probe_addr,L"
fth$ = "t,i," & dll$ & "," & fnthis$ & ",ppp"
ftk$ = "ms,i,KERNEL32.DLL,GetTickCount,"
ftu$ = "cs,i,msvcrt.dll,toupper,c"
fab$ = "cs,i,msvcrt.dll,abs,1"
' La forme a DEUX arguments est celle qui tombait avant le correctif (la
' valeur livree dependait de la forme de l'appel — mesure de logs/sonde_esp.bas).
fsa$ = fmt$ & "dyc_probe_align_strict,LL"
' La sonde thiscall lit (%ecx)/(%rcx) : le this est une chaine de huit octets
' dont le premier vaut 7, passee en 'p' (le moteur transmet son ADRESSE).
sthis$ = chr$(7) & chr$(0) & chr$(0) & chr$(0) & chr$(0) & chr$(0) & chr$(0) & chr$(0)

' Les quatre adresses de sondes. En x64 ce sont des chaines de huit octets :
' le type '8' exige huit octets, et une valeur nulle ferait lever l'erreur 8 au
' lieu de l'erreur 17 que le monde SANS sondes doit produire (l'erreur compte,
' elle est imprimee).
ad0 = string(8, chr$(0))
adi = ad0
ads = ad0
adg = ad0
if arch64 = 0 then
  ad0 = 0
  adi = 0
  ads = 0
  adg = 0
  end if

etape = 0
nfautif = 0
ntot = 0
errprobe = 0
errtot = 0
r_dfe = -1
r_dfx = -1
r_this = -1
r_tu = -1
r_ab = -1
r_tk = -1
r_nv = -1
r_sa = -1
on error goto capfix

' ------------------------ 0. les quatre adresses des sondes embarquees -----
' Le monde sans sondes (DLL livre, ou module construit sans PROBES_LIVE)
' echoue ici : les quatre etapes rendent l'erreur propre 17 et le bilan le
' dira. C'est aussi ce qui empeche les lignes suivantes de faire semblant.
etape = 1
ad0 = dyc::dyc(fad$, 0)
ap_ad0:
etape = 2
adi = dyc::dyc(fad$, 1)
ap_adi:
etape = 3
ads = dyc::dyc(fad$, 2)
ap_ads:
etape = 4
adg = dyc::dyc(fad$, 3)
ap_adg:

' -------------------- 1. DF vu par l'appele (le cld AVANT la copie) --------
etape = 5
r_dfe = dyc::dyc(fmt$ & fde$ & "," & larg$ & larg$, ad0, adg)
ap_dfe:
v$ = "FAUTIF"
if r_dfe = 0 then v$ = "sain"
ntot = ntot + 1
if v$ = "FAUTIF" then nfautif = nfautif + 1
print "fixture : df vu par l'appele          = ", r_dfe, "   ", v$, "\n"

' -------------------- 2. DF rendu a l'appelant (le cld du retour) ----------
etape = 6
r_dfx = dyc::dyc(fmt$ & fdx$ & "," & larg$ & larg$, ad0, ads)
ap_dfx:
v$ = "FAUTIF"
if r_dfx = 0 then v$ = "sain"
ntot = ntot + 1
if v$ = "FAUTIF" then nfautif = nfautif + 1
print "fixture : df rendu a l'appelant       = ", r_dfx, "   ", v$, "\n"

' -------------------- 3. thiscall : *this + dw1 + dw2 = 130 ---------------
etape = 7
r_this = dyc::dyc(fth$, sthis$, 100, 23)
ap_this:
v$ = "FAUTIF"
if r_this = 130 then v$ = "sain"
ntot = ntot + 1
if v$ = "FAUTIF" then nfautif = nfautif + 1
print "fixture : thiscall *this+dw1+dw2      = ", r_this, "   ", v$, "\n"

' -------------------- 4. toupper('b') par 'c' : 66 ------------------------
etape = 8
r_tu = dyc::dyc(ftu$, "b")
ap_tu:
v$ = "FAUTIF"
if r_tu = 66 then v$ = "sain"
ntot = ntot + 1
if v$ = "FAUTIF" then nfautif = nfautif + 1
print "fixture : toupper('b') par 'c'        = ", r_tu, "   ", v$, "\n"

' -------------------- 5. abs('b') par '1' : 98 ---------------------------
' L'argument '1' n'ecrit QU'UN octet de dwArg : les trois autres portent le
' motif 0x5A quand la mise a zero a ete remplacee (le monde de la banque).
etape = 9
r_ab = dyc::dyc(fab$, "b")
ap_ab:
v$ = "FAUTIF"
if r_ab = 98 then v$ = "sain"
ntot = ntot + 1
if v$ = "FAUTIF" then nfautif = nfautif + 1
print "fixture : abs('b') par '1'            = ", r_ab, "   ", v$, "\n"

' -------------------- 6. GetTickCount sans argument : > 0 ----------------
etape = 10
r_tk = dyc::dyc(ftk$)
ap_tk:
v$ = "FAUTIF"
if r_tk > 0 then v$ = "sain"
ntot = ntot + 1
if v$ = "FAUTIF" then nfautif = nfautif + 1
print "fixture : GetTickCount sans argument  = ", r_tk, "   ", v$, "\n"

' -------------------- 7. registres non volatils : 0 ----------------------
etape = 11
r_nv = dyc::dyc(fmt$ & fnv$ & "," & larg$ & larg$, ad0, adi)
ap_nv:
v$ = "FAUTIF"
if r_nv = 0 then v$ = "sain"
ntot = ntot + 1
if v$ = "FAUTIF" then nfautif = nfautif + 1
print "fixture : registres non volatils      = ", r_nv, "   ", v$, "\n"

' -------------------- 8. movaps strict : 1 (32 bits seulement) -----------
' EN DERNIER : le monde sans arrondi meurt dans la sonde, donc rien de ce qui
' suit ces lignes ne serait ecrit. L'appel precede l'impression.
if arch64 then
  print "fixture : movaps strict                = sans sonde en x64 (temoin hors module)\n"
else
  etape = 12
  r_sa = dyc::dyc(fsa$, 1, 2)
ap_sa:
  v$ = "FAUTIF"
  if r_sa = 1 then v$ = "sain"
  ntot = ntot + 1
  if v$ = "FAUTIF" then nfautif = nfautif + 1
  print "fixture : movaps strict               = ", r_sa, "   ", v$, "\n"
  end if

goto bilan

' ------------------------------------ le gestionnaire : une valeur negative
' Il se REARME a chaque passage (sinon le deuxieme appel fautif ferait
' tomber le programme) et deroute vers le label place APRES la ligne fautive,
' sans quoi BASIC reprendrait la ligne elle-meme.
capfix:
on error goto capfix
e = error
errtot = errtot + 1
if etape <= 4 then errprobe = errprobe + 1
if etape = 1 then
  ad0 = 0 - e
  goto ap_ad0
  end if
if etape = 2 then
  adi = 0 - e
  goto ap_adi
  end if
if etape = 3 then
  ads = 0 - e
  goto ap_ads
  end if
if etape = 4 then
  adg = 0 - e
  goto ap_adg
  end if
if etape = 5 then
  r_dfe = 0 - e
  goto ap_dfe
  end if
if etape = 6 then
  r_dfx = 0 - e
  goto ap_dfx
  end if
if etape = 7 then
  r_this = 0 - e
  goto ap_this
  end if
if etape = 8 then
  r_tu = 0 - e
  goto ap_tu
  end if
if etape = 9 then
  r_ab = 0 - e
  goto ap_ab
  end if
if etape = 10 then
  r_tk = 0 - e
  goto ap_tk
  end if
if etape = 11 then
  r_nv = 0 - e
  goto ap_nv
  end if
r_sa = 0 - e
goto ap_sa

' ------------------------------------------------------------------- bilan
bilan:
print "\n"
if errprobe <> 0 then
  print "fixture : sondes absentes du DLL teste (construit sans PROBES_LIVE)\n"
  print "fixture : aucune des ", ntot, " mesures ci-dessus ne vaut un verdict\n"
  print "\ndyc_fixture : ECHEC - ce DLL n'embarque pas les sondes\n"
' Le compte des appels qui N'ONT PAS ABOUTI est une mesure a part entiere :
' les formats de ce script sont valides, donc une erreur inattendue est un
' defaut du moteur ou du parseur, et la campagne exige ce compte a zero dans
' tous les mondes ou aucune signature ne l'annonce (elle le declare fautif pour
' la fixture du format a zero argument et pour celle du parseur, ou l'erreur EST
' le symptome mesure).
else
  print "fixture : signatures FAUTIF mesurees  = ", nfautif, " / ", ntot, "\n"
  v$ = "FAUTIF"
  if errtot = 0 then v$ = "sain"
  print "fixture : erreurs d'appel             = ", errtot, "   ", v$, "\n"
  print "\ndyc_fixture : OK\n"
  end if
