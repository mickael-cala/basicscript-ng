' mt.bas - test du module mt charge comme module ordinaire
'
' mt etait lie en statique dans sbhttpd ; il se compile aussi en bibliotheque
' partagee et se charge par `import mt.bas`. Le module tient une table de
' sessions (identifiants, variables, verrous) utilisee par le serveur pour
' suivre les clients, mais rien n'exige un serveur : une application ordinaire
' peut s'en servir.
'
' Le test exerce le cycle complet d'une session : creation (identifiant MD5 de
' 32 caracteres), affectation au fil courant, relecture, compteur, suppression.
' Le compteur est compare a sa valeur precedente, pas a zero : il compte les
' sessions creees depuis le demarrage du processus.

import mt.bas

ok = 1
n0 = mt::SessionCount()
print "=== module mt ===\n"
print "sessions avant : ", n0, "\n"

id = mt::NewSessionId("scriba-test")
if isdefined(id) then
  print "id cree        : ", id, " (", len(id), " caracteres)\n"
  if len(id) <> 32 then
    print "  attendu 32 caracteres (MD5)\n"
    ok = 0
  end if
else
  print "id cree        : undef\n"
  ok = 0
end if

if ok then
  call mt::SetSessionId(id)
  back = mt::GetSessionId()
  print "id relu        : ", back, "\n"
  if back <> id then
    print "  l'identifiant relu differe de celui cree\n"
    ok = 0
  end if

  n1 = mt::SessionCount()
  print "sessions apres : ", n1, "\n"
  if n1 <> n0 + 1 then
    print "  le compteur n'a pas augmente de 1\n"
    ok = 0
  end if

  call mt::DeleteSession(id)
  print "session fermee\n"
end if

if ok then
  print "\nmt : OK\n"
else
  print "\nmt : ECHEC\n"
end if
