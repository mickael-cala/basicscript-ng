' md5.bas - test fonctionnel de l'extension t
'
' Le module t rassemble des outils : md5fun, serialize/unserialize,
' xmlserialize, savestring/loadstring. Le noyau de ScriptBasic possede sa
' propre implementation MD5 (src/md5.c, utilisee par le format binaire mis en
' cache) ; l'extension en expose une seconde. Choisir md5fun comme test a
' l'avantage d'etre parfaitement deterministe : sur une donnee fixe, le resume
' doit toujours etre identique, de longueur 16 (MD5 rend 16 octets bruts, pas
' une chaine hexadecimale).
'
' La distribution ne livre pas de fichier t.bas : seules cgi, nt et dyc en ont
' un. La declaration est donc ecrite a la main.

declare sub md5fun alias "md5fun" lib "t"

ok = 1
print "=== extension t (outils : md5, serialisation) ===\n"

' --- longueur du resume : 16 octets, toujours ---
d0 = md5fun("")
print "longueur de md5(\"\")        : ", len(d0), "  (attendu 16)\n"
if len(d0) <> 16 then ok = 0

' --- determinisme : deux appels identiques donnent le meme resume ---
d1 = md5fun("ScriptBasic")
d2 = md5fun("ScriptBasic")
print "determinisme               : ", (d1 = d2), "  (attendu 1)\n"
if d1 <> d2 then ok = 0

' --- un seul caractere change -> resume different (effet d'avalanche) ---
d3 = md5fun("ScriptBasid")
print "deux entrees voisines      : ", (d1 <> d3), "  (attendu 1)\n"
if d1 = d3 then ok = 0

' --- undef et chaine vide ne doivent pas se confondre avec un plantage ---
du = md5fun(undef)
print "md5(undef) rendu           : [", du, "]\n"

' --- donnees plus longues : 1 000 000 de caracteres, comme au NIST ---
gros = string(1000, "a")
for i = 1 to 999
  gros = gros & string(1000, "a")
next i
print "taille de l'entree         : ", len(gros), "\n"
d4 = md5fun(gros)
print "longueur du resume         : ", len(d4), "  (attendu 16)\n"
if len(d4) <> 16 then ok = 0
if d4 = d1 then ok = 0

if ok then
  print "\nt : OK\n"
else
  print "\nt : ECHEC\n"
end if
