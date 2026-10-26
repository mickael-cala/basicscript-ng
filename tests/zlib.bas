' zlib.bas - test fonctionnel de l'extension zlib
'
' zlib est la seule extension a dependance externe que ce portage construise,
' et la raison en est un choix assume : la bibliotheque elle-meme est
' VENDORISEE dans src/modules/zlib/ (zlib 1.3.1, quinze .c et onze .h, licence
' conservee -- docs/PATCHES.md B.10). La cible rsp/ext_zlib.rsp compile tout
' cela dans modules/zlib.dll, sans rien demander a la machine : c'est la meme
' reponse que pour `re`, dont le moteur de Henry Spencer vit dans
' src/modules/re/.
'
' Le fichier de declaration zlib.bas est engendre par sbtool headers depuis les
' marqueurs BAS: / =section / besFUNCTION de interface.c, puis installe dans
' modules/include/ par le COPY de la cible -- comme pour les dix autres modules
' du portage.
'
' Les quatre fonctions sont exercees : Compress et UnCompress en memoire (dont
' une chaine BINAIRE, avec un zero et un octet 0xFF, puisque les chaines de
' ScriptBasic sont binaires) et gzip / gunzip sur un fichier temporaire, dont le
' contenu est relu apres l'aller-retour.

import zlib.bas

ok = 1
print "=== extension zlib (compression) ===\n"

' ---------------------------------------------------------------------------
' 1) Compress / UnCompress en memoire
' ---------------------------------------------------------------------------
texte = "ScriptBasic compresse cette phrase avec zlib. ScriptBasic compresse"
texte = texte & " cette phrase avec zlib. ScriptBasic compresse cette phrase."
c1 = zlib::Compress(texte)
print "longueur du texte             : ", len(texte), "\n"
print "longueur compressee           : ", len(c1), "  (inferieure au texte)\n"
if len(c1) >= len(texte) then ok = 0

d1 = zlib::UnCompress(c1)
print "aller-retour identique        : ", (d1 = texte), "  (vrai attendu)\n"
if d1 <> texte then ok = 0

' --- le nom en minuscules des sections marche aussi : les noms sont
'     insensibles a la casse, et le manuel ecrit zlib::UnCompress ---
d1b = zlib::uncompress(c1)
if d1b <> texte then ok = 0
print "meme appel en minuscules      : ", (d1b = texte), "  (vrai attendu)\n"

' --- une chaine binaire : le zero ne doit pas tronquer ---
binaire = "debut" & chr$(0) & chr$(255) & chr$(1) & "fin"
c2 = zlib::Compress(binaire)
d2 = zlib::UnCompress(c2)
print "aller-retour binaire          : ", (d2 = binaire), "  (vrai attendu)\n"
if d2 <> binaire then ok = 0
print "longueur binaire rendue       : ", len(d2), "  (attendu ", len(binaire), ")\n"
if len(d2) <> len(binaire) then ok = 0

' --- 20 000 octets : plusieurs passages dans le tampon de 1 024 ---
gros = ""
for i = 1 to 2000
  gros = gros & "0123456789"
next i
print "taille du gros texte          : ", len(gros), "  (attendu 20000)\n"
if len(gros) <> 20000 then ok = 0
c3 = zlib::Compress(gros)
d3 = zlib::UnCompress(c3)
print "20 000 octets, aller-retour   : ", (d3 = gros), "  (vrai attendu)\n"
if d3 <> gros then ok = 0

' ---------------------------------------------------------------------------
' 2) gzip / gunzip sur un fichier
'
' gzip ecrit <fichier>.gz puis retire l'original ; gunzip relit l'en-tete gzip
' (magie 1f 8b) et retire le .gz. Le fichier restaure est relu ici.
' ---------------------------------------------------------------------------
open "zlib_essai.txt" for output as 1
print#1, "ligne un de l'essai zlib\n"
print#1, string$(2000, "z") & "\n"
close 1
taille = filelen("zlib_essai.txt")

call zlib::gzip("zlib_essai.txt")
taille_gz = filelen("zlib_essai.txt.gz")
print "fichier .gz produit           : ", taille_gz, "  (> 0 et < au texte)\n"
if taille_gz <= 0 then ok = 0
if taille_gz >= taille then ok = 0

call zlib::gunzip("zlib_essai.txt.gz")
taille2 = filelen("zlib_essai.txt")
print "contenu restaure              : ", taille2, "  (attendu ", taille, ")\n"
if taille2 <> taille then ok = 0

open "zlib_essai.txt" for input as 2
line input #2, l1
line input #2, l2
close 2
' Le fichier ayant ete ecrit en mode texte, la ligne relue peut garder un
' retour chariot final : la comparaison porte sur les caracteres utiles.
print "premiere ligne restauree      : ", (left$(l1, 24) = "ligne un de l'essai zlib"), "  (vrai attendu)\n"
if left$(l1, 24) <> "ligne un de l'essai zlib" then ok = 0
print "longueur de la 2e ligne       : ", len(l2), "  (2000 caracteres utiles)\n"
if len(l2) < 2000 then ok = 0

delete "zlib_essai.txt"
delete "zlib_essai.txt.gz"

if ok then
  print "\nzlib : OK\n"
else
  print "\nzlib : ECHEC\n"
end if
