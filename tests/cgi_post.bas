' cgi_post.bas - test de non-regression du POST CGI
'
' Le meme programme sert deux fois :
'   - CGI externe : `scriba -c tests/cgi_post.bas`, corps sur stdin,
'     REQUEST_METHOD=POST et CONTENT_LENGTH dans l'environnement ;
'   - CGI interne : `sbhttpd` sert ce meme fichier, le module cgi lie en
'     statique recoit les valeurs par pfStdIn/pfEnv.
'
' tests/run.py --cgi-post fournit l'environnement et le corps, puis exige
' les lignes affichees ci-dessous. Le script verifie aussi lui-meme pour que
' la sortie reste lisible quand le test echoue.
'
' Le cas contentmax reutilise ce fichier avec un Content-Length de
' CONTENT_MAX + 1 et aucun corps : le module cgi refuse sur l'en-tete, donc
' aucune ligne ci-dessous ne s'execute. Le script ne doit pas relever
' cgi$contentmax, sans quoi le refus n'aurait pas lieu.

import cgi.bas

ok = 1
method = cgi::RequestMethod()
query = cgi::QueryString()
length = cgi::ContentLength()
ctype = cgi::ContentType()
apple = cgi::PostParam("apple")
extra = cgi::PostParam("x")

if method <> "POST" then ok = 0
if query <> "q=1" then ok = 0
if length <> 14 then ok = 0
if ctype <> "application/x-www-form-urlencoded" then ok = 0
if apple <> "vert" then ok = 0
if extra <> "1" then ok = 0

cgi::Header 200, "text/plain"
cgi::FinishHeader
print "method=", method, "\n"
print "query=", query, "\n"
print "len=", length, "\n"
print "ctype=", ctype, "\n"
print "apple=", apple, "\n"
print "x=", extra, "\n"
if ok then
  print "cgi_post : OK\n"
else
  print "cgi_post : ECHEC\n"
end if
