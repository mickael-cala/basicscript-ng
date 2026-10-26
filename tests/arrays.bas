REM Test 5 : tableaux.
REM Les indices s'ecrivent entre CROCHETS, pas entre parentheses (voir
REM ug.texi.jam) : les tableaux sont crees automatiquement au premier acces.
REM DIM (tests/dim.bas) fixe seulement l'etendue de depart ; l'acces
REM automatique reste la regle, ce programme le verifie.

REM --- Tableau simple ---
print "Tableau simple :\n"
for i = 0 to 9
  carre[i] = i * i
next i

print "Carres de 0 a 9 :\n"
for i = 0 to 9
  print "carre[" & i & "] = " & carre[i] & "\n"
next i
print "\n"

REM --- Tableau a deux dimensions ---
print "Tableau 2D :\n"
for i = 0 to 2
  for j = 0 to 2
    matrix[i, j] = i * 3 + j + 1
  next j
next i

print "Matrice 3x3 :\n"
for i = 0 to 2
  for j = 0 to 2
    print matrix[i, j] & " "
  next j
  print "\n"
next i
print "\n"

REM --- Tableau de chaines ---
print "Tableau de chaines :\n"
for i = 0 to 4
  noms[i] = "Element " & i
next i
for i = 0 to 4
  print noms[i] & "\n"
next i
print "\n"

REM --- Verification arithmetique du contenu ---
let total = 0
for i = 0 to 9
  total = total + carre[i]
next i
print "Somme des carres de 0 a 9 = " & total & "\n"
print "Resultat attendu            = 285\n"
print "\n"

print "Test des tableaux termine !\n"
