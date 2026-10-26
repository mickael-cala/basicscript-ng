# Programmes du dépôt — tableau récapitulatif

ScriptBasic n'est pas un seul binaire : c'est un **coeur** (`src/*.c`, 26
unités) et plusieurs **programmes** qui l'enveloppent, chacun avec son `main()`
ou sa fonction d'entrée. `build.py` construit `scriba`, `sbhttpd`, les
modules, l'exécutable autonome (`--standalone`), le runtime `stndlone`
(`--stndlone`), l'archive `libscriba.a` (`--libscriba`) et l'essai C du moteur
de `re` (`--re-engine`). `--standalone`
n'est pas un programme séparé, c'est une option de `scriba` qui produit un
exécutable autonome ; `stndlone` et `libscriba` sont les deux formes de
l'ancien chemin compilé. Les autres formes sont documentées ici : certaines
sont historiques, et quelques-unes ont été retirées le 2026-10-24 parce
qu'elles n'étaient ni constructibles ni utiles ici (voir § 2) ; la plupart
n'ont pas de cible.

Vocabulaire employé ici :

- **coeur** — `scriba.c`, `execute.c`, `lexer.c`, `syntax.c`, `builder.c`,
  `memory.c`, `modumana.c`… plus les commandes natives de `src/commands/` :
  c'est l'interpréteur lui-même, sans entrée ni sortie de programme.
- **variation** — le fichier qui apporte le `main()`, la licence et la liste
  des modules liés en statique. C'est le mot du projet d'origine pour ce que
  d'autres appellent un « hôte » ou un « front-end ».
- **module** (ou *extension*) — une bibliothèque partagée chargée à
  l'exécution par un `import` du programme BASIC (`re`, `hash`, `cgi`…).

---

## 1. Les programmes que `build.py` construit

| programme | fichier d'entrée | rôle | commande |
|---|---|---|---|
| `scriba` | `src/variations/standard/scribacmd.c` | l'interpréteur : lit un `.bas`, le compile en bytecode et l'exécute | `python build.py --target <cible>` |
| `sbhttpd` | `src/variations/httpd/websrv.c` | serveur HTTP **multi-thread** : chaque requête est servie par un thread qui exécute un script `.bas` | `python build.py --target <cible> --httpd` |
| **exécutable autonome** | `scriba -E` (aucune source propre) | copie de `scriba` avec le bytecode du script ajouté : le `.bas` n'est plus nécessaire à l'exécution | `python build.py --target <cible> --standalone [script]` |
| **runtime `stndlone`** | `src/stndlone.c` + C produit par `scriba -C` | second chemin autonome : le bytecode reste dans des tableaux C, `stndlone()` l'exécute ; ce n'est pas un binaire seul | `python build.py --target <cible> --stndlone [script]` |
| **`libscriba.a`** | coeur + `variations/standalone/libscriba.c` + `lmt_none.c` | archive statique pour un hôte C ; ce n'est pas un exécutable | `python build.py --target <cible> --libscriba` |
| **`re_engine`** | `tests/re_engine.c` + les quatre fichiers du moteur `re` | essai C direct du moteur Spencer : `REG_BASIC`, `REG_PEND`, références arrière, table `cnames` ; ce n'est pas un interpréteur | `python build.py --target <cible> --re-engine` |

État vérifié le 2026-10-24 :

| cible | `scriba` | `sbhttpd` | `--standalone` | `--stndlone` | `--libscriba` |
|---|---|---|---|---|---|
| `windows-x86` | compilé, testé | compilé et servi avec TCC (campagne précédente) ; cible Zig non essayée | **vérifié** (`hello_standalone.exe`) | **vérifié** (`hello_stndlone.exe`) | **archive construite** |
| `windows-x64` | compilé, testé | **compilé avec Zig et servi** (port 18080, page, page 404, 20 requêtes parallèles) | **vérifié** (`hello_standalone.exe`) | **vérifié** (`hello_stndlone.exe`) | **archive construite** |
| `linux-x64` | compilé, testé sous WSL | **compilé avec Zig et servi** (page `index.bas` par `curl`) | **vérifié** sous WSL | **vérifié** sous WSL | **archive construite** |
| `linux-arm64` | compilé (non exécuté) | non essayé | non exécuté ici | non exécuté ici | **archive construite** |
| `macos-x64` | compilé (non exécuté) | non essayé | non exécuté ici | non exécuté ici | **archive construite** |
| `macos-arm64` | compilé (non exécuté) | non essayé | non exécuté ici | non exécuté ici | **archive construite** |

`sbhttpd` a besoin de `scriba` construit pour la même cible : sa configuration
binaire (`dist/<cible>/scriba.conf`) est produite par l'interpréteur.
`--standalone`, `--stndlone` et `--libscriba` ont la même dépendance : ils
construisent `scriba` d'abord s'il manque. `--stndlone` exige en plus une
cible exécutable sur la machine hôte, puisque `scriba -C` doit tourner.

---

## 2. Les autres programmes et formes du dépôt

Les formes décrites ici n'ont pas toutes le même statut :

- **construit et vérifié** — `--standalone` (`scriba -E`), `--stndlone`
  (runtime `-C` + C genere) et `--libscriba` (archive statique).
- **source historique sans cible** — `lmt_cio` : conservé pour comprendre
  l'amont, jamais appelé par `build.py` ni par les tests.
- **formes retirées le 2026-10-24** — `basicdll`, `scribawx`, `lmt_wx` et
  `calldllbasic` : `basicdll.c` n'était qu'une déclaration sans corps de
  fonction ([E21](ERRATA.md#e21)), `scribawx` demandait wxWidgets
  ([E22](ERRATA.md#e22)) ; ces fichiers ne sont plus dans le dépôt.
- **outils et sources techniques non construits** — `testalloc`,
  `testconf`, `cftd`, `md5`, `mt19937`, `notimp`, `mynotimp` : ils ne sont
  ni des hôtes ni des formes historiques à construire.
- **essai C construit** — `re_engine`, cible `--re-engine` : il vérifie le
  moteur de `re` là où le module ne peut pas aller (mode basic, `REG_PEND`).

« Ce qui manque » dit ce qu'il faudrait pour transformer une ligne en cible de
`build.py`.

| nom | source | rôle | état | ce qui manque |
|---|---|---|---|---|
| **exécutable autonome** (`--standalone`, via `scriba -E`) | aucune : c'est une option de `scriba` | produit un `.exe`/binaire qui contient l'interpréteur **et** le bytecode du programme : le fichier `.bas` n'est plus nécessaire | **construit par `build.py --standalone`** ; `tests/hello.bas` donne `hello_standalone.exe` (PE x64 de 609 051 octets) et affiche le résultat attendu ; vérifié Windows x86, Windows x64 et Linux x64 | macOS à exécuter ; le chemin `-C` + `stndlone` est une autre cible (`--stndlone`) |
| **runtime autonome** (`stndlone`) | `src/stndlone.c` | runtime du chemin `-C` : `stndlone(argc, argv)` démarre un programme dont le bytecode est dans des tableaux C | **construit par `build.py --stndlone`** ; `tests/hello.bas` donne `hello_stndlone.exe` (601 600 octets) et affiche le résultat attendu ; vérifié Windows x86, Windows x64 et Linux x64 | macOS à exécuter ; ce runtime n'est pas un exécutable seul |
| **`libscriba`** (archive) | `src/variations/standalone/libscriba.c` + coeur + `lmt_none.c` | archive statique `libscriba.a` pour un hôte C ; `libscriba.c` porte un `main()` qui n'est tiré que si l'hôte le demande | **construite par `build.py --libscriba`** : 47 objets + `dist/<cible>/include/` ; archive testée avec un hôte C sous Windows x64 et Linux x64 | sous UNIX, l'hôte doit définir `char **_environ;` pour ne pas tirer `libscriba.o` et son `main()` |
| **`lmt_cio`** | `src/lmt_cio.c` | variante de la « licence et modules » : liste de modules liés en statique pour l'hôte console | non construit | aucun hôte `cio` séparé ; conservé comme référence |
| **`notimp`**, **`mynotimp`** | `src/notimp.c`, `src/mynotimp.c` | implémentations vides des fonctions `*Impl` que la bibliothèque standard attend | absents de `SOURCES` (inutiles ici) | rien |
| **`testalloc`** | `src/testalloc.c` | allocateur instrumenté (`-D_DEBUG`) : sentinelles, comptage, bilan des fuites | non construit | une cible `--debug` (utile, voir [ROADMAP](ROADMAP.md)) |
| **`testconf`** | `src/testconf.c` | petit programme qui affiche où l'interpréteur cherche sa configuration | non construit | cible d'outil |
| **`md5`**, **`mt19937`** | `src/tools/md5.c`, `src/tools/mt19937.c` | implémentations de référence (MD5, Mersenne Twister), non utilisées par le build | non construits | cible d'outil si besoin |
| **`re_engine`** | `tests/re_engine.c` + `src/extensions/re/` | essai C direct du moteur Spencer : mode basic, `REG_PEND`, références arrière, table `cnames` | **construit par `python build.py --target <cible> --re-engine`** et lancé par `tests/run.py --re-engine` ; 17 essais, `moteur re : OK` sous Windows x86, Windows x64 et Linux x64 (WSL) | macOS à exécuter ; ce n'est pas un produit |
| **`regex_test`** | `src/extensions/re/regex_test.c` | ancien pilote interactif de George J. Carrette (entrée `regex_test.txt`) | non construit | gardé comme référence ; `tests/re_engine.c` est l'essai maintenu |

**Ce que `--standalone`, `--stndlone` et `--libscriba` changent depuis le
2026-10-24** — ils construisent trois formes différentes. `basicc` et
`basiccmd` ont été retirés le même jour : c'étaient des doublons historiques,
remplacés par `stndlone` et `scribacmd`. `basicdll`, `scribawx`, `lmt_wx` et
`calldllbasic` ont été retirés aussi : les deux premiers n'étaient pas
constructibles ici, les deux autres n'existaient que pour eux.
Commandes :

```sh
python build.py --target windows-x64 --standalone
# -> dist/windows-x64/hello_standalone.exe
python build.py --target windows-x64 --stndlone
# -> dist/windows-x64/hello_stndlone.exe
python build.py --target windows-x64 --libscriba
# -> dist/windows-x64/libscriba.a + dist/windows-x64/include/
```

Le binaire autonome contient l'interpréteur et le bytecode, **pas les
modules** : un script qui importe `hash`, `re`… doit être lancé depuis
`dist/<cible>` pour retrouver `modules/` et `scriba.conf`. Sa sortie est
vérifiée par `python tests/run.py --target <cible> --standalone`, qui lance
`hello_standalone` sans lui donner de `.bas`.

Le chemin `-C` → `stndlone` est construit par `--stndlone` et documenté dans
[REPRISE.md § 4](REPRISE.md#4-fabriquer-un-exécutable-autonome). `--standalone`
reste la voie recommandée : il ne demande pas de compilateur. `stndlone`
reste utile comme second chemin et comme test de la génération C.

**Les 20 `manual.texi.jam`** ne sont ni des programmes ni des tests : c'étaient
des modèles de manuel, un par module. Ils ne figurent donc pas dans ce
tableau, et leur statut ne change pas avec `--standalone` :

- **20/20 supprimés** ; aucun ne reste dans `src/extensions/` ;
- il subsiste **deux** `.jam` de **données** dans `examples/sibawa/`, qui ne
  servent pas à compiler ;
- ne pas les recréer : le remplacement prévu est `pod2md.py`, qui lira les
  blocs `/*POD` des `interface.c` ([ROADMAP P4](ROADMAP.md#p4--documentation),
  [ERRATA E2](ERRATA.md#e2)).

### 2.1 Les binaires de l'ancien portage TCC

Les noms de la question (`scriba.exe` 558 080, `sbhttpd.exe` 666 624,
`standalone.exe` 549 888, `libscriba.a` 808 728, `re_engine.exe` 62 464,
`scriba.conf` 1 326, et leurs équivalents `*64` de 715 264 à 1 264 840)
viennent de `build_tcc/`, le pilote TinyCC qui a précédé ce dépôt, pas de
`ZIP-SRC/` (l'archive source d'origine n'en contient aucun). Ce pilote lit un
`rsp/<cible>.rsp` par cible et écrit dans `bin/` ou `bin64/` :

| ancien produit | cible TCC | rôle | équivalent actuel |
|---|---|---|---|
| `scriba.exe` | `scriba` | interpréteur en ligne de commande | `dist/<cible>/scriba[.exe]` (`build.py --target`) |
| `sbhttpd.exe` | `httpd` | serveur HTTP/FTP multi-thread | `dist/<cible>/sbhttpd[.exe]` (`--httpd`) |
| `standalone.exe` | `standalone` | hôte d'**un** programme déjà traduit en C par `scriba -C` | `hello_stndlone[.exe]` (`--stndlone`) ; l'autre voie est `hello_standalone[.exe]` (`--standalone`, `scriba -E`) |
| `libscriba.a` | `libscriba` | archive statique pour un hôte C | `dist/<cible>/libscriba.a` (`--libscriba`) |
| `re_engine.exe` | `re_engine` | essai en C du moteur Spencer, lié directement aux quatre objets de `re` | `dist/<cible>/re_engine[.exe]` (`--re-engine`) |
| `scriba.conf` | produit par `scriba -k` | configuration **binaire** (ABI, dossiers `module`/`include`, extension `dll`) | `dist/<cible>/scriba.conf`, engendré par `build.py` |

Le suffixe `64` et le dossier `bin64/` venaient de `--arch 64` de TCC : mêmes
sources, compilateur x86_64, configuration séparée. Ce n'est pas un doublon de
l'architecture 32 bits, et c'est pourquoi `scriba.conf` fait 1 326 octets en
32 bits contre 1 334 en 64 : le fichier encode des tailles de structures.

Les noms **sans** `.exe` de la liste (`scriba64`, `sbhttpd64`, `standalone64`,
`re_engine64`) sont les binaires **ELF du portage Linux**, produits par
`sbtool` ou gcc, qui ont cohabité avec les PE `.exe` du TCC x86_64 dans la
même arborescence `bin64/`. Les écarts de taille entre `scriba64` (834 512) et
`scriba64.exe` (715 264) viennent de là : ce sont deux formats exécutables
différents, pas deux versions du même fichier.

Depuis la migration vers Zig, `build.py` n'écrit plus dans `bin/` : il produit
`dist/<cible>/...`, et seulement ce qu'on lui demande. `build_tcc/` existe
toujours à côté du dépôt, mais il n'est pas rejoué par `build.py` ni par la CI.
Cette liste ne contient ni `basicdll.dll` ni `scribawx` : ces deux formes
n'étaient déjà dans aucune cible de `build_tcc`, et les Makefile d'origine ne
les construisaient pas non plus. Les tailles citées varient d'ailleurs avec la
révision des sources et les options ; elles ne sont pas un contrat.

### 2.2 Sources sans cible : décision d'audit (2026-10-24)

Le recensement croise tous les `.c` de `src/` avec `SOURCES`, `HTTPD_EXTRA`,
`MODULES`, `RE_ENGINE_ENGINE`, `libscriba` et `stndlone`. Il avait recensé
**27** fichiers sans cible ; après le retrait de `tools/fork.c`, il en reste
**26**, tous gardés avec un statut explicite :

| groupe | fichiers | décision | raison |
|---|---|---|---|
| modules non construits | `extensions/{bdb,curl,curses,gd,mysql,psql,xml,zlib,odbc,dyc,dbg}/interface.c` | garder | huit attendent un en-tête tiers (`db.h`, `curl.h`, `gd.h`…, [E13](ERRATA.md#e13)), `odbc` l'ABI Windows, `dyc` l'ABI 32 bits, `dbg` n'a pas de cible (préprocesseur externe) |
| dépendances de ces modules | `extensions/dbg/dbg_con.c`, `extensions/rpool.c` | garder | `dbg` est le préprocesseur externe ; `rpool` est lié par `mysql` dans les Makefiles d'origine |
| sources d'en-têtes et documentation | `command.c`, `matchc.c`, `notimp.c`, `mynotimp.c`, `buildnum.c`, `prepext.c` | garder | ce sont les sources des `.h` correspondants (`command.h`, `matchc.h`, `notimp.h`, `buildnum.h`, `prepext.h`) ; elles ne sont pas compilées |
| diagnostics | `testalloc.c`, `testconf.c`, `cftd.c` | garder | allocateur instrumenté, affichage de la configuration, vidage d'une configuration binaire ; utiles mais sans cible |
| références | `lmt_cio.c`, `tools/md5.c`, `tools/mt19937.c`, `extensions/re/regex_test.c` (+ `regex_test.txt`) | garder | listes de modules historiques et implémentations de référence ; `regex_test.c` est l'ancien pilote interactif, remplacé par `tests/re_engine.c` |
| obsolète | `tools/fork.c` | **retiré** | copie des internes de Cygwin (`winsup.h`, `dll_init.h` absents) ; inutilisable avec `fork`/`CreateProcess` |

`extensions/re/engine.ci` n'est pas dans ce tableau : ce n'est pas une unité
autonome, mais le corps du moteur que `regexec.c` inclut. Le retirer
casserait le module `re` et `--re-engine` ; il reste donc la source du moteur,
aux côtés de `regcomp.c`, `regexec.c`, `regerror.c` et `regfree.c`.

`tests/re_engine.c` a été réimporté de `build_tcc/test/re_engine.c` : c'est
lui que construit `--re-engine`. Il ne remplace pas `tests/re.bas` (qui teste
l'interface du module) mais vérifie le moteur sous-jacent et ses en-têtes
reconstitués.

---

## 3. Le CGI n'est pas un binaire

Il n'y a **aucun** exécutable CGI dans ce dépôt, et il n'y en a jamais eu :
CGI est un **protocole**, et ScriptBasic le sert de deux façons.

1. **CGI externe** — Apache, IIS, nginx+fcgi ou n'importe quel serveur lance
   `scriba.exe` **comme** programme CGI. Le script lit la requête dans les
   variables d'environnement (`QUERY_STRING`, `CONTENT_LENGTH`…) et écrit ses
   en-têtes puis sa page sur la sortie standard. Les exemples sont dans
   `examples/cgi-bin/` (`echo.bas`, `getparam.bas`, `postparam.bas`,
   `listenv.bas`…). Rien à compiler : `scriba` suffit.
2. **CGI interne** — le module `cgi` (`src/extensions/cgi/interface.c`,
   déclarations dans `src/extensions/cgi/cgi.bas`) donne les mêmes
   informations au script quand celui-ci tourne **dans** `sbhttpd`, sans
   passer par des variables d'environnement. C'est ce que `lmt_httpd.c` lie en
   statique : `sbhttpd` contient le module, il n'y a pas de `.dll` à charger
   pour lui. Le même module est aussi produit par `build.py --modules`, car
   `scriba` peut le charger pour un CGI externe.

Les scripts appellent alors les fonctions du module après un `import` :
`cgi::RequestMethod()`, `cgi::QueryString()`, `cgi::getparam("nom")`,
`cgi::getpost("nom")`, `cgi::RemoteAddress()`, `cgi::Cookie("nom")`…
Voir [MODULES.md § 4](MODULES.md#4-cgi).

Le POST est couvert par `python tests/run.py --target <cible> --cgi-post` :
`tests/cgi_post.bas` (urlencoded) et `tests/cgi_upload.bas` (multipart avec
fichier ASCII, puis 65536 octets binaires, puis un fichier de 10 Mo + 1 octet,
puis 10 Mo + 256 Ko) passent par `scriba -c` (corps sur stdin), puis par
`sbhttpd` (module `cgi` lié en statique). Le second vérifie aussi `SaveFile`
et relit le fichier sauve sur disque ; les deux cas au-delà de 10 Mo exigent
le refus `CGI_ERROR_FILEMAX` (`0x00080008`) sans exécution du script : le
module cgi compare chaque octet reçu à `FILE_MAX`, consomme le reste du corps
et rend l'erreur pendant son démarrage, et `sbhttpd` traduit ce code en
`413 Request Entity Too Large` au lieu de sa page d'erreur, qui sort en `500`
pour les autres échecs.

Un sixième cas, `contentmax`, envoie `cgi_post.bas` avec un
`Content-Length` de `CONTENT_MAX + 1` et **aucun corps** : le module refuse
sur l'en-tête seul avec `CGI_ERROR_BIG_CONTENT` (`0x00080002`), sans lire les
octets annoncés. `sbhttpd` répond le même 413. Il ne faut pas en conclure
qu'un vrai client recevra toujours le 413 : s'il est encore en train
d'envoyer, la fermeture peut le devancer. C'est le prix de `CONTENT_MAX`, qui
existe pour ne pas lire un corps démesuré ([ERRATA E23](ERRATA.md#e23)).

Un septième cas exécute `tests/err_runtime.bas`, qui échoue sur sa première
instruction avec le code 22 (`0x16`, bit `0x2` levé) : la réponse doit être
un `500` complet avec la page d'erreur du serveur, sans quoi le client lit
une ligne de statut absente ([ERRATA E24](ERRATA.md#e24)).

Deux cas couvrent les autres chemins du rapporteur. `tests/err_load.bas`
déclare un `include` introuvable : l'erreur tombe au chargement, rapportée
par le lecteur avec ses propres drapeaux ; en mode `scriba -c` le CLI écrit
une page CGI sur stderr et sort avec 0, et le serveur répond la même page en
`500`. `tests/err_module.bas` appelle `cgi::SaveFile` sur un nom qui n'est
pas un fichier téléversé : le code `0x00080000` dépasse `MAX_ERROR_CODE`
(133), donc le rapporteur écrit `0x%08x` et remplace le message par
« Extension specific error: The named file is not uploaded. » — ce cas
vérifie aussi que le message du module traverse bien le rapporteur.

Deux cas figent l'erreur tardive, dans les deux rapporteurs
([ERRATA E25](ERRATA.md#e25)). `tests/err_pending.bas` écrit `debut` sans
saut de ligne puis échoue : le serveur retient cette ligne dans son tampon,
donc rien n'est encore parti et il doit encore répondre `500` avec sa page
complète ; le CLI, lui, écrit chaque octet aussitôt, donc `debut` est déjà
sorti et il n'ajoute que le diagnostic. `tests/err_partial.bas` écrit d'abord
sa ligne de statut et `debut`, puis échoue : la réponse a commencé, les deux
n'ajoutent qu'un saut de ligne et le diagnostic. Le lanceur interdit
`<HTML>` et `Content-Type: text/html` dans les deux modes, plus `HTTP/1.0`
dans le corps du `sbhttpd`, et attend encore `debut` suivi de `error &H16` en
mode `scriba -c`. À l'inverse, `err_runtime.bas` et `err_module.bas`, qui
n'écrivent rien, doivent contenir la page et sa ligne `Status: 200 OK` en
mode CLI. Le lanceur compte le diagnostic : une seule occurrence attendue par
échec, et un seul `<HTML>` par réponse.

---

## 4. Les modules (20 extensions)

Un module est une bibliothèque partagée (`.dll` sous Windows, `.so` sous
Unix) que `modumana.c` charge sur un `import`. Chacun a son `interface.c` et,
sauf exception, un `.bas` de déclarations du même nom.

État de compilation de `interface.c` seul, **Windows x64, Zig, `-DSTATIC_LINK=1`**
(essai du 2026-10-23 ; ne prouve rien du lien ni du comportement) :

| module | dépendance externe | `interface.c` compile | remarque |
|---|---|---|---|
| `cgi` | aucune | **oui** | lié dans `sbhttpd`, essayé |
| `cio` | aucune | **oui** | `notunix.txt` : console Windows seulement |
| `dbg` | aucune | **oui** | protocole de débogage (sockets) |
| `hash` | aucune | **oui** | |
| `mt` | aucune | **oui** | threads ; lié dans `sbhttpd`, essayé |
| `win` | aucune | **oui** | Windows seulement (`notunix.txt`) ; ex-`nt` |
| `re` | aucune (moteur Spencer embarqué) | **oui** | il faut aussi `regcomp.c`, `regexec.c`, `regerror.c`, `regfree.c` |
| `t` | aucune | **oui** | |
| `trial` | aucune | **oui** | |
| `dyc` | aucune | **non** | `pModuleObject` non déclaré en 64 bits ; appel par pile = ABI 32 bits (`notunix.txt`) |
| `odbc` | `odbc32` (fourni par Windows) | **non** | `SQLINTEGER*` (long) passé pour `SQLLEN*` (long long) : cast à écrire |
| `unix` | aucune | **oui** (Linux/macOS) | ex-`ux` ; demande `unistd.h` et `sys/stat.h` |
| `bdb` | Berkeley DB (`db.h`) | **non** | en-tête absent |
| `curl` | libcurl (`curl/curl.h`) | **non** | en-tête absent |
| `curses` | ncurses (`curses.h`) | **non** | en-tête absent |
| `gd` | libgd (`gd.h`) | **non** | en-tête absent |
| `mysql` | libmysqlclient (`mysql/mysql.h`) | **non** | en-tête absent |
| `psql` | libpq (`postgresql/libpq-fe.h`) | **non** | en-tête absent |
| `xml` | libxml2 (`libxml/parser.h`) | **non** | en-tête absent |
| `zlib` | zlib (`zlib.h`) | **non** | en-tête absent |

Les huit modules « en-tête absent » ne sont pas cassés : ils attendent une
bibliothèque tierce. Zig sait en fournir plusieurs (zlib par exemple), les
autres demandent une compilation croisée de la bibliothèque. Décision du
2026-10-23 : ils ne sont pas planifiés, `mysql` excepté — voir
[MODULES.md § 6.3](MODULES.md#63-décision--les-modules-à-bibliothèque-tierce).

**Neuf sont maintenant construits et chargés à l'exécution** : `hash`, `re`,
`t`, `trial`, `cgi` et `mt` partout, `cio` et `win` sous Windows, `unix`
sous Linux/macOS (`python build.py --modules`, puis
`tests/run.py --modules`).
Leur produit est `dist/<cible>/modules/<module>.dll|.so|.dylib`, chargé par
`import` ou `declare … lib "<module>"`. Détail :
[MODULES.md § 6](MODULES.md#6-construire-les-modules-aujourdhui).

---

## 5. Les outils du dépôt

| outil | rôle |
|---|---|
| `build.py` | compilation des six cibles (`--target`, `--all`, `--httpd`, `--modules`, `--standalone`, `--stndlone`, `--libscriba`, `--re-engine`, `--clean`, `--list`) |
| `tests/run.py` | lance les tests d'une cible construite (WSL automatique pour Linux) ; `--upstream` lit le manifeste de `tests/upstream/`, `--re-engine` exécute l'essai C du moteur `re`, `--cgi-post` vérifie le POST CGI urlencoded et multipart, refus au-delà de FILE_MAX et de CONTENT_MAX compris, par `scriba -c` et par `sbhttpd` ; `--grammaire` ne lance rien et vérifie statiquement qu'aucun nom de la grammaire ne pointe sur un corps vide ; le jeu par défaut comprend neuf refus attendus (`refus_directory`, `refus_pause`, `refus_fork`, `refus_sqr`, `refus_operateur`, `refus_cv`, `refus_mk`, `refus_slif_for`, `refus_slif_mix`) |
| `pod2md.py` | engendre `docs/MANUEL.md` (210 entrées) et `docs/modules/*.md` depuis les blocs `/**` des commandes et `/*POD` des modules ; `--check` échoue si un fichier a divergé des sources |
| `scriba.conf.lsp` | configuration en texte, compilée en `scriba.conf` binaire par `scriba -f … -k …` |
| `webtest/` | deux pages d'essai servies par `sbhttpd` |
| `examples/` | 178 programmes BASIC d'origine (démonstrations, CGI, base de données…) |
| `tests/upstream/` | la suite de tests d'origine, conservée telle quelle, et son `MANIFEST.tsv` lu par `--upstream` |
| `tests/commandes.bas` | 34 vérifications des erreurs de commandes (répertoires non ouverts, `print #n`, `formatdate`, `isnumeric`, `on error resume next`, [E26 à E31](ERRATA.md#e26)), des alias rétablis et des littéraux préservés ([E35, E36](ERRATA.md#e35)) |
| `tests/slif.bas` | 30 vérifications du `if` d'une ligne : `ELSE`, chaînes de `ELSEIF`, arrêt à la première branche vraie, conditions `undef`/chaîne, imbrication dans un bloc et dans une boucle ([E37](ERRATA.md#e37)) |
| `tests/refus_slif_for.bas`, `tests/refus_slif_mix.bas` | sources qui doivent être refusées à l'analyse : une branche d'une ligne n'ouvre pas un `FOR`, et les formes d'une ligne et multi-ligne ne se mélangent pas ([E37](ERRATA.md#e37)) |
| `tests/refus_directory.bas` | source qui doit être refusée à l'analyse : `open … for directory` ([E28](ERRATA.md#e28)) |
| `tests/refus_pause.bas`, `refus_fork.bas`, `refus_sqr.bas` | les trois formes retirées de la grammaire : `pause` et `fork` ([E34](ERRATA.md#e34)), et l'ancien nom `sqr`, qui doit échouer en `&H68` alors que `sqrt` marche |
| `tests/refus_operateur.bas`, `refus_cv.bas`, `refus_mk.bas` | les autres familles retirées : un opérateur mort (`==`, `&H77`) et les conversions binaires `cvd`… / `mkd`… (`&H68`) ([E34](ERRATA.md#e34)) |
| `tests/trig.bas` | 73 vérifications : les quinze fonctions de l'audit, les six réciproques hyperboliques (`ahsin`…`ahctan`) et `rad2deg`/`deg2rad` ; valeurs remarquables, domaines et pôles à zéro exact, branches de `actan`, `pi()` à la précision du double ([E32](ERRATA.md#e32), [E34](ERRATA.md#e34)) |
| `tests/nombres.bas` | 19 vérifications des bornes entières : littéraux au-delà de la plage, produit qui déborde, `imax()`/`imin()` ([E33](ERRATA.md#e33)) |
| `tests/mathfunc.bas` | 76 vérifications de `max`, `min`, `imax`, `imin`, `bin`, `gcd` et `lcm` : arité, signe, troncature des réels, type du résultat, indice du gagnant compté à partir de 0, argument `undef` |
