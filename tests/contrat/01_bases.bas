' contrat 01 : valeurs, arithmetique, controle de flux - sans module
print "entier  : ", 42, "\n"
print "negatif : ", -7, "\n"
print "reel    : ", 2.5, "\n"
print "chaine  : ", "bonjour\n"
print "concat  : ", "a" & "b" & "c", "\n"
print "egal    : ", 1 = 1, "\n"
print "diff    : ", 1 = 2, "\n"
print "sup     : ", 10 > 2, "\n"
for i = 1 to 3
  print "boucle for ", i, "\n"
next i
j = 0
while j < 3
  j = j + 1
  print "boucle while ", j, "\n"
wend
k = 0
repeat
  k = k + 1
  print "boucle repeat ", k, "\n"
until k = 3
if 1 = 2 then
  print "impossible\n"
else
  print "if/else : le sinon marche\n"
end if
print "fin 01\n"
