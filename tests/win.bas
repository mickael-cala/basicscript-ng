' win.bas - test fonctionnel de l'extension win (ex-nt)
'
' Le module s'appelait `nt` dans la distribution d'origine ; il est renomme
' `win` dans ce portage, comme dans la vague NG, parce que « NT » ne designe
' plus la plateforme. Le prefixe des fonctions exportees devient `win_`
' (`win_startservice`, `win_hardlink`…), et le fichier de declaration
' s'appelle `win.bas`.
'
' Le test ne touche a rien : il lit une valeur du registre qui existe sur tout
' Windows, puis enumere les processus dans un tableau. Pas de MsgBox (elle
' attendrait un clic), pas de ShutDown, pas de service, pas de lien physique :
' ces trois-la demandent des privileges ou une intervention. Le lien physique,
' qui exerce les corrections `byte` du portage, a son propre test manuel :
' tests/win_hardlink.bas.
'
' Ce qui est verifie ici : le chargement du .dll, versmodu/bootmodu, la
' conversion des arguments, RegRead (une chaine) et ListProcesses (un tableau
' de tableaux construit par le module).

import win.bas

ok = 1
print "=== extension win (registre et processus Windows) ===\n"

' ---------------------------------------------------------------------------
' 1) Lecture du registre : ProductName existe sur toutes les editions.
' ---------------------------------------------------------------------------
bs = chr(92)
cle = "HKLM" & bs & "SOFTWARE" & bs & "Microsoft" & bs & "Windows NT" & bs & "CurrentVersion" & bs & "ProductName"
produit = win::RegRead(cle)
print "produit Windows      : [", produit, "]\n"
if produit = undef then
  print "  RegRead a rendu undef : valeur absente ou illisible\n"
  ok = 0
else
  if len(produit) = 0 then ok = 0
end if

' ---------------------------------------------------------------------------
' 2) Liste des processus : le module remplit un tableau, une ligne par
'    processus. Sur une machine qui fait tourner ce test, il y en a bien plus
'    que cinq (System, services, shell, l'interpreteur lui-meme).
' ---------------------------------------------------------------------------
win::ListProcesses ps
n = ubound(ps) + 1
print "processus enumeres   : ", n, "  (au moins 5 attendus)\n"
if n < 5 then ok = 0

' Chaque element est un tableau de 9 champs (0 a 8) ; ScriptBasic indexe un
' tableau de tableaux avec PS[i,j] (crochets et virgule), pas par deux
' parentheses. On verifie qu'un des chemins d'executable contient « .exe » :
' c'est le champ 8.
trouve = 0
for i = 0 to n - 1
  champ = ps[i,8]
  if champ <> undef then
    if instr(lcase(champ), ".exe") > 0 then trouve = 1
  end if
next i
print "chemin .exe trouve    : ", trouve, "  (attendu 1)\n"
if trouve <> 1 then ok = 0

if ok then
  print "\nwin : OK\n"
else
  print "\nwin : ECHEC\n"
end if
