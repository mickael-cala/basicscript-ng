' trial.bas - test fonctionnel de l'extension trial
'
' `trial` n'est pas un module de bibliotheque : c'est le module d'essai livre
' avec ScriptBasic, celui qui sert a eprouver l'ABI (besFUNCTION, besCOMMAND,
' besMODULEPOINTER, besCONFIG...). Il n'a donc pas d'usage en production, mais
' il est le seul a exercer certains mecanismes que les autres modules ne
' touchent pas : un compteur garde dans le bloc interne du module d'un appel a
' l'autre, une commande paresseuse (iff n'evalue que la branche choisie), et
' l'acces a la configuration depuis le module (module / dll / include).
'
' La declaration de src/extensions/trial/trial.bas n'est pas utilisable : elle
' porte des chemins absolus D:\MyProjects\sb\Debug\ext_trial.dll, herites de la
' machine de developpement d'origine. On declare donc ici, comme le fait
' tests/upstream/test_ext_trial.bas.
'
' Ce que le module ecrit lui-meme sur la sortie standard (version negociee,
' demarrage de bootmodu, valeurs de configuration) n'est pas verifie par le
' test : il depend du texte du module. Seuls les valeurs de retour le sont.

declare sub     trial alias "trial"  lib "trial"
declare command iff   alias "iff"    lib "trial"
declare sub     pprint alias "pprint" lib "trial"

ok = 1
print "=== extension trial (module d'essai de l'ABI) ===\n"

' --- compteur du bloc interne : 1 au premier appel, 2 au second -------------
v1 = trial()
v2 = trial()
print "compteur du module (1)     : ", v1, "  (attendu 1)\n"
if v1 <> 1 then ok = 0
print "compteur du module (2)     : ", v2, "  (attendu 2)\n"
if v2 <> 2 then ok = 0

' --- iff : commande paresseuse, trois arguments ----------------------------
' iff(condition, alors, sinon) n'evalue que la branche retenue. Le troisieme
' argument est du texte, le deuxieme aussi : le resultat doit etre une chaine.
a1 = iff(1, "oui", "non")
a2 = iff(0, "oui", "non")
print "iff(1,oui,non)             : [", a1, "]  (attendu [oui])\n"
if a1 <> "oui" then ok = 0
print "iff(0,oui,non)             : [", a2, "]  (attendu [non])\n"
if a2 <> "non" then ok = 0

' L'exemple du programme de test d'origine : iff(13, 11, 4) rend 11.
a3 = iff(13, 11, 4)
print "iff(13,11,4)               : ", a3, "  (attendu 11)\n"
if a3 <> 11 then ok = 0

' Remarque : iff(0, "seul"), sans troisieme argument, fait echouer
' l'interpreteur (error &H58) avant meme de rendre undef. Ce n'est pas le
' sujet de ce test, qui veut prouver que le module se charge et calcule ;
' la forme a trois arguments est celle du programme d'origine.

' --- pprint : la fonction d'affichage du module ----------------------------
' Elle imprime le nombre d'arguments et, pour chacun, son type et sa valeur.
' On ne peut pas verifier ce qu'elle ecrit depuis le BASIC ; l'appeler prouve
' au moins que la conversion d'arguments et le parcours de besARGUMENT
' fonctionnent sur cette plateforme.
print "pprint(texte, 42)          :\n"
call pprint("texte", 42)

if ok then
  print "\ntrial : OK\n"
else
  print "\ntrial : ECHEC\n"
end if
