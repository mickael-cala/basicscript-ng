' cio.bas - test fonctionnel de l'extension cio
'
' `cio` est le module console Windows : position du curseur, taille de
' l'ecran et du tampon, titre de la fenetre, lecture du clavier. Il n'a aucun
' sens hors Windows (notunix.txt dans son dossier), et le build ne le produit
' donc que pour les cibles Windows.
'
' Le test est volontairement un essai de chargement et d'appel, pas une
' capture d'ecran : sous CI, la sortie est redirigee et il n'y a pas
' forcement de console. Les fonctions qui interrogent la console rendent
' alors 0 ou undef, ce qui est accepte ; ce qui est exige, c'est que le
' module se charge et que les appels ne produisent ni erreur ni plantage.
'
' Ne sont pas appelees : Detach (ferme la console), GetCh/GetChE (attendent
' une touche), et tout ce qui changerait l'affichage de facon irreversible.

import cio.bas

ok = 1
print "=== extension cio (console Windows) ===\n"

' --- tailles : rendent 0 sans console, jamais un negatif ---------------------
x = cio::SizeX()
y = cio::SizeY()
print "fenetre            : ", x, " x ", y, "\n"
if x < 0 or y < 0 then ok = 0

bw = cio::BufferSizeX()
bh = cio::BufferSizeY()
print "tampon             : ", bw, " x ", bh, "\n"
if bw < 0 or bh < 0 then ok = 0

' --- titre : SetTitle puis GetTitle ----------------------------------------
' GetTitle rend undef quand il n'y a pas de console ; on ne compare alors
' rien, mais on verifie que SetTitle n'a pas echoue.
call cio::SetTitle("scriba-zig : test cio")
titre = cio::GetTitle()
print "titre              : [", titre, "]\n"
if titre <> undef then
  if len(titre) = 0 then ok = 0
end if

' --- position du curseur : sans console, la fonction rend 0 sans rien faire -
call cio::gotoxy(0, 0)
print "gotoxy(0,0)        : appele\n"

' --- clavier : personne ne tape pendant le test, kbhit doit rendre 0 -------
k = cio::kbhit()
print "kbhit              : ", k, "  (0 attendu, aucune touche)\n"
if k <> 0 then ok = 0

if ok then
  print "\ncio : OK\n"
else
  print "\ncio : ECHEC\n"
end if
