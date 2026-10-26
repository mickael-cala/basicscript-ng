' curl.bas - test fonctionnel de l'extension curl (HTTP, dependance externe)
'
' curl dormait depuis l'origine du portage : la distribution ne livrait
' aucun en-tete ni import de libcurl. Le reveil suit le modele odbc :
' en-tetes vendores (resources/curl/, libcurl 8.21.0), import genere sur
' la machine (tcc -impdef, lib/libcurl-4.def, non versionne), cible
' declaree rsp/ext_curl.rsp, construite a la demande :
'     ./obj/sbtool.exe build ext_curl --link --arch 64
'
' La DLL runtime libcurl-4.dll n'est pas dans le depot : produit de
' MinGW, elle se charge ici depuis le chemin systeme. Sans elle, la
' campagne saute proprement ce test (module 'curl' non construit), le
' precedent dyc en 64 bits.
'
' Ce que ce test etablit :
'   - la DLL curl.dll produite par TinyCC se charge dans l'interpreteur,
'     et ses imports vers libcurl-4.dll se resolvent ;
'   - les fonctions pures : version, escape/unescape, getdate ;
'   - le chemin complet init/option/perform : URL file:// (reseau local
'     du systeme de fichiers, aucun serveur, aucune sortie reseau) et
'     collecte du corps en chaine par buffercollect ;
'   - le refus honnete d'une URL file:// inexistante (erreur du module,
'     remontee par perform, pas un echec de chargement) ;
'   - le refus de CLOSEPOLICY : option perdue de libcurl, rejetee par
'     curl_easy_setopt (CURLE_UNKNOWN_OPTION) et remontee comme les
'     autres erreurs d'option.
'
' Ce que ce test n'etablit PAS : HTTPS, POST, FTP — un serveur et des
' identifiants, absents de la campagne. La distribution ne livrait pas
' d'import pour curl ; les fonctions s'appellent sous le nom du module,
' nommees par les marqueurs =section de l'interface : init, option,
' perform, info, finish, error, escape, unescape, getdate, version.
' (Ces marqueurs portaient « curl_init », d'ou un curl::curl_init
' redondant, alors que le bloc de documentation de l'interface annonce
' curl::init() depuis l'origine : PATCHES.md B.15.)

import curl.bas

ok = 1
print "=== extension curl (HTTP, dependance externe libcurl) ===\n"

' ---------------------------------------------------------------------------
' 1) version : la bibliotheque repond, avec ses composants
' ---------------------------------------------------------------------------
v = curl::version()
print "libcurl                       : ", left$(v, 40), "...\n"
if len(v) < 20 then ok = 0

' ---------------------------------------------------------------------------
' 2) escape / unescape : aller-retour
' ---------------------------------------------------------------------------
e = curl::escape("a b&c")
u = curl::unescape("a%20b%26c")
print "escape/unescape aller-retour  : ", (u = "a b&c"), "  (vrai attendu)\n"
if e <> "a%20b%26c" then ok = 0
if u <> "a b&c" then ok = 0

' ---------------------------------------------------------------------------
' 3) getdate : epoque d'une date HTTP figee
' (Thu Oct  8 12:00:00 UTC 2026, valeur confirmee par date -u)
' ---------------------------------------------------------------------------
g = curl::getdate("Thu, 08 Oct 2026 12:00:00 GMT")
print "getdate                       : ", g, "  (1791460800 attendu)\n"
if g <> 1791460800 then ok = 0

' ---------------------------------------------------------------------------
' 4) perform sur URL file:// : le chemin complet, sans serveur ni reseau.
' L'URL file exige un chemin absolu, separe par des / : curdir rend le
' repertoire courant, que la boucle convertit, comme le ferait une
' commande du langage si elle existait.
' ---------------------------------------------------------------------------
c = curdir()
u2 = ""
for i = 1 to len(c)
  ch = mid$(c, i, 1)
  if ch = "\\" then ch = "/"
  u2 = u2 & ch
next i

' le fichier temoin : ecrit par le test, relu par le module, efface ensuite.
' Une seule ligne, sans saut de ligne : print# traduit les \n en CRLF
' sous Windows, et la longueur comparee doit tenir sans ambiguite ; les
' 1 024 premiers z forcent buffercollect a realoigner son tampon.
open "curl_essai.txt" for output as 1
print#1, string$(1024, "z") & "ligne un de l'essai curl"
close 1

h = curl::init()
curl::option(h, "URL", "file:///" & u2 & "/curl_essai.txt")
r = curl::perform(h)
curl::finish(h)
attendu = string$(1024, "z") & "ligne un de l'essai curl"
print "perform file://               : ", (r = attendu), "  (vrai attendu)\n"
print "longueur recue                : ", len(r), "  (attendu ", len(attendu), ")\n"
if len(r) <> len(attendu) then ok = 0
if r <> attendu then ok = 0

' ---------------------------------------------------------------------------
' 5) refus d'une URL file:// inexistante : perform remonte une erreur de
' module (0x00081100 + code libcurl), interceptee ici ; l'absence d'erreur
' serait un echec.
' ---------------------------------------------------------------------------
on error goto RefusURL
h = curl::init()
curl::option(h, "URL", "file:///fichier-inexistant-curl-essai.txt")
r = curl::perform(h)
curl::finish(h)
print "refus URL inexistante         : PAS D'ERREUR  (erreur attendue)\n"
ok = 0
goto SuiteURL
RefusURL:
print "refus URL inexistante         : intercepte\n"
SuiteURL:

' ---------------------------------------------------------------------------
' 6) refus de CLOSEPOLICY : l'option a perdu sa fonction en 7.x, les en-tetes
' actuels la gardent comme alias de l'emplacement obsolete 72, et setopt
' repond CURLE_UNKNOWN_OPTION ; le module remonte l'erreur comme les autres.
' ---------------------------------------------------------------------------
on error goto RefusCP
h = curl::init()
curl::option(h, "CLOSEPOLICY", "OLD")
print "refus CLOSEPOLICY             : PAS D'ERREUR  (erreur attendue)\n"
ok = 0
goto SuiteCP
RefusCP:
print "refus CLOSEPOLICY             : intercepte\n"
SuiteCP:

delete "curl_essai.txt"

if ok then
  print "\ncurl : OK\n"
else
  print "\ncurl : ECHEC\n"
end if

end
