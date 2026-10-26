' cgi_upload.bas - test de non-regression du POST multipart et de l'upload
'
' Meme programme dans les deux modes :
'   - CGI externe : `scriba -c tests/cgi_upload.bas`, corps multipart sur
'     stdin, REQUEST_METHOD/CONTENT_LENGTH/CONTENT_TYPE dans l'environnement ;
'   - CGI interne : `sbhttpd` sert ce fichier, le module cgi recoit le corps
'     par pfStdIn et le type par pfEnv.
'
' Le fichier sauve est choisi par CGI_UPLOAD_SAVE en mode externe. Dans
' sbhttpd, pfEnv ne transmet pas les variables arbitraires : environ() rend
' undef et le script retombe sur "uploaded.bin" dans le repertoire courant du
' serveur (le dossier temporaire du test).
'
' Le corps est construit par tests/run.py : un champ texte `apple`, un champ
' `x`, puis un fichier `up`. Le nom et la taille attendus du fichier sont lus
' dans la query (`upname`, `uplen`) : le meme script sert donc au petit
' fichier ASCII de 8 octets et au fichier binaire de 65536 octets, zeros
' compris. Le script verifie les valeurs, sauve le fichier, puis affiche le
' resultat ; le lanceur controle aussi le contenu du fichier sur disque.
'
' Les cas multipart-filemax et multipart-filemax-256k reutilisent ce meme
' fichier avec 10 Mo + 1 octets, puis 10 Mo + 256 Ko : le module cgi refuse
' alors le corps pendant son demarrage et aucune ligne ci-dessous ne
' s'execute. Le lanceur y attend CGI_ERROR_FILEMAX, et pour le second une
' fermeture propre de la connexion par sbhttpd.

' CONTENT_MAX vaut 10 Mo, comme FILE_MAX : sans relever le premier, un
' fichier de 10 Mo + 1 octet serait refuse comme contenu trop grand
' (0x00080002) avant d'atteindre le controle de taille de fichier. On laisse
' FILE_MAX a sa valeur par defaut pour verifier 0x00080008.
option cgi$contentmax 20971520
import cgi.bas

ok = 1
method = cgi::RequestMethod()
ctype = cgi::ContentType()
apple = cgi::PostParam("apple")
extra = cgi::PostParam("x")
upname = cgi::FileName("up")
uplen = cgi::FileLength("up")
upname_exp = cgi::GetParam("upname")
if not isdefined(upname_exp) then upname_exp = "upload.txt"
uplen_exp = cgi::GetParam("uplen")
if not isdefined(uplen_exp) then uplen_exp = "8"
savepath = environ("CGI_UPLOAD_SAVE")
if not isdefined(savepath) then savepath = "uploaded.bin"

if method <> "POST" then ok = 0
if ctype <> "multipart/form-data; boundary=scriba-test-boundary" then ok = 0
if apple <> "vert" then ok = 0
if extra <> "1" then ok = 0
if upname <> upname_exp then ok = 0
if uplen <> val(uplen_exp) then ok = 0

cgi::SaveFile "up", savepath

cgi::Header 200, "text/plain"
cgi::FinishHeader
print "method=", method, "\n"
print "ctype=", ctype, "\n"
print "apple=", apple, "\n"
print "x=", extra, "\n"
print "upname=", upname, "\n"
print "uplen=", uplen, "\n"
print "saved=", savepath, "\n"
if ok then
  print "cgi_upload : OK\n"
else
  print "cgi_upload : ECHEC\n"
end if
