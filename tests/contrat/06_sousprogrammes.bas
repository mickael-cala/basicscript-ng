' contrat 06 : sous-programmes, fonctions, module - sans module externe
' (cette grammaire refuse un sub niche dans un module : le module ne
' porte ici qu'une constante, accedee par son nom qualifie)
global compteur

sub Affiche(n)
  print "  sub(", n, ")\n"
  compteur = compteur + 1
end sub

function Factorielle(n)
  if n <= 1 then
    Factorielle = 1
  else
    Factorielle = n * Factorielle(n - 1)
  end if
end function

const LIMITE = 3

compteur = 0
for i = 1 to LIMITE
  Affiche(i)
next i
print "compteur : ", compteur, "\n"
print "fact(5)  : ", Factorielle(5), "\n"
print "fact(0)  : ", Factorielle(0), "\n"

module Boite
  const SECRET = 7
end module
print "boite    : ", Boite::SECRET, "\n"
print "fin 06\n"
