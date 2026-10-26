# Modules, documentation et remplacement des `.jam`

## 1. Ce qu'étaient les fichiers `.jam`

Il y en avait **20**, tous nommés `manual.texi.jam`, un par module de
`src/extensions/`. Chacun faisait trois lignes :

```
{#sep/[[[/]]]}

[[[#define MODULE=re]]]
[[[#define FILE=interface.c]]]
[[[#include ../../html/texi/skeleton.jim]]]
```

C'est un modèle **Jamal** : un outil de macro-expansion lisait le fichier,
remplaçait `MODULE` et `FILE`, puis incluait `html/texi/skeleton.jim`, qui
découpait `interface.c` pour en tirer un manuel TeXinfo. Autrement dit :

- le `.jam` **ne contenait aucune documentation** : juste le nom du module et
  celui du fichier à lire ;
- le texte des manuels est dans les blocs `/*POD … */` des `interface.c`
  (eux-mêmes découpés par les repères `=H nom`, `=S section`, `/*FUNCTION*/`) ;
- la chaîne complète demandait Jamal, Perl et TeXinfo, tous absents de ce
  dépôt et tous remplacés ou abandonnés.

Les 20 `.jam` ont donc été **supprimés** : ils ne décrivaient rien et
pointaient vers un `skeleton.jim` qui n'existe pas ici. Aucun autre fichier du
dépôt ne les citait (les commentaires « ne pas éditer » des `.bas` ont été
réécrits lors de l'aplatissement).

## 2. Par quoi ils sont remplacés

Par deux choses :

1. **Le tableau ci-dessous** — il dit, pour chaque module, quel fichier porte
   la documentation et combien de fonctions publiques il déclare. C'est
   l'information que le `.jam` portait réellement (module → fichier).
2. **Les blocs `/*POD` restés dans les `interface.c`** — c'est le texte, et il
   est toujours là. Rien ne le lit automatiquement, mais il est lisible à la
   main et reste la source de vérité pour qui veut régénérer un manuel.

Si un jour un manuel HTML ou Markdown est souhaité, le travail est **un seul
script** d'environ 200 lignes : lire les `=H`/`=S`/`=B`/`=n` des
`interface.c`, les traduire en Markdown, puis produire `docs/modules/<nom>.md`.
C'est inscrit au [ROADMAP](ROADMAP.md#p4--documentation) ; aucun outil Perl
n'est à réinstaller.

## 3. Le tableau des modules

« fonctions » = nombre de `besFUNCTION(...)` dans `interface.c`, c'est-à-dire
de fonctions que le programme BASIC peut appeler (les `versmodu`, `bootmodu`,
`finimodu`… ne sont pas comptés ici).

| module | documentation (source) | déclarations BASIC | fonctions | particulier |
|---|---|---|---|---|
| `bdb` | `src/extensions/bdb/interface.c` | `bdb.bas` | 15 | Berkeley DB |
| `cgi` | `src/extensions/cgi/interface.c` | `cgi.bas` | 32 | voir § 4 |
| `cio` | `src/extensions/cio/interface.c` | `cio.bas` | 21 | console Windows |
| `curl` | `src/extensions/curl/interface.c` | `curl.bas` | 10 | libcurl |
| `curses` | `src/extensions/curses/interface.c` | `curses.bas` | 30 | terminal |
| `dbg` | `src/extensions/dbg/interface.c` | `dbg.bas` | 0 | protocole de débogage, pas de fonction BASIC |
| `dyc` | `src/extensions/dyc/interface.c` | `dyc.bas` | 3 | appels C dynamiques, ABI 32 bits |
| `gd` | `src/extensions/gd/interface.c` | `gd.bas` | 24 | images |
| `hash` | `src/extensions/hash/interface.c` | `hash.bas` | 12 | tables de hachage |
| `mt` | `src/extensions/mt/interface.c` | `mt.bas` | 18 | threads et sessions ; lié aussi dans `sbhttpd` |
| `mysql` | `src/extensions/mysql/interface.c` | `mysql.bas` | 24 | MySQL |
| `win` | `src/extensions/win/interface.c` | `win.bas` | 11 | API Windows (registre, services, processus) ; s'appelait `nt` |
| `odbc` | `src/extensions/odbc/interface.c` | `odbc.bas` | 8 | bases ODBC |
| `psql` | `src/extensions/psql/interface.c` | `psql.bas` | 25 | PostgreSQL |
| `re` | `src/extensions/re/interface.c` | `re.bas` | 6 | expressions régulières (moteur Spencer) |
| `t` | `src/extensions/t/interface.c` | `t.bas` | 6 | |
| `trial` | `src/extensions/trial/interface.c` | `trial.bas` | 6 | module d'essai, sert aux tests de l'ABI |
| `unix` | `src/extensions/unix/interface.c` | `unix.bas` | 2 | appels Unix (`fork`, `chmod`) ; s'appelait `ux` |
| `xml` | `src/extensions/xml/interface.c` | `xml.bas` | 19 | libxml2 |
| `zlib` | `src/extensions/zlib/interface.c` | `zlib.bas` | 4 | compression |

Les 20 `.bas` sont présents depuis le 2026-10-23 (15 manquaient ; ils ont été
repris de la distribution d'origine). Ils servent au `import module.bas` des
programmes BASIC et sont aussi la référence des noms exposés. Neuf modules
sont réellement construits (§ 6) ; les onze autres ne le sont pas encore.

Deux ont été renommés pour la même raison que dans la vague NG : `nt`
devient `win` (le nom d'un système disparu ne dit plus de quoi le module
parle) et `ux` devient `unix`. Le renommage va jusqu'aux fonctions exportées,
comme dans NG : `win_hardlink`, `win_startservice`, `unixfork`, `unixchmod`.
Les anciens noms (`nt_hardlink`, `uxfork`…) n'existent plus ; aucun script
livré ne les employait.

## 4. cgi

Le module `cgi` mérite sa section parce qu'il est lié en statique dans
`sbhttpd` avec `mt`, et parce qu'il a deux vies :

- **sous `sbhttpd`** — le serveur remplit la structure au moment de la
  requête ; `cgi::RequestMethod()`, `cgi::ScriptName()`, `cgi::QueryString()`
  renvoient les valeurs de la requête en cours. `lmt_httpd.c` déclare
  `extern SLFST CGI_SLFST[]` et l'inscrit dans `StaticallyLinkedModules`.
- **en CGI externe** — le même module lit `getenv` ; c'est ce qui permet aux
  scripts de `examples/cgi-bin/` de tourner sous Apache/IIS avec `scriba`
  comme exécutable CGI.
- **comme module chargeable** — `build.py --modules` produit aussi `cgi.*`,
  et `tests/cgi.bas` simule une requête GET en posant les variables
  d'environnement. Hors requête, le `bootmodu` du module bascule en mode
  « fichier de débogage » et rend l'erreur `0x00080004` : un programme
  ordinaire doit donc fournir au moins `REQUEST_METHOD`.

Il n'y a donc **pas** de programme CGI à compiler : voir
[PROGRAMMES.md § 3](PROGRAMMES.md#3-le-cgi-nest-pas-un-binaire).

## 5. Les deux `.jam` qui restent (exemple SIBAWA)

`examples/sibawa/sibawa.texi.jam` (manuel Texinfo de l'application) et
`examples/sibawa/templates.txt.jam` (gabarits HTML en syntaxe Jamal) ne sont
pas des fichiers de build : ce sont les **données** de l'exemple SIBAWA. Ils
ne sont lus par aucun outil du dépôt ; les régénérer demanderait `jamal.pl` et
`ssplit.pl` (voir `examples/sibawa/mktm.cmd`), deux outils Perl absents.

Ils sont donc conservés tels quels. Le jour où SIBAWA devient un exemple
exécutable, deux voies : pré-générer `Templates/*.html` hors du dépôt et les
versionner, ou réécrire `mktm.cmd` en Python. Détail et recommandation :
[docs/README.md § 3.2](README.md#32-ce-qui-reste--2-fichiers-et-ce-quils-sont-vraiment).

## 6. Construire les modules (aujourd'hui)

```sh
python build.py --target windows-x64 --modules
python build.py --all --modules
python build.py --target windows-x64 --modules --release   # sans trial
```

Neuf modules sont construits, ceux qui n'ont **aucune bibliothèque
tierce** :

| module | sources compilées | déclaration copiée | test |
|---|---|---|---|
| `hash` | `interface.c` | `hash.bas` | `tests/hash.bas` |
| `re` | `interface.c` + `regcomp.c`, `regexec.c`, `regerror.c`, `regfree.c` | `re.bas` | `tests/re.bas` |
| `t` | `interface.c` | `t.bas` | `tests/md5.bas` |
| `trial` | `interface.c` | — (chemins absolus dans l'original) | `tests/trial.bas` ; `test_only`, exclu par `--release` |
| `cgi` | `interface.c` + `cgi.c` | `cgi.bas` | `tests/cgi.bas` |
| `mt` | `interface.c` | `mt.bas` | `tests/mt.bas` |
| `cio` | `interface.c` | `cio.bas` | `tests/cio.bas` |
| `win` | `interface.c` | `win.bas` | `tests/win.bas` |
| `unix` | `interface.c` | `unix.bas` | `tests/unix.bas` |

Les six premiers se compilent sur les six cibles. `cio` et `win` sont
Windows seulement, `unix` l'est pour Unix : `build.py` ne tente que les
modules possibles sur la cible (clé `modules` de la configuration, voir
[MODULES § 3](MODULES.md#3-le-tableau-des-modules)).

Le produit va dans `dist/<cible>/modules/<module><ext>`, avec l'extension
que la clé `dll` de la configuration attend : `.dll` sous Windows, `.so` sous
Linux, `.dylib` sous macOS. Les déclarations vont dans
`dist/<cible>/modules/include/`, parce que `import re.bas` cherche dans la clé
`include` (`modules/include/`), pas à côté du script.

### 6.1 Deux règles que le portage a dû apprendre

**Les modules sont compilés avec `-fvisibility=hidden`.** Sous ELF et Mach-O,
une fonction sans attribut est exportée, et les références internes d'une
bibliothèque chargée par `dlopen` peuvent être détournées vers la libc, qui
est plus tôt dans la portée globale. C'est arrivé à `re` : le moteur Spencer
définit `regcomp`/`regexec`/`regfree`, la libc aussi ; l'appel partait dans
la libc, qui écrivait une `regex_t` de glibc dans une `regex_t` de Spencer.
Symptôme mesuré sous Linux x64 : `re::n()` rendait 224 au lieu de 1, puis
`malloc(): unaligned tcache chunk detected`. La parade est en deux moitiés :
`DLL_EXPORT` marque explicitement les fonctions exportées
(`__attribute__((visibility("default")))` hors Windows), et le reste est
caché. Voir [ERRATA E18](ERRATA.md#e18). Conséquence pour un futur module :
toute fonction destinée au BASIC doit passer par `besFUNCTION`/`besCOMMAND`
ou être marquée `DLL_EXPORT`.

**Un module n'est pas un programme.** Pas de `main`, et pas de lien avec les
objets du coeur : il ne connaît l'interpréteur que par la `pSupportTable`
reçue en premier argument. C'est ce qui permet de le charger dans `scriba`
ou dans `sbhttpd` sans recompiler l'hôte.

### 6.2 Ce qui reste

Ne sont pas construits comme modules chargeables : `odbc` (64 bits), `dyc`
(ABI 32 bits), et les huit à bibliothèque tierce (`zlib`, `curl`, `xml`,
`gd`, `curses`, `mysql`, `psql`, `bdb`) — dix au total. Ces huit derniers
sont l'objet d'une décision, § 6.3.

`dbg` n'est pas produit par `--modules` : ce n'est pas un module mais un
**préprocesseur externe** (interface `prepext.h`, chargé par `scriba -p`),
sans `versmodu` ni `bootmodu`. Le construire en bibliothèque partagée serait
techniquement possible — `scriba -p` charge justement un objet partagé — mais
aucune cible n'existe et son usage reste au développement.

`trial` est un module d'**essai de l'ABI** : il reste construit par
`--modules` pour `tests/trial.bas`, mais il est marqué `test_only` dans
`build.py`. `--release` le saute et supprime un `trial.dll`/`.so`/`.dylib`
resté dans `dist/<cible>/modules/` d'une construction précédente, afin qu'un
artefact de production ne le transporte pas par accident.

Le ROADMAP P2 et P6 les liste un par un.

### 6.3 Décision : les modules à bibliothèque tierce

**Les huit modules qui réclament une bibliothèque extérieure ne sont pas
planifiés, à une exception près : `mysql`.** Ce n'est pas un refus de
principe, c'est une conséquence du dépôt tel qu'il est.

Le critère retenu est **« qui s'en sert ici »**, pas « quelle bibliothèque
est utile en général ». Comptage des `import` dans `examples/`, `tests/` et
`webtest/` :

| module | fichiers du dépôt qui l'importent | verdict |
|---|---:|---|
| `mysql` | 7 (dont SIBAWA) | **retenu** : seule bibliothèque qui rend exécutable une application du dépôt |
| `curl` | 6 (exemples de démonstration) | non planifié ; TLS à maintenir |
| `gd` | 2 | non planifié |
| `odbc` | 1 | hors sujet ici : aucune bibliothèque externe (E11) |
| `psql` | 0 | non planifié |
| `bdb` | 0 | non planifié |
| `xml` | 0 | non planifié |
| `curses` | 0 | non planifié |
| `zlib` | 0 | non planifié (Zig fournit zlib, mais rien ne l'appelle) |

Ce que coûterait un module tiers :

- **huit bibliothèques × six cibles**, avec en-têtes, versions et dépendances
  transitives (`gd` tire zlib/png/freetype, `curl` une pile TLS, `mysql`
  OpenSSL) ;
- l'installation de ces bibliothèques dans la CI Windows, Ubuntu et macOS, ou
  leur vendorisation — et l'invariant « un clone neuf se reconstruit avec
  `zig cc` seul » est perdu ;
- des licences à arbitrer pour un dépôt LGPL 2.1 : le client MySQL est GPLv2,
  Berkeley DB 6 est AGPL ;
- les interfaces existent (24 fonctions pour `mysql`, 25 pour `psql`, 24 pour
  `gd`…) mais **aucune n'a de test** : il faudrait les écrire pour trois
  systèmes, ce qui coûte plus cher que le portage lui-même.

**L'exception `mysql`** est conditionnée : elle ne se justifie que si SIBAWA
(`examples/sibawa/`) devient l'objectif. Elle demande alors `cgi` + `mysql` +
`t` + `mt`, tous déjà construits sauf `mysql`. Tant que ce n'est pas le cas,
rien ne distingue `mysql` d'un autre module tiers.

**Ce que la décision laisse ouvert** : le contrat du projet. Si l'objectif
devient « portage fidèle de la distribution ScriptBasic », cette section est à
rouvrir en bloc ; si l'objectif reste « un interpréteur qui se construit et se
teste partout », elle tient. Ce qui manque à un BASIC en 2026 n'est de toute
façon aucun des huit : c'est JSON, SQLite et HTTPS.

## 7. Règle à retenir

Aucun fichier généré n'est versionné. Un module = un `interface.c` (+ ses
éventuels `.c` de moteur) + un `.bas` + ses données (`regex.h`, `engine.ci`
pour `re`, `copyright.txt`…). La documentation est dans le `.c`. Si un outil
doit un jour en extraire quelque chose, ce sera un script du dépôt, versionné
avec lui — pas un `.jam` à côté de chaque source.
