; scriba.conf.lsp - configuration ScriptBasic du portage TinyCC
; ===========================================================
;
; ScriptBasic ne lit pas ce fichier directement : il lit sa version binaire
; (scriba.conf) placee a cote de l'executable. Ce texte est la source, la
; version binaire se produit avec :
;
;     bin/scriba.exe -f bin/scriba.conf -k scriba.conf.lsp
;
; L'option -f designe le fichier binaire a ecrire ; sans elle, ScriptBasic
; chercherait <repertoire de l'executable>/scriba.conf, et en l'absence de
; ce fichier retomberait sur %windir%\SCRIBA.INI, hors du projet.
;
; Le fichier officiel du depot (../scriba.conf.lsp) pointe vers
; t:\ScriptBasic\... et t:\MyProjects\ScriptBasic\..., chemins de la machine
; de developpement d'origine. Cette version-ci les remplace par des chemins
; relatifs a la racine du depot, ce qui rend le build autonome.
;
; La syntaxe est celle du langage LSP de ScriptBasic : un nom, une valeur,
; et des blocs imbriques entre parentheses.

; l'extension des bibliotheques a chargement dynamique
dll ".dll"

; ou les modules sont charges depuis
module "modules/"

; ou chercher les fichiers include systeme et de modules
include "modules/include/"

; ou les modules installent leur documentation
docu "modules/docu/"

; nombre maximum d'include imbriques
maxinclude 100

; ---------------------------------------------------------------------------
; Configuration du serveur HTTP (variation httpd)
; ---------------------------------------------------------------------------
;
; Les scripts peuvent changer le repertoire courant, tous les chemins sont
; donc donnes depuis la racine du projet.

; `servers.port` et `servers.server.port` ont deux roles distincts dans
; websrv.c : le premier alimente la variable d'environnement SERVER_PORT vue
; par les scripts, le second le binding effectif. Les deux doivent concorder,
; sinon SERVER_PORT annoncerait 0.
;
; Les cinq journaux sont declares : sans cle `servers.log.panic.file` et
; `servers.log.app.file`, AppInit refuse de demarrer (et `servers.nolog 1`
; serait la seule alternative, mais on perdrait la trace des requetes).
;
; Le fichier pid est obligatoire des lors que les journaux sont actifs :
; AppStart echoue sur `if( ! pWD->nolog )iError = 1` quand il ne peut pas
; l'ouvrir. Il joue aussi un role utile : le thread de garde surveille ce
; fichier et arrete le serveur des qu'on le supprime, ce qui donne un moyen
; d'arret propre sans tuer le processus.

servers (
  server (
    port 18080
    ip "127.0.0.1"
    protocol http
    )
  port 18080
  ip "127.0.0.1"
  threads 4
  listenbacklog 30
  home "webtest/"
  run404 "webtest/404.bas"
  proxyip 0
  log (
    panic ( file "logs/panic.txt" )
    app   ( file "logs/app.txt" )
    err   ( file "logs/err.txt" )
    hit   ( file "logs/hit.txt" )
    stat  ( file "logs/stat.txt" )
    )
  pid (
    file "logs/pid.txt"
    delay 1
    wait (
      period 2
      length 1
      )
    )
  client (
    allowed "127.0.0.1/255.255.255.255"
    )
  )
