#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
tests/run.py - execute les tests ScriptBasic sur un binaire construit.

    python tests/run.py --target windows-x64
    python tests/run.py --target linux-x64            (lance sous WSL si besoin)
    python tests/run.py --target windows-x64 --modules
    python tests/run.py --target windows-x64 --upstream
    python tests/run.py --target windows-x64 --upstream-all
    python tests/run.py --target windows-x64 --standalone
    python tests/run.py --target windows-x64 --re-engine
    python tests/run.py --target windows-x64 --cgi-post
    python tests/run.py --grammaire
    python tests/run.py --contrat
    python tests/run.py --embed
    python tests/run.py --exe dist/macos-arm64/scriba

Le jeu par defaut ne contient que des tests SANS module externe (pas de re,
hash, curl, odbc...) : ce sont les seuls qui doivent passer sur les six
cibles. `operators.bas` y figure : il vérifie les opérateurs étendus qui
restent (`<<`, `>>`, `??`, `**`, `//` et les affectations composées). `--modules` ajoute les tests des modules construits par
`build.py --modules` : hash, re, t (via md5.bas), trial, cgi, mt, zlib et
odbc partout, cio, win, dyc et curl sous Windows (curl en 64 bits
seulement, ERRATA E40), unix sous Linux/macOS.

`--upstream` lance le sous-ensemble de `tests/upstream/` decrit par
`tests/upstream/MANIFEST.tsv` : tests du coeur et tests des modules
reellement construits. `--upstream-all` ajoute les essais longs et les
diagnostics. Un test qui exige un module absent, un reseau ou une saisie est
saute avec sa raison, jamais compte comme un echec silencieux.

`--standalone` ne lance aucun `.bas` : il execute `hello_standalone`, deja
construit par `build.py --standalone`, et exige ses deux lignes de sortie. Il
ne construit rien lui-meme. `--stndlone` fait de meme pour le second chemin,
avec `hello_stndlone` produit par `build.py --stndlone`. `--re-engine`
execute l'essai C du moteur Spencer produit par `build.py --re-engine` et
exige son verdict `moteur re : OK`.

`--cgi-post` verifie le POST CGI dans les deux modes : `scriba -c` avec le
corps sur stdin et les variables `REQUEST_METHOD`/`CONTENT_LENGTH`, puis le
meme script servi par `sbhttpd`. Neuf cas passent par les deux chemins :
`application/x-www-form-urlencoded` ; `multipart/form-data` avec un petit
fichier ASCII ; `multipart/form-data` avec 65536 octets binaires, zeros
compris ; `multipart/form-data` avec un fichier de FILE_MAX+1 octets, qui
est refuse ; le meme refus avec 256 Ko de plus, pour verifier que le module
consomme le reste du corps et que le serveur ferme proprement ; un corps
annonce au-dela de CONTENT_MAX, refuse sur le seul en-tete ; et trois erreurs
qui verifient les trois chemins du rapporteur : execution (code 22, bit 0x2),
chargement (include absent, rapporte par le lecteur) et module (0x00080000,
au-dela de MAX_ERROR_CODE). Toutes trois exigent une page d'erreur complete
(ERRATA E24). `cgi_upload.bas` verifie `FileName`, `FileLength`, `SaveFile` et
le contenu sauve ; sur le dernier cas, aucune ligne du script ne s'execute :
le lanceur exige le code CGI_ERROR_FILEMAX (0x00080008) et, par sbhttpd, le
statut 413. Le test exige que
`sbhttpd` et le module `cgi` soient construits (`build.py --httpd` et
`build.py --modules`) ; le serveur recoit une configuration temporaire, sur
un port libre, et est arrete par suppression de son fichier pid. Le test ne
passe pas par WSL : une cible non native est sautee, car il ouvre un vrai
port local.

`--grammaire` ne lance rien et ne demande aucun binaire : il relit
`src/syntax.c` et `src/syntax.h` et echoue si un nom de la grammaire pointe sur
un emplacement vide de `CommandFunction[]` (la classe de l'erreur 28), si un
code `CMD_` n'a pas de `#define`, ou si un `NOTIMP_*` est arme. Les onze mots du
preprocesseur sans corps (`const`, `var`, `global`, `module`, `endmodule`,
`local`, `label`, `nlabel`, `empty`, `pragma`, `gconst`) sont declares en tete
du fichier avec leur raison : ils n'atteignent jamais l'execution, et la liste
est explicite pour qu'un nouveau nom sans corps fasse echouer le controle au
lieu de passer inapercu.

Quatre precautions que le portage a rendues necessaires :

1. ScriptBasic rend TOUJOURS 0, meme sur une erreur de syntaxe ou d'include
   (mesure : un programme fautif sort avec le code 0 et un message
   "error &H.."). Un test qui se contenterait du code de sortie passerait
   donc a tort. On considere comme echec : un code non nul, une ligne
   "ECHEC" dans la sortie, ou un message "error &H" du rapporteur.

2. Les tests de modules se lancent depuis dist/<cible>, parce que les cles
   `module` et `include` de la configuration sont relatives au repertoire
   courant ("modules/", "modules/include/"). Sous Windows, l'interpreteur y
   trouve aussi scriba.conf a cote de l'executable ; sur UNIX il ne le fait
   pas, et c'est la variable SCRIBACONF qui le lui indique.

3. cgi exige un environnement de requete des son chargement : sans
   REQUEST_METHOD, son bootmodu bascule en mode "fichier de debogage" et rend
   l'erreur 0x00080004. La table TEST_ENV, ci-dessous, fournit donc une fausse
   requete GET aux tests cgi, testmt et testmtlock ; les autres ne la voient pas.

4. Un test upstream peut etre un simple programme de demonstration, sans
   assertion : `ok` signifie ici que le programme s'execute jusqu'au bout et
   que le rapporteur ne signale ni "ECHEC", ni "error &H", ni "error 0x".
   Le manifeste dit quels fichiers sont exclus et pourquoi.
"""

import argparse
import http.client
import os
import re
import shutil
import socket
import subprocess
import sys
import tempfile
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

TESTS = ["hello", "vars", "loops", "arrays", "dim", "functions", "operators",
         "commandes", "mathfunc", "trig", "nombres", "slif", "exit"]

# Fichiers qui doivent etre refuses A L'ANALYSE : le lanceur exige le message
# du rapporteur et interdit toute execution. C'est la contrepartie des tests
# ordinaires, qui exigent l'absence de « error &H » (ERRATA E28).
REFUS_ANALYSE = [("refus_directory.bas", "error &H77"),
                 ("refus_pause.bas", "error &H77"),
                 ("refus_fork.bas", "error &H68"),
                 ("refus_sqr.bas", "error &H68"),
                 ("refus_operateur.bas", "error &H77"),
                 ("refus_cv.bas", "error &H68"),
                 ("refus_mk.bas", "error &H68"),
                 ("refus_slif_for.bas", "error &H77"),
                 ("refus_slif_mix.bas", "error &H77"),
                 ("refus_exit.bas", "error &H74"),
                 ("refus_exit_for.bas", "error &H74"),
                 ("refus_exit_while.bas", "error &H74"),
                 ("refus_exit_sub.bas", "error &H74"),
                 ("refus_isdefined.bas", "error &H71"),
                 ("refus_dim.bas", "error &H77"),
                 ("refus_dim_imbrique.bas", "error &H77"),
                 ("refus_dim_liste.bas", "error &H77")]

# Tests des modules construits par `build.py --modules`. md5.bas est le test
# du module t : c'est son nom historique, il porte deja l'en-tete du module.
# cgi et mt etaient lies dans sbhttpd seulement ; ils sont desormais des
# modules ordinaires et ont chacun leur test.
MODULE_TESTS = ["hash", "re", "md5", "trial", "cgi", "mt", "zlib", "odbc", "bytes"]

# Modules propres a une famille d'OS, dans le meme ordre que build.py : cio,
# win, dyc et curl n'existent que sous Windows, unix (ex-ux) que sous
# Linux/macOS. Les lancer la ou le module n'est pas construit ferait un
# echec de chargement.
MODULE_TESTS_BY_OS = {"windows": ["cio", "win", "dyc", "curl"],
                      "unix": ["unix"]}

# Modules absents d'une cible malgre la bonne famille d'OS : le test est
# saute avec sa raison, jamais compte en echec. curl exige la DLL
# d'execution libcurl-4.dll, qui n'existe ici qu'en 64 bits (ERRATA E40) ;
# dyc est 32 bits seulement (ERRATA E10). odbc est revenu sous Linux par
# le shim de SONAME (E42 corrige) : il demande juste libodbc2 installe
# sur la machine d'execution.
MODULE_TESTS_SKIP = {
    "curl": {"windows-x86": "pas de libcurl-4.dll 32 bits ici"},
    "dyc": {"windows-x64": "module 32 bits seulement (ERRATA E10)"},
    "odbc": {"macos-x64": "ni -lodbc ni en-tetes ODBC dans le sysroot",
             "macos-arm64": "ni -lodbc ni en-tetes ODBC dans le sysroot"},
    # mt n'est pas construit sur macOS (E47) : sans ce saut, le test
    # chercherait sa declaration dans modules/include/ et mourrait en
    # &H3e au lieu de sauter avec sa raison.
    "mt": {"macos-arm64": "non construit sur macOS (ERRATA E47)",
           "macos-x64": "non construit sur macOS (ERRATA E47)"},
}

# Le harnais du manuel (156 affirmations sur les exemples de MANUEL.md) :
# ses attentes encodent la plateforme LLP64 de Windows (maxint et minint,
# le generateur aleatoire, lof et les fins de ligne). Sous Linux, les cinq
# ecarts d'ABI sont documentes a l'ERRATA E44 : il entre dans le jeu par
# defaut des cibles Windows seulement.
MANUEL_TESTS_BY_OS = {"windows": ["manuel"]}

MODULE_TESTS_ALL = MODULE_TESTS + [t for v in MODULE_TESTS_BY_OS.values()
                                  for t in v]

# Variables d'environnement propres a un test. cgi n'a pas d'autre facon de
# savoir qu'il tourne dans une requete : sans REQUEST_METHOD, son bootmodu
# bascule en mode "fichier de debogage" et rend l'erreur 0x00080004. Le test
# simule donc une requete GET avec une chaine de requete fixe. testmt et
# testmtlock incluent cgi et demandent le meme environnement.
CGI_ENV = {
    "REQUEST_METHOD": "GET",
    "QUERY_STRING": "a=1&b=deux",
    "SERVER_NAME": "scriba.test",
    "SERVER_SOFTWARE": "scriba-test/1.0",
    "GATEWAY_INTERFACE": "CGI/1.1",
    "SERVER_PROTOCOL": "HTTP/1.1",
    "SERVER_PORT": "8080",
}

TEST_ENV = {"cgi": CGI_ENV, "testmt": CGI_ENV, "testmtlock": CGI_ENV}

# Requetes POST partagees par --cgi-post. Les corps sont des octets : le
# multipart contient des CR/LF et doit traverser stdin sans traduction de
# fin de ligne. Le corps urlencoded fait 14 octets ; le script le verifie,
# ce qui controle que CONTENT_LENGTH est transmis et respecte.
CGI_POST_BODY = b"apple=vert&x=1"
CGI_POST_ENV = {
    "REQUEST_METHOD": "POST",
    "QUERY_STRING": "q=1",
    "CONTENT_LENGTH": str(len(CGI_POST_BODY)),
    "CONTENT_TYPE": "application/x-www-form-urlencoded",
    "SERVER_NAME": "scriba.test",
    "SERVER_SOFTWARE": "scriba-test/1.0",
    "GATEWAY_INTERFACE": "CGI/1.1",
    "SERVER_PROTOCOL": "HTTP/1.1",
    "SERVER_PORT": "8080",
}
CGI_POST_EXPECT = ("method=POST", "query=q=1", "len=14",
                   "ctype=application/x-www-form-urlencoded", "apple=vert",
                   "x=1", "cgi_post : OK")

# Multipart : un champ texte apple, un champ x, puis un fichier up. La
# frontiere est fixe pour que cgi_upload.bas puisse verifier le Content-Type
# exact. Le premier fichier est ASCII (8 octets) ; le second contient les 256
# valeurs d'octet, zeros compris, et 65536 octets : il depasse BUFFER_MAX
# (10 Ko) et HIT_MAX_SIZE (32 Ko) tout en restant sous FILE_MAX (10 Mo).
CGI_UPLOAD_BOUNDARY = "scriba-test-boundary"
CGI_UPLOAD_CONTENT = "fichier\n"
CGI_UPLOAD_BYTES = CGI_UPLOAD_CONTENT.encode("ascii")
CGI_UPLOAD_TYPE = f"multipart/form-data; boundary={CGI_UPLOAD_BOUNDARY}"


def multipart_body(filename, content):
    """Construit apple=vert, x=1 et un fichier binaire nomme T<filename>."""
    head = (
        f"--{CGI_UPLOAD_BOUNDARY}\r\n"
        'Content-Disposition: form-data; name="apple"\r\n\r\n'
        "vert\r\n"
        f"--{CGI_UPLOAD_BOUNDARY}\r\n"
        'Content-Disposition: form-data; name="x"\r\n\r\n'
        "1\r\n"
        f"--{CGI_UPLOAD_BOUNDARY}\r\n"
        f'Content-Disposition: form-data; name="up"; filename="{filename}"\r\n'
        "Content-Type: application/octet-stream\r\n\r\n"
    ).encode("ascii")
    return head + content + f"\r\n--{CGI_UPLOAD_BOUNDARY}--\r\n".encode("ascii")


CGI_UPLOAD_BODY = multipart_body("upload.txt", CGI_UPLOAD_BYTES)
CGI_UPLOAD_EXPECT = ("method=POST", f"ctype={CGI_UPLOAD_TYPE}",
                     "apple=vert", "x=1", "upname=upload.txt", "uplen=8",
                     "cgi_upload : OK")

CGI_BINARY_NAME = "binary.bin"
CGI_BINARY_BYTES = bytes(range(256)) * 256
CGI_BINARY_BODY = multipart_body(CGI_BINARY_NAME, CGI_BINARY_BYTES)
CGI_BINARY_EXPECT = ("method=POST", f"ctype={CGI_UPLOAD_TYPE}",
                     "apple=vert", "x=1", f"upname={CGI_BINARY_NAME}",
                     f"uplen={len(CGI_BINARY_BYTES)}", "cgi_upload : OK")

# Rejet au-dela de FILE_MAX (0xA00000, 10 Mo, valeur par defaut du module).
# Le fichier fait un octet de trop : le controle de taille du module cgi
# tombe apres la lecture de ce dernier octet, donc la frontiere finale du
# multipart n'est jamais lue. Le module rend CGI_ERROR_FILEMAX (0x00080008)
# pendant son demarrage : le script ne s'execute pas, il n'y a ni en-tete
# CGI, ni fichier sauve. cgi_upload.bas releve cgi$contentmax, sans quoi le
# corps entier serait refuse avant ce controle (CGI_ERROR_BIG_CONTENT,
# 0x00080002) ; cgi$filemax reste a sa valeur par defaut.
CGI_FILE_MAX = 0xA00000
CGI_FILEMAX_CODE = 0x00080008
CGI_TOO_BIG_NAME = "trop-gros.bin"
CGI_TOO_BIG_BYTES = bytes(CGI_FILE_MAX + 1)
CGI_TOO_BIG_BODY = multipart_body(CGI_TOO_BIG_NAME, CGI_TOO_BIG_BYTES)
# Depuis E46 (corrige), le module fournit son message sur tous les chemins
# et sur les deux plateformes : le rapporteur affiche donc son texte, et le
# code complet reste l'attente commune (POSIX ne transmet que 8 bits de
# code de sortie, le verifier le relit dans la page quand il faut).
CGI_TOO_BIG_EXPECT = ("error 0x00080008",)
CGI_TOO_BIG_EXPECT_HTTPD = CGI_TOO_BIG_EXPECT + (
    "Extension specific error: File is larger than the configured "
    "allowed size.",)

# Second refus, avec 256 Ko au-dela de la limite. Le module a deja lu tout ce
# qu'il pouvait quand il refuse : le reste du corps est encore dans la socket
# du serveur. Sans consommation de ce reste, sbhttpd ferme une socket non
# vide, le noyau envoie un RST et le client perd le 413. Ce cas est donc le
# seul qui distingue une fermeture propre d'une fermeture brutale.
CGI_TOO_BIG_OVERSHOOT = 0x40000
CGI_TOO_BIG_LONG_NAME = "enorme.bin"
CGI_TOO_BIG_LONG_BYTES = bytes(CGI_FILE_MAX + CGI_TOO_BIG_OVERSHOOT)
CGI_TOO_BIG_LONG_BODY = multipart_body(CGI_TOO_BIG_LONG_NAME,
                                       CGI_TOO_BIG_LONG_BYTES)

# Troisieme refus : le corps annonce depasse CONTENT_MAX (0xA00000, meme
# valeur que FILE_MAX par defaut). Le module le voit dans l'en-tete
# Content-Length et rend CGI_ERROR_BIG_CONTENT (0x00080002) sans lire un seul
# octet : c'est la protection contre les corps demesures. Le test annonce
# donc la taille sans envoyer le corps, ce qui laisse la socket du serveur
# vide et permet de lire le 413 sans competition avec l'envoi.
# cgi_post.bas ne releve pas cgi$contentmax : la limite testee est celle par
# defaut, contrairement aux cas multipart qui passent par cgi_upload.bas.
CGI_CONTENT_MAX = 0xA00000
CGI_BIG_CONTENT_CODE = 0x00080002
CGI_BIG_CONTENT_DECLARED = CGI_CONTENT_MAX + 1
CGI_BIG_CONTENT_EXPECT = ("error 0x00080002",)
CGI_BIG_CONTENT_EXPECT_HTTPD = CGI_BIG_CONTENT_EXPECT + (
    "Extension specific error: http request is larger than the allowed "
    "and configured maximal size.",)

# Erreur d'execution, pour figer ERRATA E24 : HttpProc passait le code
# d'erreur comme mot de drapeaux a httpd_report, qui y teste REPORT_F_FRST
# (0x2). Un code dont ce bit est leve perdait donc la ligne de statut, et le
# client levait BadStatusLine. 22 = COMMAND_ERROR_FILE_CANNOT_BE_OPENED
# (0x16) : 22 & 0x2 vaut 2, et l'erreur tombe a l'execution, apres le
# chargement, donc c'est bien httpd_report qui ecrit la reponse.
CGI_RUNTIME_CODE = 22
# Le script n'a rien ecrit : les deux rapporteurs peuvent encore ouvrir leur
# page. Cote serveur c'est un 500 ; cote CLI c'est la page CGI du rapporteur,
# avec sa ligne `Status: 200 OK` (ERRATA E25).
CGI_RUNTIME_EXPECT = ("Status: 200 OK", "Error has happened in the code",
                      "error &H16")
CGI_RUNTIME_EXPECT_HTTPD = ("Error has happened in the code", "error &H16")

# Erreur de chargement : l'include n'existe pas, l'erreur tombe avant toute
# execution et c'est le lecteur qui la rapporte. En mode `scriba -c` le
# rapporteur du CLI ecrit une page complete sur stderr et le processus sort
# avec 0, comme sur toute erreur de syntaxe. Cote serveur, httpd_report ecrit
# la meme page, en 200.
CGI_LOAD_EXPECT = ("Status: 200 OK", "error &H44")
CGI_LOAD_EXPECT_HTTPD = ("Error has happened in the code", "error &H44")

# Erreur apres le debut de la reponse (ERRATA E25) : le script ecrit sa
# ligne de statut et un morceau de corps, puis echoue. Le serveur ne peut
# plus choisir ni le statut ni le type : il ne doit ajouter que le
# diagnostic, sans seconde ligne de statut, sans en-tete, sans page HTML.
# Le texte « <HTML> » et « Content-Type: text/html » sont donc interdits
# dans les deux modes, et « HTTP/1.0 » cote sbhttpd, puisque le script a
# annonce text/plain et deja ecrit sa propre ligne de statut.
CGI_PARTIAL_EXPECT = ("debut", "error &H16")
# Les deux rapporteurs suivent la meme regle : en externe le CLI ajoute le
# diagnostic sans page, en interne le serveur ajoute en plus un saut de ligne.
CGI_PARTIAL_FORBID = ("<HTML>", "Content-Type: text/html")
CGI_PARTIAL_FORBID_HTTPD = ("HTTP/1.0",) + CGI_PARTIAL_FORBID

# Erreur d'un module a l'execution, au-dela de MAX_ERROR_CODE (133) :
# cgi::SaveFile sur un nom qui n'est pas un fichier televerse rend
# 0x00080000. Le rapporteur imprime le code en 0x%08x et remplace le message
# par « Extension specific error: ... », complete par celui du module.
CGI_MODULE_CODE = 0x00080000
CGI_MODULE_EXPECT = ("Status: 200 OK", "Error has happened in the code",
                     "error 0x00080000")
CGI_MODULE_EXPECT_HTTPD = ("Error has happened in the code",
                           "error 0x00080000",
                           "The named file is not uploaded.")

CGI_POST_CASES = (
    {
        "label": "urlencoded",
        "script": "cgi_post.bas",
        "body": CGI_POST_BODY,
        "content_type": "application/x-www-form-urlencoded",
        "expect": CGI_POST_EXPECT,
        "query": "q=1",
        "save": None,
        "save_content": None,
    },
    {
        "label": "multipart",
        "script": "cgi_upload.bas",
        "body": CGI_UPLOAD_BODY,
        "content_type": CGI_UPLOAD_TYPE,
        "expect": CGI_UPLOAD_EXPECT,
        "query": f"q=1&uplen={len(CGI_UPLOAD_BYTES)}&upname=upload.txt",
        # cgi_upload.bas retombe sur ce nom dans le repertoire courant de
        # sbhttpd ; en mode externe, CGI_UPLOAD_SAVE designe un chemin absolu.
        "save": "uploaded.bin",
        "save_content": CGI_UPLOAD_BYTES,
    },
    {
        "label": "multipart-binaire",
        "script": "cgi_upload.bas",
        "body": CGI_BINARY_BODY,
        "content_type": CGI_UPLOAD_TYPE,
        "expect": CGI_BINARY_EXPECT,
        "query": f"q=1&uplen={len(CGI_BINARY_BYTES)}&upname={CGI_BINARY_NAME}",
        "save": "uploaded.bin",
        "save_content": CGI_BINARY_BYTES,
    },
    {
        "label": "multipart-filemax",
        "script": "cgi_upload.bas",
        "body": CGI_TOO_BIG_BODY,
        "content_type": CGI_UPLOAD_TYPE,
        "expect": CGI_TOO_BIG_EXPECT,
        "expect_httpd": CGI_TOO_BIG_EXPECT_HTTPD,
        # Le module echoue avant la premiere ligne du script : sbhttpd
        # n'a rien envoye et repond donc 413, pas la page d'erreur en 200
        # des autres echecs ([ERRATA E23](../docs/ERRATA.md#e23)).
        "status": 413,
        # Rien ne doit annoncer un succes, et aucun fichier ne doit etre
        # sauve.
        "forbid": ("cgi_upload : OK",),
        "code": CGI_FILEMAX_CODE,
        "query": f"q=1&uplen={len(CGI_TOO_BIG_BYTES)}&upname={CGI_TOO_BIG_NAME}",
        "save": None,
        "save_content": None,
        # 10 Mo comptes octet par octet par le module et, en interne, lus par
        # sbhttpd : ce cas demande plus que les trois autres.
        "timeout": 180,
    },
    {
        "label": "multipart-filemax-256k",
        "script": "cgi_upload.bas",
        "body": CGI_TOO_BIG_LONG_BODY,
        "content_type": CGI_UPLOAD_TYPE,
        "expect": CGI_TOO_BIG_EXPECT,
        "expect_httpd": CGI_TOO_BIG_EXPECT_HTTPD,
        "status": 413,
        "forbid": ("cgi_upload : OK",),
        "code": CGI_FILEMAX_CODE,
        "query": (f"q=1&uplen={len(CGI_TOO_BIG_LONG_BYTES)}"
                  f"&upname={CGI_TOO_BIG_LONG_NAME}"),
        "save": None,
        "save_content": None,
        "timeout": 180,
    },
    {
        "label": "contentmax",
        "script": "cgi_post.bas",
        "body": b"",
        # Annonce 1 octet de plus que CONTENT_MAX, sans envoyer de corps.
        "declared": CGI_BIG_CONTENT_DECLARED,
        "content_type": "application/x-www-form-urlencoded",
        "expect": CGI_BIG_CONTENT_EXPECT,
        "expect_httpd": CGI_BIG_CONTENT_EXPECT_HTTPD,
        "status": 413,
        "forbid": ("cgi_post : OK", "cgi_upload : OK"),
        "code": CGI_BIG_CONTENT_CODE,
        "query": "q=1",
        "save": None,
        "save_content": None,
    },
    {
        "label": "erreur-execution",
        "script": "err_runtime.bas",
        # Le programme echoue sur sa premiere instruction : aucun corps n'est
        # lu par le module cgi, qui n'est meme pas importe.
        "body": b"",
        "content_type": "application/x-www-form-urlencoded",
        "expect": CGI_RUNTIME_EXPECT,
        "expect_httpd": CGI_RUNTIME_EXPECT_HTTPD,
        # Le script n'a rien ecrit : le serveur peut encore choisir le statut.
        "status": 500,
        "code": CGI_RUNTIME_CODE,
        "query": "q=1",
        "save": None,
        "save_content": None,
    },
    {
        "label": "erreur-chargement",
        "script": "err_load.bas",
        "body": b"",
        "content_type": "application/x-www-form-urlencoded",
        "expect": CGI_LOAD_EXPECT,
        "expect_httpd": CGI_LOAD_EXPECT_HTTPD,
        # En interne l'erreur de chargement arrive avant toute sortie : 500.
        # En externe le CLI reste en page CGI « Status: 200 OK » (voir
        # CGI_LOAD_EXPECT), c'est son rapporteur qui decide.
        "status": 500,
        # En externe, l'erreur arrive avant l'execution : le CLI repond une
        # page CGI sur stderr et sort avec 0. La ligne « error &H.. » y est
        # donc attendue, pas subie.
        "cgi_page": True,
        "forbid": ("jamais atteint",),
        "query": "q=1",
        "save": None,
        "save_content": None,
    },
    {
        "label": "erreur-ligne-en-attente",
        "script": "err_pending.bas",
        "body": b"",
        "content_type": "application/x-www-form-urlencoded",
        "expect": ("debut", "error &H16"),
        "expect_httpd": ("Error has happened in the code", "error &H16"),
        # Rien n'est encore parti cote serveur : la premiere ligne attend dans
        # le tampon, donc il choisit encore son statut. Cote CLI, chaque octet
        # part aussitot : « debut » est deja sorti, donc pas de page.
        "status": 500,
        "code": CGI_RUNTIME_CODE,
        "forbid": ("jamais atteint",) + CGI_PARTIAL_FORBID,
        "forbid_httpd": ("jamais atteint",),
        "query": "q=1",
        "save": None,
        "save_content": None,
    },
    {
        "label": "erreur-apres-sortie",
        "script": "err_partial.bas",
        "body": b"",
        "content_type": "application/x-www-form-urlencoded",
        "expect": CGI_PARTIAL_EXPECT,
        "expect_httpd": CGI_PARTIAL_EXPECT,
        # La reponse a deja commence : son statut est celui que le script a
        # ecrit, et le serveur n'a plus le choix.
        "status": 200,
        "code": CGI_RUNTIME_CODE,
        "forbid_httpd": CGI_PARTIAL_FORBID_HTTPD,
        "forbid": ("jamais atteint",) + CGI_PARTIAL_FORBID,
        "query": "q=1",
        "save": None,
        "save_content": None,
    },
    {
        "label": "erreur-module",
        "script": "err_module.bas",
        "body": b"",
        "content_type": "application/x-www-form-urlencoded",
        "expect": CGI_MODULE_EXPECT,
        "expect_httpd": CGI_MODULE_EXPECT_HTTPD,
        "status": 500,
        "code": CGI_MODULE_CODE,
        "forbid": ("jamais atteint",),
        "query": "q=1",
        "save": None,
        "save_content": None,
    },
)

# Manifeste des 32 fichiers de tests/upstream. Il est volontairement separe
# du lanceur : ajouter ou requalifier un test ne demande pas de toucher au
# code Python, et un fichier non declare ne peut pas disparaitre du jeu.
UPSTREAM_MANIFEST = ROOT / "tests" / "upstream" / "MANIFEST.tsv"
UPSTREAM_COLUMNS = ("chemin", "categorie", "modules", "os", "repertoire",
                    "mode", "attendu", "raison")
UPSTREAM_MODES = ("defaut", "all", "jamais")
UPSTREAM_CATEGORIES = ("coeur", "module", "reseau", "interactif",
                       "diagnostic")

# Sur Windows, un binaire Linux ne s'execute que par WSL.
WSL_DISTRO = os.environ.get("WSL_DISTRO", "Ubuntu-26.04")


def infer_target(exe):
    """Deduit la cible du dossier dist/<cible> quand --exe est utilise."""
    name = exe.parent.name
    if name.startswith(("windows-", "linux-", "macos-")):
        return name
    return None


def target_family(target):
    """Rend 'windows' ou 'unix' pour filtrer le manifeste upstream."""
    if target:
        family = target.split("-")[0]
        if family in ("windows", "linux", "macos"):
            return "windows" if family == "windows" else "unix"
    return "windows" if os.name == "nt" else "unix"


def module_suffix(target):
    """Rend l'extension des modules de la cible."""
    if target:
        if target.startswith("windows"):
            return ".dll"
        if target.startswith("macos"):
            return ".dylib"
        if target.startswith("linux"):
            return ".so"
    if sys.platform == "win32":
        return ".dll"
    if sys.platform == "darwin":
        return ".dylib"
    return ".so"


def read_upstream_manifest(path=None):
    """Lit le manifeste TSV et rend une liste de dictionnaires."""
    path = path or UPSTREAM_MANIFEST
    rows = []
    seen = set()
    for lineno, line in enumerate(path.read_text(encoding="utf-8").splitlines(),
                                  1):
        if not line.strip() or line.lstrip().startswith("#"):
            continue
        fields = [field.strip() for field in line.split("\t")]
        if len(fields) != len(UPSTREAM_COLUMNS):
            raise ValueError(f"{path}:{lineno}: {len(fields)} colonnes, "
                             f"{len(UPSTREAM_COLUMNS)} attendues")
        row = dict(zip(UPSTREAM_COLUMNS, fields))
        if row["mode"] not in UPSTREAM_MODES:
            raise ValueError(f"{path}:{lineno}: mode inconnu {row['mode']!r}")
        if row["categorie"] not in UPSTREAM_CATEGORIES:
            raise ValueError(f"{path}:{lineno}: categorie inconnue "
                             f"{row['categorie']!r}")
        if row["attendu"] not in ("ok", "-"):
            raise ValueError(f"{path}:{lineno}: attendu inconnu "
                             f"{row['attendu']!r}")
        if row["mode"] != "jamais" and row["attendu"] != "ok":
            raise ValueError(f"{path}:{lineno}: un test lance doit avoir "
                             "attendu=ok")
        if row["mode"] == "jamais" and row["raison"] in ("", "-"):
            raise ValueError(f"{path}:{lineno}: un test jamais lance doit "
                             "avoir une raison")
        if row["os"] not in ("tous", "windows", "unix"):
            raise ValueError(f"{path}:{lineno}: os inconnu {row['os']!r}")
        if row["repertoire"] not in ("racine", "dist"):
            raise ValueError(f"{path}:{lineno}: repertoire inconnu "
                             f"{row['repertoire']!r}")
        if row["chemin"] in seen:
            raise ValueError(f"{path}:{lineno}: doublon {row['chemin']!r}")
        seen.add(row["chemin"])
        rows.append(row)
    return rows


def run_upstream(exe, target, run_all, timeout):
    """Lance les tests decrits par le manifeste upstream."""
    try:
        rows = read_upstream_manifest()
    except (OSError, ValueError) as exc:
        print(f"manifeste upstream illisible : {exc}")
        return 1

    family = target_family(target)
    suffix = module_suffix(target)
    config = exe.parent / "scriba.conf"
    run_count = failures = skipped = 0

    print(f"binaire : {exe}")
    print(f"manifeste : {UPSTREAM_MANIFEST.relative_to(ROOT)}")

    # Le manifeste doit couvrir tous les .bas presents : un fichier ajoute
    # sans ligne ne doit pas disparaitre du jeu sans que personne le voie.
    declared = {row["chemin"] for row in rows}
    present = {path.relative_to(UPSTREAM_MANIFEST.parent).as_posix()
               for path in UPSTREAM_MANIFEST.parent.rglob("*.bas")}
    for name in sorted(present - declared):
        failures += 1
        print(f"  ECHEC   {name} (absent du manifeste)")

    for row in rows:
        name = row["chemin"]
        script = ROOT / "tests" / "upstream" / name
        if not script.exists():
            failures += 1
            print(f"  ECHEC   {name} (fichier absent)")
            continue

        reason = row["raison"]
        if row["mode"] == "jamais":
            skipped += 1
            print(f"  SAUTE   {name} : {reason}")
            continue
        if row["mode"] == "all" and not run_all:
            skipped += 1
            print(f"  SAUTE   {name} : seulement avec --upstream-all")
            continue
        if row["os"] != "tous" and row["os"] != family:
            skipped += 1
            print(f"  SAUTE   {name} : reserve a {row['os']}")
            continue

        required = [] if row["modules"] in ("", "-") else [
            module.strip() for module in row["modules"].split(",")]
        missing = [module for module in required
                   if not (exe.parent / "modules" /
                           (module + suffix)).exists()]
        if missing:
            skipped += 1
            print(f"  SAUTE   {name} : module(s) non construit(s) : "
                  f"{', '.join(missing)}")
            continue

        workdir = exe.parent if row["repertoire"] == "dist" else ROOT
        if row["repertoire"] == "dist" and not config.exists():
            skipped += 1
            print(f"  SAUTE   {name} : scriba.conf absent a cote du binaire")
            continue

        try:
            cmd, cwd, env = command(exe, target, script, workdir, config,
                                    TEST_ENV.get(script.stem))
        except RuntimeError as exc:
            skipped += 1
            print(f"  SAUTE   {name} : {exc}")
            continue

        try:
            proc = subprocess.run(cmd, cwd=cwd, env=env, capture_output=True,
                                  text=True, errors="replace", timeout=timeout)
        except subprocess.TimeoutExpired:
            run_count += 1
            failures += 1
            print(f"  ECHEC   {name} (delai de {timeout} s depasse)")
            continue

        output = proc.stdout + proc.stderr
        # ScriptBasic rend 0 meme sur erreur : le code de sortie ne suffit pas.
        problems = []
        if proc.returncode != 0:
            problems.append(f"code {proc.returncode}")
        if "ECHEC" in proc.stdout:
            problems.append("ECHEC dans la sortie")
        if "error &H" in output or "error 0x" in output:
            problems.append("erreur du rapporteur")

        run_count += 1
        if problems and row["attendu"] == "ok":
            failures += 1
            print(f"  ECHEC   {name} ({', '.join(problems)})")
            for line in output.strip().splitlines()[-4:]:
                print(f"          {line}")
        else:
            print(f"  OK      {name}")

    print()
    print(f"{run_count} lance(s), {failures} echec(s), {skipped} saute(s)")
    if run_count == 0:
        print("aucun test upstream n'a pu etre lance")
        return 1
    return 0 if failures == 0 else 1


def to_wsl(path):
    p = Path(path).resolve().as_posix()
    if len(p) > 2 and p[1] == ":":
        p = "/mnt/" + p[0].lower() + p[2:]
    return p


def command(exe, target, script, workdir, config, env_extra=None):
    """Rend (commande, repertoire de travail, environnement)."""
    unix = bool(target) and target.startswith(("linux", "macos"))
    env_extra = env_extra or {}
    if unix and os.name == "nt":
        if not target.startswith("linux"):
            raise RuntimeError(f"{target} ne peut pas s'executer sous WSL")
        prefix = ""
        for key, value in env_extra.items():
            prefix += f"{key}='{value}' "
        if config and config.exists():
            prefix += f"SCRIBACONF='{to_wsl(config)}' "
        inner = (f"cd '{to_wsl(workdir)}' && {prefix}"
                 f"'{to_wsl(exe)}' '{to_wsl(script)}'")
        return (["wsl.exe", "-d", WSL_DISTRO, "--", "sh", "-c", inner],
                str(ROOT), None)

    env = dict(os.environ)
    env.update(env_extra)
    if unix and config and config.exists():
        # Sur UNIX la configuration n'est pas cherchee a cote de l'executable :
        # c'est SCRIBACONF (ou /etc/scriba/basic.conf) qui la designe.
        env["SCRIBACONF"] = str(config)
    return [str(exe), str(script)], str(workdir), env


def free_port():
    """Choisit un port libre sur la boucle locale.

    Le serveur de test ne peut pas prendre 18080 : une autre instance peut
    deja l'occuper. Le port est relache avant le demarrage ; le risque de
    course est limite a cette fenetre et le test echoue clairement si le
    serveur ne peut pas se lier.
    """
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.bind(("127.0.0.1", 0))
        return sock.getsockname()[1]


def tail(text, limit=2000):
    """Rend la fin d'un texte, pour ne pas noyer la sortie d'un echec."""
    text = text.strip()
    if len(text) <= limit:
        return text
    return "..." + text[-limit:]


def duplicate_report_problems(expect, output):
    """Verifie qu'un rapport n'est pas ecrit deux fois.

    Le moteur rapporte les erreurs de chargement et l'hote celles que
    `scriba_Run` rend ; jamais les deux pour la meme erreur, puisque
    `HttpProc` et le CLI sortent des qu'un chargement echoue (ERRATA E25).
    Ce controle fige la regle : un diagnostic attendu doit apparaitre une
    seule fois, et une reponse ne porte qu'une page d'erreur, meme quand
    elle a deja commence.
    """
    problems = []
    for line in expect:
        if line.startswith("error ") and output.count(line) > 1:
            problems.append(f"diagnostic en double : {line}")
    if output.count("<HTML>") > 1:
        problems.append("page d'erreur en double")
    return problems


def cgi_post_problems(code, output, expect, expect_status, expect_code=0,
                      cgi_page=False):
    """Verifie une reponse CGI POST et rend la liste des manques.

    T<expect_code> est le code de sortie attendu : 0 pour une reponse
    normale, CGI_ERROR_FILEMAX quand le module doit refuser le corps. Un
    refus attendu n'ecrit ni en-tete CGI ni ligne du script : seuls le code
    et les lignes de T<expect> sont alors exiges.

    T<cgi_page> dit que `scriba -c` a rapporte une erreur de chargement sous
    forme de page CGI : la ligne « error &H.. » de la sortie est alors
    attendue et ne doit pas etre comptee comme un echec du rapporteur.
    """
    problems = []
    if code != expect_code:
        # POSIX ne transmet que 8 bits de code de sortie : sous UNIX,
        # exit(0x00080008) rend 8, et un code etendu dont les 8 bits bas
        # sont nuls (0x00080000) devient 0, indiscernable d'un succes.
        # Le code complet, lui, est affiche noir sur blanc par le
        # rapporteur ("error 0x........") : on l'y relit avant de crier.
        lu = re.search(r"error 0x([0-9a-fA-F]{8})", output)
        if not (lu and int(lu.group(1), 16) == expect_code):
            problems.append(f"code {code}, {expect_code} attendu")
    for line in expect:
        if line not in output:
            problems.append(f"ligne absente : {line}")
    problems += duplicate_report_problems(expect, output)
    if expect_code:
        # Refus attendu : pas d'en-tete CGI, l'erreur est le resultat.
        return problems
    if expect_status and "Status: 200" not in output and \
            "HTTP/1.0 200" not in output:
        problems.append("en-tete 200 absent")
    if "ECHEC" in output:
        problems.append("ECHEC dans la sortie")
    if not cgi_page and ("error &H" in output or "error 0x" in output):
        problems.append("erreur du rapporteur")
    return problems


def cgi_httpd(exe, target, timeout, script_name, body, content_type, expect,
              query="q=1", save_name=None, save_content=None, forbid=(),
              expect_status=200, declared_length=None):
    """Sert un script CGI par sbhttpd et rend (problemes, details).

    sbhttpd cherche sa configuration a cote de l'executable sous Windows,
    sans lire SCRIBACONF. Pour ne pas toucher a dist/, le binaire est copie
    dans un dossier temporaire avec sa configuration compilee. Sous UNIX, la
    meme configuration est designee par SCRIBACONF et le binaire d'origine
    est utilise. Les modules sont references en chemin absolu, ce qui permet
    de lancer le serveur depuis le dossier temporaire.

    Si T<save_name> est fourni, le script doit sauver l'upload sous ce nom
    dans son repertoire courant (le dossier temporaire) ; le contenu est
    compare a T<save_content> quand il est fourni.

    T<forbid> liste des chaines qui doivent rester absentes du corps ou de
    la sortie du CLI ;
    T<expect_status> est le statut HTTP attendu (200 par defaut, 413 pour un
    corps trop gros). T<declared_length>, s'il est fourni, remplace la taille
    du corps dans l'en-tete `Content-Length` : le module peut ainsi refuser
    sur l'en-tete seul, sans qu'on lui envoie les octets annonces.
    La configuration du serveur de test ajoute
    `errmsgdest 3` : sans lui, sbhttpd n'envoie pas ses erreurs au client
    (valeur par defaut : journal seulement) et un refus du module cgi
    arriverait en corps vide.
    """
    windows = (target or "").startswith("windows") or (
        not target and sys.platform == "win32")
    server_name = "sbhttpd.exe" if windows else "sbhttpd"
    server_source = exe.parent / server_name
    if not server_source.is_file():
        return [f"{server_name} absent ; construisez-le d'abord : "
                f"python build.py --target {target or '<cible>'} --httpd"], ""

    problems = []
    details = []
    with tempfile.TemporaryDirectory(prefix="scriba-cgi-post-") as temp:
        tmp = Path(temp)
        bindir = tmp / "bin"
        home = tmp / "home"
        logs = tmp / "logs"
        bindir.mkdir()
        home.mkdir()
        logs.mkdir()
        script = ROOT / "tests" / script_name
        shutil.copy2(script, home / script_name)

        port = free_port()
        if windows:
            server = bindir / server_name
            shutil.copy2(server_source, server)
        else:
            server = server_source

        text = (ROOT / "scriba.conf.lsp").read_text(encoding="utf-8")
        modules = (exe.parent / "modules").as_posix()
        includes = (exe.parent / "modules" / "include").as_posix()
        text = re.sub(r'(?m)^dll\s+"[^"]*"',
                      f'dll "{module_suffix(target)}"', text)
        text = re.sub(r'(?m)^module\s+"[^"]*"',
                      f'module "{modules}/"', text)
        text = re.sub(r'(?m)^include\s+"[^"]*"',
                      f'include "{includes}/"', text)
        text = re.sub(r'(?m)^(\s*)port\s+\d+', rf'\1port {port}', text)
        # Erreurs du serveur envoyees au client ET au journal : le cas
        # multipart-filemax lit la page d'erreur pour verifier le code.
        text = re.sub(r'(?m)^(\s*)threads\s+(\d+)',
                      r'\1threads \2\n  errmsgdest 3', text)
        text = re.sub(r'(?m)^(\s*)home\s+"[^"]*"',
                      rf'\1home "{home.as_posix()}/"', text)
        text = re.sub(r'(?m)^\s*run404\s+.*$', '', text)
        text = text.replace('"logs/', f'"{logs.as_posix()}/')

        lsp = tmp / "cgi-post.conf.lsp"
        conf = bindir / "scriba.conf"
        lsp.write_text(text, encoding="utf-8", newline="")
        try:
            built = subprocess.run([str(exe), "-f", str(conf), "-k", str(lsp)],
                                   cwd=str(exe.parent), capture_output=True,
                                   text=True, errors="replace", timeout=timeout)
        except (OSError, subprocess.TimeoutExpired) as exc:
            return [f"configuration de test impossible : {exc}"], ""
        if built.returncode != 0 or not conf.is_file():
            return [f"configuration de test non compilee (code "
                    f"{built.returncode})"], tail(built.stdout + built.stderr)

        process = None
        stdout = stderr = ""
        status = None
        response_body = ""
        env = dict(os.environ)
        env["SCRIBACONF"] = str(conf)
        try:
            process = subprocess.Popen([str(server)], cwd=str(bindir), env=env,
                                       stdout=subprocess.PIPE,
                                       stderr=subprocess.PIPE, text=True,
                                       errors="replace")
            ready = False
            deadline = time.monotonic() + min(timeout, 15)
            while time.monotonic() < deadline:
                if process.poll() is not None:
                    break
                try:
                    with socket.create_connection(("127.0.0.1", port),
                                                  timeout=0.25):
                        ready = True
                        break
                except OSError:
                    time.sleep(0.1)
            if not ready:
                problems.append(f"sbhttpd n'a pas ouvert le port {port}")
            else:
                conn = http.client.HTTPConnection("127.0.0.1", port,
                                                  timeout=timeout)
                try:
                    if declared_length is None:
                        conn.request("POST", f"/{script_name}?{query}",
                                     body=body,
                                     headers={
                                         "Content-Type": content_type,
                                         "Content-Length": str(len(body)),
                                     })
                    else:
                        # En-tete seul : annoncer plus que CONTENT_MAX sans
                        # envoyer le corps laisse la socket du serveur vide,
                        # donc la fermeture reste propre et le 413 lisible.
                        conn.putrequest("POST",
                                        f"/{script_name}?{query}")
                        conn.putheader("Content-Type", content_type)
                        conn.putheader("Content-Length",
                                       str(declared_length))
                        conn.endheaders()
                        if body:
                            conn.send(body)
                    response = conn.getresponse()
                    status = response.status
                    response_body = response.read().decode("utf-8", "replace")
                finally:
                    conn.close()
                if status != expect_status:
                    problems.append(f"statut HTTP {status}, {expect_status} "
                                    "attendu")
                for line in expect:
                    if line not in response_body:
                        problems.append(f"ligne absente : {line}")
                problems += duplicate_report_problems(expect, response_body)
                if "ECHEC" in response_body:
                    problems.append("ECHEC dans le corps")
                for line in forbid:
                    if line in response_body:
                        problems.append(f"ligne inattendue : {line}")
                if save_name:
                    saved = bindir / save_name
                    if not saved.is_file():
                        problems.append(f"fichier sauve absent : {save_name}")
                    elif save_content is not None and \
                            saved.read_bytes() != save_content:
                        problems.append("contenu sauve incorrect")
        except (OSError, http.client.HTTPException) as exc:
            problems.append(f"requete POST impossible : {exc}")
        finally:
            if process is not None:
                if process.poll() is None:
                    # sbhttpd surveille ce fichier : le supprimer est l'arret
                    # propre prevu par la configuration, sans tuer le serveur.
                    try:
                        (logs / "pid.txt").unlink()
                    except OSError:
                        pass
                    try:
                        process.wait(timeout=5)
                    except subprocess.TimeoutExpired:
                        process.terminate()
                        try:
                            process.wait(timeout=5)
                        except subprocess.TimeoutExpired:
                            process.kill()
                            process.wait()
                try:
                    stdout, stderr = process.communicate(timeout=5)
                except (OSError, subprocess.TimeoutExpired):
                    pass

        if response_body:
            details.append("corps recu :\n" + tail(response_body))
        if stderr.strip():
            details.append("stderr sbhttpd :\n" + tail(stderr))
        if stdout.strip():
            details.append("stdout sbhttpd :\n" + tail(stdout))
        for name in ("panic.txt", "err.txt", "app.txt"):
            log = logs / name
            if log.is_file() and log.stat().st_size:
                details.append(f"{name} :\n" +
                               tail(log.read_text(encoding="utf-8",
                                                  errors="replace")))
    return problems, "\n".join(details)


def run_cgi_post(exe, target, timeout=30):
    """Verifie le POST CGI externe (scriba -c) et interne (sbhttpd).

    Onze cas passent par les deux chemins : urlencoded, multipart ASCII,
    multipart binaire, deux multipart depassant FILE_MAX (de 1 octet, puis de
    256 Ko), un corps annonce au-dela de CONTENT_MAX, et cinq erreurs
    (execution, chargement, module, ligne en attente, erreur apres le debut
    de la reponse).
    Les corps multipart sont envoyes en octets, sans conversion
    de fin de ligne ; le fichier sauve est relu sur disque, y compris pour
    65536 octets binaires contenant des zeros. Les trois cas de refus
    n'attendent aucun fichier : le module cgi refuse le corps avant la
    premiere ligne du script et rend CGI_ERROR_FILEMAX (0x00080008) ou
    CGI_ERROR_BIG_CONTENT (0x00080002). Les trois derniers executent
    `err_runtime.bas` (echec a l'execution, code 22), `err_load.bas` (include
    absent, erreur de chargement rapportee par le lecteur) et `err_module.bas`
    (0x00080000, au-dela de MAX_ERROR_CODE) : pour chacun, la reponse doit
    etre la page d'erreur du    serveur, avec son statut 500 (ERRATA E24) ; cote CLI, le
    rapporteur du script ecrit sa page `Status: 200 OK`.
    Les deux rapporteurs suivent la meme regle quand la reponse a commence
    (ERRATA E25) : `err_partial.bas` a deja ecrit son statut et un debut de
    corps quand il echoue, donc les deux modes ajoutent le diagnostic sans
    page ni en-tete (`forbid` et `forbid_httpd` interdisent `<HTML>` et
    `Content-Type: text/html`). `err_pending.bas` separe les deux modes : sa
    premiere ligne n'a pas de saut, donc sbhttpd la retient encore et doit
    repondre 500 avec la page complete, tandis que le CLI ecrit chaque octet
    aussitot : « debut » est deja parti, pas de page.
    """
    print(f"binaire : {exe}")
    if target and target.startswith(("linux", "macos")) and os.name == "nt":
        print("  SAUTE   cgi-post : cible non native ; le test ouvre un port "
              "local et ne passe pas par WSL")
        return 0

    config = exe.parent / "scriba.conf"
    suffix = module_suffix(target)
    module = exe.parent / "modules" / ("cgi" + suffix)
    total = len(CGI_POST_CASES) * 2
    failures = 0

    print("  --- CGI externe (scriba -c) ---")
    with tempfile.TemporaryDirectory(prefix="scriba-cgi-upload-") as temp:
        upload_save = Path(temp) / "uploaded.bin"
        for case in CGI_POST_CASES:
            label = f"cgi-post externe/{case['label']}"
            if not module.is_file():
                print(f"  ECHEC   {label} (module cgi absent : {module.name})")
                print(f"          construisez-le : python build.py --target "
                      f"{target or '<cible>'} --modules")
                failures += 1
                continue

            env = dict(os.environ)
            env.update(CGI_POST_ENV)
            env["QUERY_STRING"] = case["query"]
            env["CONTENT_LENGTH"] = str(case.get("declared",
                                                 len(case["body"])))
            env["CONTENT_TYPE"] = case["content_type"]
            if os.name != "nt" and config.is_file():
                env["SCRIBACONF"] = str(config)
            if case["save"]:
                try:
                    upload_save.unlink()
                except OSError:
                    pass
                env["CGI_UPLOAD_SAVE"] = str(upload_save)

            expect_code = case.get("code", 0)
            try:
                proc = subprocess.run(
                    [str(exe), "-c", str(ROOT / "tests" / case["script"])],
                    cwd=str(exe.parent), env=env, input=case["body"],
                    capture_output=True,
                    timeout=case.get("timeout", timeout))
                output = (proc.stdout + proc.stderr).decode("utf-8", "replace")
                problems = cgi_post_problems(proc.returncode, output,
                                             case["expect"], True,
                                             expect_code,
                                             case.get("cgi_page", False))
                for line in case.get("forbid", ()):
                    if line in output:
                        problems.append(f"ligne inattendue : {line}")
            except (OSError, subprocess.TimeoutExpired) as exc:
                problems = [str(exc)]
                output = ""

            if case["save"]:
                if not upload_save.is_file():
                    problems.append("fichier sauve absent")
                elif upload_save.read_bytes() != case["save_content"]:
                    problems.append("contenu sauve incorrect")

            if problems:
                failures += 1
                print(f"  ECHEC   {label} ({', '.join(problems)})")
                for line in tail(output).splitlines():
                    print(f"          {line}")
            else:
                print(f"  OK      {label}")

    print("  --- CGI interne (sbhttpd) ---")
    for case in CGI_POST_CASES:
        label = f"cgi-post interne/{case['label']}"
        problems, details = cgi_httpd(
            exe, target, case.get("timeout", timeout), case["script"],
            case["body"], case["content_type"],
            case.get("expect_httpd", case["expect"]),
            query=case["query"], save_name=case["save"],
            save_content=case["save_content"],
            forbid=case.get("forbid_httpd", case.get("forbid", ())),
            expect_status=case.get("status", 200),
            declared_length=case.get("declared"))
        if problems:
            failures += 1
            print(f"  ECHEC   {label} ({', '.join(problems)})")
            for line in details.splitlines():
                print(f"          {line}")
        else:
            print(f"  OK      {label}")

    print()
    print(f"{total - failures}/{total} cas CGI POST reussi(s)")
    return 0 if failures == 0 else 1


def run_standalone(exe, target, timeout=30, kind="standalone", modules=False):
    """Verifie l'executable autonome produit par --standalone ou --stndlone.

    Le lanceur ne le construit pas : il execute `hello_<kind>` tel quel, sans
    script ni compilateur, et exige les deux lignes de tests/hello.bas. C'est
    la difference avec un test ordinaire, qui recoit toujours un .bas.

    Avec modules=True, l'artifact attendu est
    `standalone_modules_standalone` (construit par
    `build.py --standalone --modules tests/standalone_modules.bas`) et la
    verification se fait depuis la racine du depot : le repertoire n'a ni
    modules/ ni scriba.conf, donc un succes prouve que les modules sont
    reellement colles dans l'executable (phase 6).
    """
    if modules:
        name = "standalone_modules_standalone"
    else:
        name = "hello_" + kind
    suffix = ".exe" if (target or "").startswith("windows") or (
        not target and os.name == "nt") else ""
    artifact = exe.parent / (name + suffix)
    print(f"binaire : {exe}")
    print(f"autonome : {artifact}")
    if not artifact.is_file():
        print(f"  ECHEC   {name} absent ; construisez-le d'abord : "
              f"python build.py --target <cible> --{kind}")
        return 1

    unix = target.startswith(("linux", "macos")) if target else \
        sys.platform != "win32"
    config = artifact.parent / "scriba.conf"
    try:
        if unix and os.name == "nt":
            if not target or not target.startswith("linux"):
                print("  ECHEC   cette cible ne peut pas s'executer sous WSL")
                return 1
            prefix = (f"SCRIBACONF='{to_wsl(config)}' "
                      if config.exists() else "")
            inner = (f"cd '{to_wsl(artifact.parent)}' && "
                     f"{prefix}'./{artifact.name}'")
            cmd = ["wsl.exe", "-d", WSL_DISTRO, "--", "sh", "-c", inner]
            cwd, env = str(ROOT), None
        else:
            env = dict(os.environ)
            if unix and config.exists():
                env["SCRIBACONF"] = str(config)
            cmd, cwd = [str(artifact)], str(artifact.parent)
        proc = subprocess.run(cmd, cwd=cwd, env=env, capture_output=True,
                              text=True, errors="replace", timeout=timeout)
    except (OSError, subprocess.TimeoutExpired) as exc:
        print(f"  ECHEC   {name} : {exc}")
        return 1

    if modules:
        cwd = str(ROOT)  # neutre : ni modules/ ni scriba.conf a cote
    output = proc.stdout + proc.stderr
    problems = []
    if proc.returncode != 0:
        problems.append(f"code {proc.returncode}")
    if modules:
        if "standalone modules : OK" not in output:
            problems.append("verdict des modules absents")
    else:
        if "Hello from ScriptBasic" not in output:
            problems.append("premiere ligne absente")
        if "Compilation reussie" not in output:
            problems.append("seconde ligne absente")
    if "ECHEC" in output or "error &H" in output or "error 0x" in output:
        problems.append("erreur du rapporteur")

    if problems:
        print(f"  ECHEC   {name} ({', '.join(problems)})")
        for line in output.strip().splitlines()[-4:]:
            print(f"          {line}")
        return 1
    print(f"  OK      {name}")
    return 0


def run_re_engine(exe, target, timeout=60):
    """Execute l'essai C direct du moteur Spencer.

    Le programme n'est pas un script BASIC : il est lie aux quatre fichiers
    du moteur et rend 0 seulement si tous ses essais passent. On exige donc
    le code 0, l'absence de ligne ECHEC et le verdict final.
    """
    suffix = ".exe" if (target or "").startswith("windows") or (
        not target and os.name == "nt") else ""
    artifact = exe.parent / ("re_engine" + suffix)
    print(f"binaire : {exe}")
    print(f"moteur  : {artifact}")
    if not artifact.is_file():
        print("  ECHEC   re_engine absent ; construisez-le d'abord : "
              "python build.py --target <cible> --re-engine")
        return 1

    unix = target.startswith(("linux", "macos")) if target else \
        sys.platform != "win32"
    try:
        if unix and os.name == "nt":
            if not target or not target.startswith("linux"):
                print("  ECHEC   cette cible ne peut pas s'executer sous WSL")
                return 1
            inner = (f"cd '{to_wsl(artifact.parent)}' && "
                     f"'./{artifact.name}'")
            cmd = ["wsl.exe", "-d", WSL_DISTRO, "--", "sh", "-c", inner]
            cwd, env = str(ROOT), None
        else:
            cmd, cwd, env = [str(artifact)], str(artifact.parent), dict(os.environ)
        proc = subprocess.run(cmd, cwd=cwd, env=env, capture_output=True,
                              text=True, errors="replace", timeout=timeout)
    except (OSError, subprocess.TimeoutExpired) as exc:
        print(f"  ECHEC   re_engine : {exc}")
        return 1

    output = proc.stdout + proc.stderr
    problems = []
    if proc.returncode != 0:
        problems.append(f"code {proc.returncode}")
    if "moteur re : OK" not in output:
        problems.append("verdict final absent")
    if "ECHEC" in output:
        problems.append("essai en echec")
    if problems:
        print(f"  ECHEC   re_engine ({', '.join(problems)})")
        for line in output.strip().splitlines()[-8:]:
            print(f"          {line}")
        return 1
    print("  OK      re_engine")
    return 0


# ---------------------------------------------------------------------------
# Controle statique de la grammaire
# ---------------------------------------------------------------------------

# Noms de la table des mots cles dont l'emplacement dans CommandFunction[] est
# vide sans que ce soit un defaut : ils n'atteignent jamais l'execution.
#   - CONST, VAR, GLOBAL : developpes par ipreproc.c avant la compilation ;
#   - MODULE, ENDMODULE : consommes par builder.c, qui compile le bloc a part ;
#   - LOCAL, LABEL, NLABEL, EMPTY, PRAGMA, GCONST : aucune regle de syntaxe ne
#     les accepte, ils sont refuses a l'analyse (error &H77), jamais executes.
PREPROCESSEUR_SANS_CORPS = {"CONST", "VAR", "GLOBAL", "MODULE", "ENDMODULE",
                            "LOCAL", "LABEL", "NLABEL", "EMPTY", "PRAGMA",
                            "GCONST"}


def _table_body(text, name):
    """Contenu de l'initialisation de `name[]` dans un source C."""
    opening = text.index("{", text.index(name + "[]"))
    depth = 0
    for i in range(opening, len(text)):
        if text[i] == "{":
            depth += 1
        elif text[i] == "}":
            depth -= 1
            if depth == 0:
                return text[opening:i]
    raise ValueError(f"accolades non equilibrees dans {name}[]")


def check_grammar():
    """Verifie qu'aucun nom de la grammaire ne pointe sur un corps vide.

    Controle purement statique : il lit src/syntax.c et src/syntax.h, donc il
    ne demande aucun binaire construit. Trois defauts le font echouer :

      - un nom dont l'emplacement dans CommandFunction[] vaut NULL, c'est-a-dire
        accepte a l'analyse et sans rien a appeler a l'execution (la classe de
        l'erreur 28) ;
      - un nom qui renvoie a un code CMD_ sans #define, ou hors de la table ;
      - un #define NOTIMP_xxx non nul, qui remplacerait un corps par
        NOTIMPLEMENTED (ERRATA E34).
    """
    syntax = (ROOT / "src" / "syntax.c").read_text(encoding="utf-8",
                                                    errors="replace")
    header = (ROOT / "src" / "syntax.h").read_text(encoding="utf-8",
                                                    errors="replace")

    codes = {name: int(number) for name, number
             in re.findall(r"^#define\s+(CMD_\w+)\s+(\d+)", header, re.M)}
    bodies = {int(number): name for name, number
              in re.findall(r"(\w+)\s*,\s*/\*\s*(\d+)\s*\*/",
                            _table_body(syntax, "CommandFunction"))}

    problems, armed, ok, allowed = [], [], 0, 0
    seen = {"nom": 0, "arite": 0, "interne": 0}

    def check(symbol, cmd, table, kind):
        nonlocal ok, allowed
        seen[kind] += 1
        head = f'{table} "{symbol}" -> {cmd}' if symbol else f"{table} -> {cmd}"
        number = codes.get(cmd)
        if number is None:
            problems.append(f"{head} : code sans #define")
            return
        body = bodies.get(number)
        expected = "COMMAND_" + cmd[len("CMD_"):]
        if body == expected:
            ok += 1
        elif symbol and symbol in PREPROCESSEUR_SANS_CORPS:
            allowed += 1
        else:
            problems.append(f"{head} : {body or 'emplacement absent'} "
                            f"au lieu de {expected}")

    for symbol, cmd in re.findall(r'\{\s*"([^"]*)"\s*,\s*(CMD_\w+)\s*\}',
                                  syntax):
        check(symbol, cmd, "nom", "nom")

    for table in ("BINARIES", "UNARIES"):
        for cmd in sorted(set(re.findall(r"CMD_\w+",
                                         _table_body(syntax, table)))):
            check(None, cmd, table, "arite")

    for cmd in re.findall(r"\{\s*(CMD_\w+)\s*,\s*\d+",
                          _table_body(syntax, "INTERNALFUNCTIONS")):
        check(None, cmd, "INTERNALFUNCTIONS", "interne")

    for path in sorted((ROOT / "src").rglob("*")):
        if path.suffix not in (".c", ".h"):
            continue
        text = path.read_text(encoding="utf-8", errors="replace")
        # Un NOTIMP_xxx cite dans un commentaire n'arme rien : on retire les
        # commentaires avant de chercher (mynotimp.c en contient un exemple).
        text = re.sub(r"/\*.*?\*/", "", text, flags=re.S)
        text = re.sub(r"//[^\n]*", "", text)
        for name, value in re.findall(r"#define\s+(NOTIMP_\w+)\s+(\d+)", text):
            if int(value) != 0:
                armed.append(f"{path.relative_to(ROOT).as_posix()} : "
                             f"{name} = {value}")

    print(f"grammaire : {seen['nom']} noms, {seen['arite']} codes d'arite, "
          f"{seen['interne']} fonctions internes")
    print(f"  {len(codes)} codes CMD_, {len(bodies)} emplacements dans "
          f"CommandFunction[]")
    print(f"  OK      {ok} reference(s) vers un corps")
    if allowed:
        print(f"  OK      {allowed} mot(s) du preprocesseur, sans corps et "
              f"jamais execute(s)")
    if armed:
        print(f"  ECHEC   {len(armed)} NOTIMP arme(s) :")
        for line in armed:
            print(f"          {line}")
    else:
        print("  OK      0 NOTIMP arme")
    if problems:
        print(f"  ECHEC   {len(problems)} nom(s) sans corps :")
        for line in problems:
            print(f"          {line}")
        print()
        print(f"grammaire : {len(problems)} nom(s) en echec, "
              f"{len(armed)} NOTIMP arme(s)")
        return 1
    print()
    print("grammaire : OK")
    return 0


def run_embed(exe, target):
    """Joue les hotes embed construits par --libscriba.

    Chaque programme de examples/embed/ prouve un aspect de l'embarquement
    (charger-executer, appeler une fonction BASIC avec argument, lire une
    variable) et affiche sa propre ligne de verdict, attendue ici. Le
    repertoire courant est examples/embed/ : les hotes ouvrent bonus.bas
    relatif. Aucune configuration n'est requise : le langage pur suffit.
    """
    famille = target.split("-")[0] if target else "windows"
    edir = exe.parent / "embed"
    attendu = {
        "lancer": "embed lancer : OK",
        "appeler": "embed appeler : OK - Double(21) = 42",
        "variable": "embed variable : OK - total = 21",
    }
    suffix = ".exe" if famille == "windows" else ""
    # Les .pdb que l'editeur de liens depose a cote ne sont pas des hotes :
    # on vise par nom, jamais par glob.
    hotes = [(nom, edir / (nom + suffix)) for nom in attendu
             if (edir / (nom + suffix)).exists()]
    if not hotes:
        print(f"aucun hote dans {edir} : construisez avec --libscriba")
        return 1

    travaux = ROOT / "examples" / "embed"
    failures = 0
    print(f"hotes : {edir}")
    for nom, hote in hotes:
        if famille == "linux" and sys.platform == "win32":
            def wsl(p):
                p = Path(p).resolve().as_posix()
                return "/mnt/" + p[0].lower() + p[2:] if p[1] == ":" else p
            inner = f"cd '{wsl(travaux)}' && '{wsl(hote)}'"
            cmd = ["wsl.exe", "-d", "Ubuntu-26.04", "--", "sh", "-c", inner]
            cwd = ROOT
        else:
            cmd = [str(hote)]
            cwd = travaux
        proc = subprocess.run(cmd, cwd=cwd, capture_output=True,
                              text=True, errors="replace")
        sortie = proc.stdout + proc.stderr
        if proc.returncode == 0 and attendu[nom] in proc.stdout:
            print(f"  OK      {nom}")
        else:
            failures += 1
            print(f"  ECHEC   {nom} (code {proc.returncode})")
            for ligne in sortie.strip().splitlines()[-3:]:
                print(f"          {ligne}")
    total = len(hotes)
    print(f"\n{total - failures}/{total} hote(s) embed respecte(s)")
    return 0 if failures == 0 else 1


def run_contrat(exe, target):
    """Joue les contrats snapshot de tests/contrat/.

    Un contrat est un couple <nom>.bas + <nom>.out : la sortie standard
    attendue, octet pour octet apres normalisation des fins de ligne, et
    un <nom>.rc optionnel pour le code de sortie (ScriptBasic rend 0 meme
    en erreur, E3 : le .rc ne sert qu'aux harnais qui plantent expres,
    comme manuel). Les .out sont regeneres par l'interpreteur, jamais
    ecrits a la main ; toute divergence est une modification de
    comportement a assumer consciemment.
    """
    cdir = ROOT / "tests" / "contrat"
    scripts = sorted(cdir.glob("*.bas"))
    if not scripts:
        print(f"aucun contrat dans {cdir}")
        return 1
    config = exe.parent / "scriba.conf"
    failures = 0
    print(f"binaire : {exe}")
    for script in scripts:
        nom = script.stem
        out_file = script.with_suffix(".out")
        if not out_file.exists():
            print(f"  ECHEC   {nom} : {out_file.name} absent "
                  f"(regenerez-le, ne l'editez pas)")
            failures += 1
            continue
        attendu_rc = 0
        rc_file = script.with_suffix(".rc")
        if rc_file.exists():
            attendu_rc = int(rc_file.read_text(encoding="utf-8").strip())
        cmd, cwd, env = command(exe, target, script, ROOT, config, None)
        proc = subprocess.run(cmd, cwd=cwd, env=env, capture_output=True)
        obtenu = proc.stdout.replace(b"\r\n", b"\n")
        attendu = out_file.read_bytes().replace(b"\r\n", b"\n")
        if proc.returncode == attendu_rc and obtenu == attendu:
            print(f"  OK      {nom}")
        else:
            failures += 1
            if proc.returncode != attendu_rc:
                raison = f"code {proc.returncode} (attendu {attendu_rc})"
            else:
                raison = "sortie differente"
            print(f"  ECHEC   {nom} ({raison})")
            lo = obtenu.split(b"\n")
            la = attendu.split(b"\n")
            for i in range(max(len(lo), len(la))):
                l1 = lo[i] if i < len(lo) else b"<fin>"
                l2 = la[i] if i < len(la) else b"<fin>"
                if l1 != l2:
                    print(f"          ligne {i + 1} attendue : {l2[:60]!r}")
                    print(f"                  obtenue : {l1[:60]!r}")
                    break
    total = len(scripts)
    print(f"\n{total - failures}/{total} contrat(s) respecte(s)")
    return 0 if failures == 0 else 1


def main():
    ap = argparse.ArgumentParser(description="Tests ScriptBasic.")
    ap.add_argument("--target", help="cible construite dans dist/")
    ap.add_argument("--exe", help="chemin direct du binaire")
    ap.add_argument("--test", action="append", help="test a lancer (repetable)")
    ap.add_argument("--modules", action="store_true",
                    help="ajoute les tests des modules (hash, re, t, trial)")
    ap.add_argument("--upstream", action="store_true",
                    help="lance le sous-ensemble decrit par tests/upstream/MANIFEST.tsv")
    ap.add_argument("--upstream-all", action="store_true",
                    help="--upstream plus les essais longs et les diagnostics")
    ap.add_argument("--upstream-timeout", type=int, default=30,
                    help="delai par test upstream, en secondes (defaut 30)")
    ap.add_argument("--standalone", action="store_true",
                    help="verifie hello_standalone produit par build.py --standalone")
    ap.add_argument("--stndlone", action="store_true",
                    help="verifie hello_stndlone produit par build.py --stndlone")
    ap.add_argument("--re-engine", action="store_true",
                    help="execute l'essai C du moteur produit par build.py --re-engine")
    ap.add_argument("--cgi-post", action="store_true",
                    help="verifie le POST CGI par scriba -c et par sbhttpd")
    ap.add_argument("--cgi-post-timeout", type=int, default=30,
                    help="delai du test CGI POST, en secondes (defaut 30)")
    ap.add_argument("--contrat", action="store_true",
                    help="joue les contrats snapshot de tests/contrat/")
    ap.add_argument("--embed", action="store_true",
                    help="joue les hotes embed de dist/<cible>/embed/")
    ap.add_argument("--grammaire", action="store_true",
                    help="verifie statiquement que tout nom de la grammaire "
                         "a un corps (ne lance rien, ne demande aucun binaire)")
    args = ap.parse_args()

    if args.grammaire:
        if (args.target or args.exe or args.test or args.modules or
                args.upstream or args.upstream_all or args.standalone or
                args.stndlone or args.re_engine or args.cgi_post):
            ap.error("--grammaire ne se combine avec rien : il ne lit que les "
                     "sources")
        return check_grammar()

    if (args.upstream or args.upstream_all) and args.test:
        ap.error("--test ne se combine pas avec --upstream")
    if (args.standalone or args.stndlone) and (
            args.test or args.upstream or args.upstream_all):
        ap.error("--standalone/--stndlone ne se combinent pas avec "
                 "--test ni --upstream")
    if args.standalone and args.stndlone:
        ap.error("--standalone et --stndlone sont exclusifs")
    if args.re_engine and (args.test or args.modules or args.upstream or
                           args.upstream_all or args.standalone or
                           args.stndlone or args.cgi_post):
        ap.error("--re-engine ne se combine pas avec --test, --modules, "
                 "--upstream, --standalone, --stndlone ou --cgi-post")
    if args.cgi_post and (args.test or args.modules or args.upstream or
                          args.upstream_all or args.standalone or
                          args.stndlone):
        ap.error("--cgi-post ne se combine pas avec --test, --modules, "
                 "--upstream, --standalone ou --stndlone")
    if not args.target and not args.exe:
        sys.exit("indiquez --target ou --exe")

    if args.exe:
        exe = Path(args.exe)
        if not exe.is_absolute():
            exe = ROOT / exe
    else:
        exe = ROOT / "dist" / args.target / (
            "scriba.exe" if args.target.startswith("windows") else "scriba")

    target = args.target or infer_target(exe)
    if args.re_engine:
        return run_re_engine(exe, target)

    if not exe.exists():
        sys.exit(f"binaire introuvable : {exe}\nconstruisez-le d'abord : "
                 f"python build.py --target {args.target or '<cible>'}")

    if args.cgi_post:
        return run_cgi_post(exe, target, args.cgi_post_timeout)
    if args.standalone:
        return run_standalone(exe, target, modules=args.modules)
    if args.stndlone:
        return run_standalone(exe, target, kind="stndlone")
    if args.upstream or args.upstream_all:
        return run_upstream(exe, target, args.upstream_all,
                            args.upstream_timeout)
    if args.contrat:
        return run_contrat(exe, target)
    if args.embed:
        return run_embed(exe, target)

    extra = []
    if args.modules and target:
        family = target.split("-")[0]
        extra = MODULE_TESTS_BY_OS.get(
            "windows" if family == "windows" else "unix", [])
    manuel = []
    if target:
        family = target.split("-")[0]
        manuel = MANUEL_TESTS_BY_OS.get(
            "windows" if family == "windows" else "unix", [])
    names = args.test or (TESTS + manuel +
                          (MODULE_TESTS + extra if args.modules else []))
    config = exe.parent / "scriba.conf"
    failures = 0
    skipped = 0
    print(f"binaire : {exe}")
    for name in names:
        script = ROOT / "tests" / f"{name}.bas"
        if not script.exists():
            print(f"  ABSENT  {name}.bas")
            failures += 1
            continue
        module_test = name in MODULE_TESTS_ALL
        workdir = exe.parent if module_test else ROOT
        skip_reason = MODULE_TESTS_SKIP.get(name, {}).get(target)
        if skip_reason:
            print(f"  SAUTE   {name} : {skip_reason}")
            skipped += 1
            continue
        try:
            cmd, cwd, env = command(exe, target, script, workdir, config,
                                    TEST_ENV.get(name))
        except RuntimeError as exc:
            print(f"  SAUTE   {name} : {exc}")
            skipped += 1
            continue
        proc = subprocess.run(cmd, cwd=cwd, env=env, capture_output=True,
                              text=True, errors="replace")
        output = proc.stdout + proc.stderr
        # ScriptBasic rend 0 meme sur erreur : le code de sortie ne suffit pas.
        failed = (proc.returncode != 0 or "ECHEC" in proc.stdout
                  or "error &H" in output)
        if not failed:
            print(f"  OK      {name}")
        else:
            failures += 1
            reason = "code %d" % proc.returncode if proc.returncode else \
                     ("ECHEC dans la sortie" if "ECHEC" in proc.stdout
                      else "erreur du rapporteur")
            print(f"  ECHEC   {name} ({reason})")
            lines = output.strip().splitlines()
            for line in lines[-4:]:
                print(f"          {line}")
            if module_test:
                print(f"          modules construits ? python build.py "
                      f"--target {target or '<cible>'} --modules")

    # Les refus a l'analyse verifient le coeur du langage : ils ne sont
    # rattaches ni aux modules ni a un test nomme, pour que les comptes de
    # ces deux campagnes ne bougent pas.
    refus = [] if (args.modules or args.test) else REFUS_ANALYSE
    for script_name, expected in refus:
        script = ROOT / "tests" / script_name
        label = f"refus/{script.stem}"
        if not script.exists():
            print(f"  ABSENT  {script_name}")
            failures += 1
            continue
        try:
            cmd, cwd, env = command(exe, target, script, ROOT, config, None)
        except RuntimeError as exc:
            print(f"  SAUTE   {label} : {exc}")
            skipped += 1
            continue
        proc = subprocess.run(cmd, cwd=cwd, env=env, capture_output=True,
                              text=True, errors="replace")
        output = proc.stdout + proc.stderr
        if expected in output and "ECHEC" not in proc.stdout:
            print(f"  OK      {label}")
        else:
            failures += 1
            print(f"  ECHEC   {label} ({expected} attendu)")
            for line in output.strip().splitlines()[-4:]:
                print(f"          {line}")

    # Les sauts sortent du compte : un test non joue ne peut pas gonfler
    # le ratio. Leur nombre est affiche, avec leur raison, plus haut.
    total = len(names) + len(refus) - skipped
    print()
    compte = f"{total - failures}/{total} test(s) reussi(s)"
    if skipped:
        compte += f" ({skipped} saute(s) avec raison)"
    print(compte)
    return 0 if failures == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
