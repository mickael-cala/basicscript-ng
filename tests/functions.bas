REM Test 4 : fonctions, portee des variables et recursivite.
REM Note : en ScriptBasic les variables d'une fonction sont GLOBALES par
REM defaut. Sans declaration `local`, la boucle `for i = 1 to n` de factorial
REM reecrit le `i` de la boucle appelante. C'est la cause de l'emballement
REM observe avec la version precedente de ce test.

function add(x, y)
  add = x + y
end function

function factorial(n)
  local i, result
  let result = 1
  for i = 1 to n
    result = result * i
  next i
  factorial = result
end function

function fib(n)
  local r
  if n <= 1 then
    r = n
  else
    r = fib(n - 1) + fib(n - 2)
  endif
  fib = r
end function

print "Test de la fonction add :\n"
print "add(5, 7)   = " & add(5, 7) & "\n"
print "add(10, 20) = " & add(10, 20) & "\n"
print "\n"

print "Test de la fonction factorial :\n"
for i = 1 to 10
  print i & "! = " & factorial(i) & "\n"
next i
print "\n"

print "Test de la fonction fib (recursion) :\n"
for i = 0 to 10
  print "fib(" & i & ") = " & fib(i) & "\n"
next i
print "\n"

print "Test des fonctions termine !\n"
