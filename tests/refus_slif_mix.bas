' Une construction est soit entierement multi-ligne, soit entierement sur une
' ligne : le ELSEIF qui suit un corps multi-ligne ne peut pas porter sa branche
' sur la meme ligne, sinon la chaine se ferme a la fin de la ligne et END IF
' n'a plus rien a fermer. Refus a l'analyse (E74/E77).
if 1 then
  print "a"
elseif 1 then print "b"
end if
