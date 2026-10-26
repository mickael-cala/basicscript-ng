' dyc_abi.bas - audit des AUTRES contrats d'ABI du moteur d'appel dyc
'
' L'audit d'alignement de pile (section 12 du portage, test/dyc_probe_align.c)
' a montre ce qu'une garantie d'ABI non tenue coute : une violation d'acces sur
' un appel sur deux. Restaient quatre contrats dont le non-respect ne se voit
' pas au premier essai :
'
'   DF     la direction de `rep movsl` : a 1, la copie des arguments se fait
'          vers les adresses DECROISSANTES. L'ABI exige DF = 0 a l'entree de
'          toute fonction, et l'appelant de DynaCall a le droit de le retrouver
'          tel qu'il l'a laisse. Le 32 bits n'avait AUCUN `cld` (le x64 en avait
'          un avant sa copie) et ni l'un ni l'autre ne rendait DF a l'appelant.
'   x87    le moteur lit un retour flottant par fstps/fstpl, donc DEPILE : une
'          serie d'appels a retour flottant correctement declare doit laisser la
'          profondeur du registre ST inchangee. Quand le format declare un
'          retour ENTIER pour une fonction qui rend un double, la valeur reste
'          empilee — le moteur ne peut pas deviner le type de retour, mais la
'          sonde MESURE ce que cela coute, et rend le compte a zero ensuite.
'   MXCSR  le moteur ne le lit ni ne l'ecrit : l'ABI en fait un registre que
'          l'APPELé doit preserver, pas l'appelant. La sonde le verifie : sa
'          valeur ne doit pas bouger d'un appel a l'autre.
'   regs   EBX, ESI, EDI, EBP (et, en x64, RBX, RBP, RSI, RDI, R12-R15)
'          appartiennent a l'APPELANT : DynaCall est une fonction comme une
'          autre, et meme une exportation (test/bench_dyc.c l'appelle par
'          GetProcAddress). Le moteur les utilise avant le call et declare ces
'          registres « clobbered » a TCC : tout depend de ce que TCC fait de
'          cette liste, et seule une sonde qui APPELLE DynaCall avec des
'          sentinelles peut trancher.
'
' LES SONDES. Elles sont dans le DLL dyc lui-meme, sous PROBES_LIVE (voir
' test/dyc_probe_abi.c et test/dyc_probe_abi64.c, et la note « OPTION DE
' COMPILATION PROBES_LIVE » de src/extensions/dyc/interface.c). Sur le DLL
' livre — celui que la campagne essaie en phase 2 — les sondes n'existent pas :
' les quatorze appels rendent alors l'erreur propre 17, la section s'efface
' d'elle-meme et le test reste OK, exactement comme les sections du DLL sonde
' de test/dyc.bas. C'est la phase 2bis qui reconstruit le DLL avec -DPROBES_LIVE
' et rejoue CE fichier : c'est la que le verdict est prononce.
'
' LES INSTRUMENTS SONT CALIBRES. Une sonde qui ne sait pas dire « non » ne
' prouve rien : le script test/dyc_fixture.bas rejoue les memes mesures sur les
' moteurs reconstruits par la campagne SANS les correctifs (-DDYC_SANS_CLD,
' -DDYC_ESP_NONALIGN) et doit y lire la signature du defaut — c'est une phase de
' la campagne, pas une manoeuvre manuelle.
'
' Les adresses des sondes internes (celle de DynaCall, celle d'une cible inerte,
' celles des cibles qui posent et lisent DF) sont demandees au C par
' dyc_probe_addr(which) : une reference a un symbole C depuis l'asm devrait
' connaitre sa decoration exacte (`_DynaCall@24` — le nom meme de l'exportation),
' alors qu'une adresse arrivee par la pile ne depend d'aucune convention de nom.
' En 32 bits elle passe en 'L' (un entier) ; en x64 elle fait 8 octets et passe
' donc par 'v' (retour en chaine binaire) et '8' (argument pointe).
'
' NB de langue : ce BASIC refuse les commentaires en fin de ligne — toutes les
' explications de ce fichier sont donc sur leurs propres lignes.

import dyc.bas

ok = 1

' ------------------------------------------------------------------ le monde --
' Taille de PROCESS_BASIC_INFORMATION : 24 octets en 32 bits, 48 en 64. Le
' noyau 32 bits refuse toute autre taille, le 64 bits echoue la demande de 24 :
' l'echec est donc le discriminant (meme mesure que test/dyc.bas).
pbi$ = string(64, chr$(0))
rl$ = string(8, chr$(0))
arch64 = 0
rw = dyc::dyc("cs,p,ntdll.dll,NtQueryInformationProcess,LPLLL", -1, 0, pbi$, 24, rl$)
if rw <> 0 then arch64 = 1

' noms et formats selon le monde : le retour d'adresse est 'i' en 32 bits (elle
' tient sur 4 octets) et 'v' en x64 ; l'argument d'adresse est 'L' puis '8'.
dll$ = "modules/dyc.dll"
fmt$ = "ms,i," & dll$ & ","
fmv$ = fmt$
larg$ = "L"
fnv$ = "dyc_probe_nvregs"
fde$ = "dyc_probe_dfentry"
fdx$ = "dyc_probe_dfexit"
fuite = 3
if arch64 then
  dll$ = "modules64/dyc64.dll"
  fmt$ = "cs,i," & dll$ & ","
  fmv$ = "cs,v," & dll$ & ","
  larg$ = "8"
  fnv$ = "dyc_probe_nvregs64"
  fde$ = "dyc_probe_dfentry64"
  fdx$ = "dyc_probe_dfexit64"
  fuite = 0
  end if
fad$ = fmv$ & "dyc_probe_addr,L"
' fuite = 3 : trois valeurs x87 laissees par les appels fautifs de la section
' x87. En x64 les flottants voyagent dans XMM, la pile x87 reste vide : rien a
' rendre.
'
' Les quatre adresses sont initialisees a une chaine de huit zeros en x64 : le
' monde SANS sondes les laisse ainsi, et les appels qui les utilisent doivent
' alors echouer sur la fonction absente (erreur 17) et non sur l'argument (le
' type '8' exige huit octets, un zero nu donnerait l'erreur propre 8 — juste,
' mais une AUTRE erreur que celle que la section veut compter).
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

' ------------------------------------------------------- les quatorze appels -
' Chacun est protege : dans le monde sans sondes, tous rendent 17 et le
' gestionnaire deroute vers le label place APRES la ligne fautive (reprendre la
' ligne elle-meme bouclerait sur son erreur).
rnv = 0
rdfe = 0
rdfx = 0
rsw0 = 0
rsw1 = 0
rsw2 = 0
rsw3 = 0
rpop = 0
rmx0 = 0
rmx1 = 0
errabi = 0
i = 0
etape = 0
on error goto capabi

' etape 1 : adresse de DynaCall
etape = 1
ad0 = dyc::dyc(fad$, 0)
ab1:
' etape 2 : adresse de la cible inerte
etape = 2
adi = dyc::dyc(fad$, 1)
ab2:
' etape 3 : adresse de la cible qui pose DF
etape = 3
ads = dyc::dyc(fad$, 2)
ab3:
' etape 4 : adresse de la cible qui lit DF
etape = 4
adg = dyc::dyc(fad$, 3)
ab4:
' etape 5 : registres non volatils — sentinelles, appel a DynaCall, comparaison
etape = 5
rnv = dyc::dyc(fmt$ & fnv$ & "," & larg$ & larg$, ad0, adi)
ab5:
' etape 6 : DF vu par l'appele (le `cld` d'AVANT la copie des arguments)
etape = 6
rdfe = dyc::dyc(fmt$ & fde$ & "," & larg$ & larg$, ad0, adg)
ab6:
' etape 7 : DF rendu a l'appelant (le `cld` d'APRES le retour de l'appele)
etape = 7
rdfx = dyc::dyc(fmt$ & fdx$ & "," & larg$ & larg$, ad0, ads)
ab7:
' etape 8 : profondeur x87 au depart
etape = 8
rsw0 = dyc::dyc(fmt$ & "dyc_probe_fpusw,")
ab8:
' etape 9 : vingt appels a retour flottant CORRECTEMENT declare ('d')
etape = 9
for i = 1 to 20
  x = dyc::dyc("c8,d,msvcrt.dll,cos,D", 0.0)
next i
rsw1 = dyc::dyc(fmt$ & "dyc_probe_fpusw,")
ab9:
' etape 10 : trois appels dont le format declare un retour ENTIER pour un double
etape = 10
for i = 1 to 3
  x = dyc::dyc("cs,i,msvcrt.dll,cos,D", 0.0)
next i
rsw2 = dyc::dyc(fmt$ & "dyc_probe_fpusw,")
ab10:
' etape 11 : on rend le compte a zero (la mesure est close)
etape = 11
rpop = dyc::dyc(fmt$ & "dyc_probe_x87pop,L", fuite)
ab11:
' etape 12 : temoin apres reparation
etape = 12
rsw3 = dyc::dyc(fmt$ & "dyc_probe_fpusw,")
ab12:
' etape 13 : MXCSR avant
etape = 13
rmx0 = dyc::dyc(fmt$ & "dyc_probe_mxcsr,")
ab13:
' etape 14 : MXCSR apres dix appels flottants de plus
etape = 14
for i = 1 to 10
  x = dyc::dyc("c8,d,msvcrt.dll,cos,D", 0.0)
next i
rmx1 = dyc::dyc(fmt$ & "dyc_probe_mxcsr,")
ab14:
goto abibilan

capabi:
on error goto capabi
errabi = errabi + 1
if error <> 17 then
  print "dyc_abi : erreur inattendue ", error, " a l'etape ", etape, "\n"
  ok = 0
  end if
if etape = 1 then goto ab1
if etape = 2 then goto ab2
if etape = 3 then goto ab3
if etape = 4 then goto ab4
if etape = 5 then goto ab5
if etape = 6 then goto ab6
if etape = 7 then goto ab7
if etape = 8 then goto ab8
if etape = 9 then goto ab9
if etape = 10 then goto ab10
if etape = 11 then goto ab11
if etape = 12 then goto ab12
if etape = 13 then goto ab13
if etape = 14 then goto ab14

' ------------------------------------------------------------------- verdict -
abibilan:
' TOP = bits 11-13 du mot d'etat x87 ; (t1 - 3) mod 8 apres trois valeurs
' empilees par les appels fautifs.
t0 = (rsw0 \ 2048) and 7
t1 = (rsw1 \ 2048) and 7
t2 = (rsw2 \ 2048) and 7
t3 = (rsw3 \ 2048) and 7
tf = (t1 - fuite) and 7
conforme = 0

if errabi = 0 then
  conforme = 1
  if rdfe <> 0 then conforme = -1
  if rdfx <> 0 then conforme = -1
  if rnv <> 0 then conforme = -1
  if t0 <> 0 then conforme = -1
  if t1 <> t0 then conforme = -1
  if t2 <> tf then conforme = -1
  if rpop <> fuite or t3 <> t0 then conforme = -1
  if rmx0 <> rmx1 then conforme = -1
  end if

print "abi : DF vu par l'appele            = ", rdfe, "  (0 attendu : cld avant la copie)\n"
print "abi : DF rendu a l'appelant         = ", rdfx, "  (0 attendu : cld avant le retour)\n"
print "abi : registres non volatils perdus = ", rnv, "  (0 attendu ; 1=EBX/RBX 2=ESI/RBP 4=EDI/RSI 8=EBP/RDI, x64 : 16=R12 32=R13 64=R14 128=R15)\n"
print "abi : pile x87 au depart            = TOP ", t0, "  (0 attendu : rien laisse par les appels precedents)\n"
print "abi : pile x87, 20 retours 'd'      = TOP ", t1, "  (inchangee : fstpl depile exactement ce que l'appele empile)\n"
if arch64 then
  print "abi : pile x87, 3 retours 'i' (x64) = TOP ", t2, "  (inchangee : les flottants passent par XMM)\n"
else
  print "abi : pile x87, 3 retours 'i'       = TOP ", t2, "  (attendu ", tf, " : le format declare entier, la valeur reste empilee)\n"
  end if
print "abi : x87pop(", fuite, ")                    = ", rpop, "  puis TOP ", t3, "  (le compte est rendu a ", t0, ")\n"
print "abi : MXCSR avant / apres           = ", rmx0, " / ", rmx1, "  (egaux attendus : le moteur n'y touche pas)\n"
print "abi : erreurs 17 sur les 14 appels  = ", errabi, "  (0 = sondes presentes)\n"

if errabi = 0 and conforme = -1 then
  print "dyc_abi : contrat d'ABI non tenu\n"
  ok = 0
  end if
if errabi <> 0 and errabi <> 14 then
  ' Un compte d'erreurs intermediaire : les sondes sont la pour certaines et pas
  ' pour d'autres, ou un appel a echoue pour une autre raison. Rien de tel ne
  ' doit passer pour un audit saute.
  print "dyc_abi : erreurs partielles (", errabi, "/ 14) -- etat incoherent\n"
  ok = 0
  end if
if errabi <> 0 then
  ' Les quatorze appels ont rendu 17 : le DLL essaie est celui du depot, sans
  ' les sondes. Les valeurs lues ci-dessus ne veulent alors rien dire — le
  ' gestionnaire d'erreur les a laissees a leur initialisation — et le test le
  ' DIT au lieu de s'effacer en silence : c'est la phase 2bis de la campagne,
  ' qui reconstitue le DLL sonde, qui rejoue ce fichier et prononce le verdict.
  print "dyc_abi : sondes absentes du DLL livre -- audit saute (phase 2bis)\n"
  end if

if ok = 0 then
  print "\ndyc_abi : ECHEC\n"
elseif errabi = 0 then
  print "\ndyc_abi : OK - DF, pile x87, MXCSR et registres non volatils sous controle\n"
else
  print "\ndyc_abi : OK (sondes absentes : rien n'a ete juge ici)\n"
  end if
