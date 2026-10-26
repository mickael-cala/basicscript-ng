REM Test 3 : boucles et conditions.

print "Boucle FOR de 1 a 10 : "
for i = 1 to 10
  print i & " "
next i
print "\n"

print "Boucle FOR avec pas de 2 : "
for i = 0 to 10 step 2
  print i & " "
next i
print "\n"

print "Boucle WHILE : "
let count = 5
while count > 0
  print count & " "
  count = count - 1
wend
print "BOOM!\n"

print "Test des conditions :\n"
let x = 42
if x > 50 then
  print "x est superieur a 50\n"
elseif x > 40 then
  print "x est superieur a 40\n"
elseif x > 30 then
  print "x est superieur a 30\n"
else
  print "x est inferieur ou egal a 30\n"
endif

print "Boucle DO...LOOP : "
let n = 1
do
  print n & " "
  n = n * 2
loop while n < 100
print "\n"

print "Test des boucles termine !\n"
