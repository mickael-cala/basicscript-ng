REM Test 2 : variables, arithmetique et chaines.
REM Note : ScriptBasic n'a pas d'operateur textuel `mod`, le reste de la
REM division s'ecrit avec le caractere % (voir syntax.def : `MOD: %`).
REM Note : `print` n'ajoute pas de retour a la ligne, d'ou les \n explicites.

let a = 10
let b = 20
let c = a + b

print "a = " & a & "\n"
print "b = " & b & "\n"
print "c = a + b = " & c & "\n"
print "\n"

let x = 100
let y = 7

print "x = " & x & "\n"
print "y = " & y & "\n"
print "x + y = " & (x + y) & "\n"
print "x - y = " & (x - y) & "\n"
print "x * y = " & (x * y) & "\n"
print "x / y = " & (x / y) & "\n"
print "x % y = " & (x % y) & "\n"
print "\n"

let s1 = "Hello"
let s2 = "World"
let s3 = s1 & " " & s2

print "s1 = " & s1 & "\n"
print "s2 = " & s2 & "\n"
print "s3 = s1 & s2 = " & s3 & "\n"
print "\n"

REM Comparaisons, pour verifier que le type booleen circule.
print "5 < 9  = " & (5 < 9) & "\n"
print "5 >= 9 = " & (5 >= 9) & "\n"
print "\n"

print "Test des variables termine !\n"
