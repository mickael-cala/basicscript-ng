' EXIT quitte la boucle demandee. Les tests verifient aussi que la commande
' qui suit la boucle s'execute (le saut ne doit ni la sauter ni l'engloutir),
' et que EXIT ne sort que de la boucle intérieure de l'espece demandee.

' EXIT FOR depuis une boucle for imbriquee : seule la boucle interieure sort.
for i = 1 to 3
  for j = 1 to 5
    if j = 3 then exit for
  next
  print "for ",i," ",j,"\n"
next

' La commande apres la boucle doit etre la premiere executee.
for i = 1 to 1
  exit for
  print "jamais for\n"
next
print "apres for\n"

' EXIT WHILE.
i = 0
while i < 10
  i += 1
  if i = 3 then exit while
wend
print "while ",i,"\n"

' EXIT DO, dans un do classique ferme par loop.
i = 0
do
  i += 1
  if i = 2 then exit do
loop
print "do ",i,"\n"
print "apres do\n"

' EXIT REPEAT.
i = 0
repeat
  i += 1
  if i = 2 then exit repeat
until i > 5
print "repeat ",i,"\n"

' EXIT nu depuis deux boucles de natures differentes : seule la plus
' interieure est quittee.
for i = 1 to 2
  do
    exit
  loop
  print "nu ",i,"\n"
next

' EXIT DO traverse un for : c'est le do qui est quitte, pas le for.
for i = 1 to 3
  do
    exit do
  loop
  print "do dans for ",i,"\n"
next

' EXIT FOR traverse un while : c'est le for qui est quitte, pas le while.
for i = 1 to 3
  j = 0
  while j < 5
    j += 1
    if j = 2 then exit for
  wend
  print "jamais while\n"
next
print "for depuis while ",i,"\n"

' EXIT est borne par la subroutine : une boucle de l'appelant n'est pas
' visible et la boucle interne de la subroutine continue de fonctionner.
sub essai
  for k = 1 to 4
    exit for
  next
  print "sub ",k,"\n"
end sub
for i = 1 to 2
  essai()
next

' Meme portee pour une fonction : la boucle interne de la fonction est
' quittee, celle de l'appelant continue.
function premier
  for k = 1 to 4
    exit for
  next
  premier = k
end function
for i = 1 to 2
  print "fonction ",premier(),"\n"
next

print "exit : OK\n"

' EXIT sur la derniere construction du programme : le label de sortie n'a
' pas de ligne suivante, le saut doit simplement terminer le programme.
for i = 1 to 2
  exit for
next
