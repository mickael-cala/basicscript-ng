' unix.bas - test fonctionnel de l'extension unix (ex-ux)
'
' Le module s'appelait `ux` dans la distribution d'origine ; il est renomme
' `unix` dans ce portage, comme dans la vague NG : `ux` ne veut rien dire et
' le module est entierement POSIX. Les fonctions exportees deviennent
' `unixfork` et `unixchmod`, et le fichier de declaration s'appelle
' `unix.bas`.
'
' Ce module ne se compile que sous Linux et macOS (build.py le filtre par OS).
' Il demande fork() et chmod(), donc <unistd.h> et <sys/stat.h>, que
' l'interface n'inclut pas elle-meme (ERRATA E12) : build.py les ajoute.
'
' Seul chmod est exerce ici. fork() n'est pas appele : il dupliquerait le
' processus de test, avec deux sorties et deux codes de sortie, ce qui rend le
' resultat ininterpretable par le lanceur. C'est un essai manuel.
'
' chmod ne rend pas de valeur et ignore l'echec du systeme : le test ne peut
' donc pas prouver que le mode a change, seulement que l'appel se fait sans
' erreur et que le fichier reste lisible par son proprietaire.

import unix.bas

ok = 1
print "=== extension unix (fork, chmod) ===\n"

' --- un fichier ecrit par le noyau, puis deux chmod ------------------------
open "tmp_unix_mode.txt" for output as 1
print#1, "contenu de test\n"
close 1

' 0600 : rw-------
call unix::chmod("tmp_unix_mode.txt", &H180)
print "chmod 0600         : appele\n"

' Le proprietaire garde le droit de lire : la relecture doit rendre le texte.
open "tmp_unix_mode.txt" for input as 1
line input #1, l
close 1
print "relecture          : [", l, "]\n"
' line input conserve le saut de ligne : on compare les 15 premiers caracteres.
if left(l, 15) <> "contenu de test" then ok = 0

' 0644 : rw-r--r--
call unix::chmod("tmp_unix_mode.txt", &H1A4)
print "chmod 0644         : appele\n"

if ok then
  print "\nunix : OK\n"
else
  print "\nunix : ECHEC\n"
end if
