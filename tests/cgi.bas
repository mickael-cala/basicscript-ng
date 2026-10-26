' cgi.bas - test du module cgi charge comme module ordinaire
'
' cgi etait jusqu'ici lie en statique dans sbhttpd seulement : aucun test ne
' pouvait l'exercer. Compile par `build.py --modules`, il se charge par
' `import cgi.bas` et fonctionne hors serveur, a condition de lui donner un
' environnement de requete.
'
' Son bootmodu appelle cgi_ReadHttpRequest des le chargement. Sans
' REQUEST_METHOD, le module bascule en mode "fichier de debogage" et rend
' l'erreur 0x00080004 : un programme ordinaire ne peut donc pas l'importer
' sans variables CGI. tests/run.py fournit celles d'une requete GET pour ce
' test precis (voir TEST_ENV) :
'
'   REQUEST_METHOD=GET  QUERY_STRING=a=1&b=deux  SERVER_NAME=scriba.test
'
' Le test verifie la chaine de requete, l'extraction des parametres (y compris
' l'absence), les variables d'environnement et l'en-tete imprime par le .bas.

import cgi.bas

ok = 1
print "=== module cgi ===\n"

' --- chaine de requete et parametres --------------------------------------
q = cgi::QueryString()
print "QueryString    : [", q, "]\n"
if q <> "a=1&b=deux" then ok = 0

a = cgi::GetParam("a")
b = cgi::GetParam("b")
print "GetParam(a)    : [", a, "]\n"
print "GetParam(b)    : [", b, "]\n"
if a <> "1" then ok = 0
if b <> "deux" then ok = 0

absent = cgi::GetParam("absent")
if isdefined(absent) then
  print "GetParam(absent): [", absent, "] (inattendu)\n"
  ok = 0
else
  print "GetParam(absent): undef (attendu)\n"
end if

' --- variables d'environnement --------------------------------------------
m = cgi::RequestMethod()
s = cgi::ServerName()
print "RequestMethod  : [", m, "]\n"
print "ServerName     : [", s, "]\n"
if m <> "GET" then ok = 0
if s <> "scriba.test" then ok = 0

' --- en-tete HTTP imprime par le .bas -------------------------------------
' Header n'ecrit qu'une fois par processus ; il s'appuie sur ServerSoftware.
print "--- en-tete attendu ci-dessous ---\n"
cgi::Header 200, "text/plain"
cgi::FinishHeader
print "corps du programme\n"

if ok then
  print "\ncgi : OK\n"
else
  print "\ncgi : ECHEC\n"
end if
