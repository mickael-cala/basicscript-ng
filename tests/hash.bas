' hash.bas - test fonctionnel de l'extension hash
'
' Deux remarques prealables, toutes deux verifiees pendant ce portage :
'
' 1) La distribution de ScriptBasic ne livre PAS de fichier hash.bas. Trois
'    extensions seulement (cgi, nt, dyc) ont leur fichier de declaration, alors
'    que l'en-tete de dyc.bas rappelle qu'ils sont produits par headerer.pl a
'    partir des marqueurs besFUNCTION de interface.c. Pour hash, interface.c ne
'    porte ni marqueur BAS: ni bloc TO_BAS, donc rien n'a jamais ete genere.
'    Les declare ci-dessous sont donc ecrits a la main, d'apres les signatures
'    lues dans interface.c.
'
' 2) gethv() et thishv() renvoyaient une "reference mortelle" via
'    besNEWMORTALREF (basext.h), qui appelle memory_NewMortalRef. Or cette
'    fonction de l'interpreteur est un bouchon VOLONTAIRE : la structure des
'    variables ne permet pas de reference mortelle. Les deux fonctions
'    echouaient donc sur l'erreur 1 (COMMAND_ERROR_MEMORY_LOW), sur toute
'    plateforme. Elles renvoient maintenant une COPIE de la valeur :
'    modifier la copie ne modifie pas la table, il faut rappeler sethv.
'
'    Sont testes : newh, sethv, gethv, thishv, ivhv, delhk, relh et le
'    parcours complet (starth, endh, nexthk, thishk).

declare sub newh   alias "newh"   lib "hash"
declare sub sethv  alias "sethv"  lib "hash"
declare sub gethv  alias "gethv"  lib "hash"
declare sub thishv alias "thishv" lib "hash"
declare sub ivhv   alias "ivhv"   lib "hash"
declare sub delhk  alias "delhk"  lib "hash"
declare sub starth alias "starth" lib "hash"
declare sub endh   alias "endh"   lib "hash"
declare sub nexthk alias "nexthk" lib "hash"
declare sub thishk alias "thishk" lib "hash"
declare sub relh   alias "relh"   lib "hash"

ok = 1

' ---------------------------------------------------------------------------
' La DLL est resolue par modumana.c dans le dossier declare par la cle
' `module` de scriba.conf : c'est le test du chargement dynamique.
' ---------------------------------------------------------------------------
h = newh()
print "handle de table              : ", h, "\n"
if h <= 0 then ok = 0

' --- ecriture et presence, sur les trois types de valeurs ---
call sethv(h, "alma", 42)
call sethv(h, "korte", "szilva")
call sethv(h, "szam", 3.5)

e1 = ivhv(h, "alma")
e2 = ivhv(h, "korte")
e3 = ivhv(h, "szam")
e4 = ivhv(h, "nincsilyen")
print "ivhv(alma)                   : ", e1, "  (attendu 1)\n"
print "ivhv(korte)                  : ", e2, "  (attendu 1)\n"
print "ivhv(szam)                   : ", e3, "  (attendu 1)\n"
print "ivhv(inconnu)                : ", e4, "  (attendu 0)\n"
if e1 <> 1 or e2 <> 1 or e3 <> 1 or e4 <> 0 then ok = 0

' --- lecture par copie, sur les trois types de valeurs ---
v1 = gethv(h, "alma")
v2 = gethv(h, "korte")
v3 = gethv(h, "szam")
print "gethv(alma)                  : ", v1, "  (attendu 42)\n"
print "gethv(korte)                 : ", v2, "  (attendu szilva)\n"
print "gethv(szam)                  : ", v3, "  (attendu 3.5)\n"
if v1 <> 42 or v2 <> "szilva" or v3 <> 3.5 then ok = 0

' La copie est independante : la modifier ne touche pas la table.
v1 = 999
if gethv(h, "alma") <> 42 then ok = 0
if isdefined(gethv(h, "nincsilyen")) then ok = 0

' thishv lit la valeur de l'element courant.
call starth(h)
vt = thishv(h)
if vt <> 42 then ok = 0
call endh(h)
vt = thishv(h)
if vt <> 3.5 then ok = 0

' --- parcours en avant, dans l'ordre d'insertion ---
'
' Motif d'iteration attendu par le module : thishk() rend la cle courante MAIS
' echoue sur HASH_ERROR_NO_CURRENT_ELEMENT si l'iterateur est a NULL, alors que
' nexthk() rend undef (et non une erreur) quand il depasse le dernier element.
' On appelle donc thishk() une seule fois, au debut, puis on boucle sur nexthk().
call starth(h)
k = thishk(h)
k1 = k
k2 = nexthk(h)
print "parcours avant               : ", k1, " / ", k2, "\n"
if k1 <> "alma" then ok = 0
if k2 <> "korte" then ok = 0

' --- mise a jour d'une cle existante : la table ne doit pas doubler la cle ---
call sethv(h, "alma", 43)
if gethv(h, "alma") <> 43 then ok = 0
n = 0
call starth(h)
k = thishk(h)
while k <> undef
  n = n + 1
  k = nexthk(h)
wend
print "cles apres mise a jour       : ", n, "  (attendu 3)\n"
if n <> 3 then ok = 0

' La derniere cle doit rester "szam", la mise a jour ne doit pas reordonner.
call starth(h)
k = thishk(h)
while k <> undef
  k3 = k
  k = nexthk(h)
wend
print "derniere cle apres mise a jour: ", k3, "  (attendu szam)\n"
if k3 <> "szam" then ok = 0

' --- le parcours en arriere part du dernier element ---
call endh(h)
k4 = thishk(h)
print "parcours arriere (derniere)  : ", k4, "  (attendu szam)\n"
if k4 <> "szam" then ok = 0

' --- suppression ---
call delhk(h, "korte")
e5 = ivhv(h, "korte")
print "ivhv(korte) apres suppression: ", e5, "  (attendu 0)\n"
if e5 <> 0 then ok = 0
if isdefined(gethv(h, "korte")) then ok = 0

' --- une table de 200 cles : hachage, collisions, listes chainees ---
h2 = newh()
for i = 1 to 200
  call sethv(h2, "cle_" & i, i)
next i
bon = 1
for i = 1 to 200
  if ivhv(h2, "cle_" & i) <> 1 then bon = 0
next i
print "200 cles retrouvees          : ", bon, "  (attendu 1)\n"
if bon <> 1 then ok = 0

' Nombre d'elements parcourus : doit valoir exactement 200.
n2 = 0
call starth(h2)
kk = thishk(h2)
while kk <> undef
  n2 = n2 + 1
  kk = nexthk(h2)
wend
print "200 elements parcourus       : ", n2, "  (attendu 200)\n"
if n2 <> 200 then ok = 0

call relh(h)
call relh(h2)

if ok then
  print "\nhash : OK\n"
else
  print "\nhash : ECHEC\n"
end if
