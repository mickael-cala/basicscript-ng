# Documentation — index, état et questions fréquentes

Ce dossier contient tout ce qu'il faut pour reprendre `scriba-zig` sans
contexte : par une personne comme par un agent. Ce fichier est le **sommaire**
et il répond d'abord aux questions précises qu'on se pose à l'arrivée.

État : **2026-10-24**. Toutes les affirmations ci-dessous ont été vérifiées
sur cette machine ; ce qui n'a pas été vérifié est dit comme tel.

---

## 1. Ordre de lecture

| ordre | document | à quoi il sert |
|---|---|---|
| 1 | [REPRISE.md](REPRISE.md) | l'état du dépôt, les commandes, la carte du code |
| 2 | ce fichier | le sommaire et les questions ouvertes |
| 3 | [PROGRAMMES.md](PROGRAMMES.md) | tous les exécutables : construits, possibles, abandonnés |
| 4 | [MODULES.md](MODULES.md) | les 20 modules et le remplacement des `.jam` |
| 5 | [ERRATA.md](ERRATA.md) | les défauts connus, `E1`…`E38`, identifiants stables |
| 6 | [ROADMAP.md](ROADMAP.md) | ce qui reste, par priorité, avec critère d'acceptation |
| 7 | [EVOLUTIONS.md](EVOLUTIONS.md) | les décisions prises et pourquoi |
| 8 | [INVENTAIRE.md](INVENTAIRE.md) | ce que contient le dépôt, fichier par fichier |
| 9 | [GRAMMAIRE.md](GRAMMAIRE.md) | mots-clés, fonctions, numéros et stratégie de renumérotation |
| — | [../AGENTS.md](../AGENTS.md) | les consignes de travail (interdits, conventions) |

---

## 2. L'état en un tableau

| question | réponse courte | détail |
|---|---|---|
| reste-t-il des `.jam` ? | **2**, dans `examples/sibawa/` seulement. Aucun dans le build. | § 3 |
| par quoi les remplacer ? | rien à remplacer pour compiler ; ce sont des données d'exemple, pas des modèles de build | § 3 |
| manque-t-il des `.bas` ? | **non** : 20/20 modules, 3/3 `include/`, 32/32 fichiers de `test/` | § 4 |
| que manque-t-il alors ? | les **binaires** de onze modules (neuf sont construits : `hash`, `re`, `t`, `trial`, `cgi`, `mt`, `cio`, `win`, `unix`) ; les formes `basicdll` et `scribawx` ont été retirées le 2026-10-24 ([E21](ERRATA.md#e21), [E22](ERRATA.md#e22)) | § 4, [MODULES § 6](MODULES.md#6-construire-les-modules-aujourdhui) |
| `nt` et `ux`, devenus quoi ? | `nt` → **`win`**, `ux` → **`unix`** (même renommage que la vague NG). Les fonctions exportées gardent leurs noms C | [MODULES § 3](MODULES.md#3-le-tableau-des-modules) |
| sur quelle plateforme tourne le serveur HTTP multi-thread ? | **Windows x64** et **Linux x64** (Zig, prouvés), **Windows x86** (campagne TCC) ; macOS : ni compilé ni exécuté | § 5 |
| y a-t-il un programme CGI ? | **non, et il n'y en a jamais eu** : CGI est un protocole, servi par `scriba` ou par le module `cgi` dans `sbhttpd` | § 5 |
| quels programmes existent ? | `scriba`, `sbhttpd`, exécutable autonome (`-E`), runtime `stndlone` (`-C`), archive `libscriba.a`, essai `re_engine` ; `basicdll` et `scribawx` ont été retirés | [PROGRAMMES.md](PROGRAMMES.md) |
| « scriba embarquable », c'est quoi ? | `libscriba.a` est une archive construite par `--libscriba` (coeur + 47 objets + en-têtes) ; `basicdll.dll` a été retirée le 2026-10-24, `basicdll.c` ne contenant qu'une déclaration, sans implémentation ([E21](ERRATA.md#e21)) | [PROGRAMMES.md § 2](PROGRAMMES.md#2-les-autres-programmes-et-formes-du-dépôt) |

---

## 3. Les fichiers `.jam` : combien, où, et par quoi ils sont remplacés

### 3.1 Ce qui a été supprimé du build : 20 fichiers

Les 20/20 `manual.texi.jam` de `src/extensions/<module>/` ont été **supprimés**.
Chacun faisait trois lignes (`MODULE=…`, `FILE=interface.c`,
`#include ../../html/texi/skeleton.jim`) : un modèle **Jamal** qui demandait
Jamal, Perl et TeXinfo pour produire un manuel TeXinfo. Ils ne contenaient
aucune documentation.

Remplacés par :

| ce que le `.jam` portait | remplacement | où |
|---|---|---|
| module → fichier à documenter | un tableau des 20 modules | [MODULES.md § 3](MODULES.md#3-le-tableau-des-modules) |
| le texte du manuel | les blocs `/*POD`, `=H`, `=S` restés dans chaque `interface.c` | `src/extensions/*/interface.c` |
| la fabrication d'un manuel | `pod2md.py` (Python), qui engendre `MANUEL.md` et `docs/modules/*.md` | [ROADMAP P4](ROADMAP.md#p4--documentation) |

Aucun outil du dépôt ne lit un `.jam`. `build.py` compile, `tests/run.py`
essaie : rien d'autre.

### 3.2 Ce qui reste : 2 fichiers, et ce qu'ils sont vraiment

| fichier | nature | l'utiliser demande | recommandation |
|---|---|---|---|
| `examples/sibawa/sibawa.texi.jam` | le **manuel Texinfo** de l'application d'exemple SIBAWA (texte réel, ~50 lignes ; seule la première ligne est une directive Jamal) | rien : c'est du texte lisible ; retirer la ligne `{#sep/[[[/]]]}` suffit à en faire un `.texi` ou un `.md` | le garder tel quel (copie de l'amont) ; le convertir le jour où l'exemple est repris |
| `examples/sibawa/templates.txt.jam` | les **gabarits HTML** de SIBAWA, en syntaxe Jamal | `jamal.pl` **et** `ssplit.pl` (voir `examples/sibawa/mktm.cmd`) — deux outils Perl absents du dépôt | ne pas le convertir à la main : pré-générer `Templates/*.html` hors du dépôt et les versionner, ou réécrire `mktm.cmd` en Python |

Ce sont des **données d'application**, pas des fichiers de compilation : les
supprimer ne casserait aucun build, mais rendrait l'exemple SIBAWA
irreproductible. La règle du dépôt (« aucun `.jam` ») vise la chaîne de
build ; elle ne demande pas de détruire les données d'un exemple. Voir
[ROADMAP P6](ROADMAP.md#p6--le-reste) pour la décision à prendre le jour où
SIBAWA devient un exemple exécutable.

---

## 4. Les `.bas` : ce qui est là, ce qui manque

### 4.1 Ce qui est là

| catégorie | nombre | emplacement |
|---|---|---|
| déclarations des modules (`import hash.bas`…) | **20 / 20** | `src/extensions/<module>/<module>.bas` |
| bibliothèque d'includes | **3 / 3** (`error.bas`, `time.bas`, `modinst.bas`) | `include/` |
| tests locaux | 29 | `tests/*.bas` (11 des modules, 6 autonomes, 3 scripts d'erreur pour `--cgi-post`, 9 hors jeu) |
| suite d'origine | 32 `.bas` (dont 3 sous-dossiers) | `tests/upstream/` |
| exemples | 178 | `examples/` |
| pages du serveur d'essai | 2 | `webtest/` |
| **total** | **261 `.bas`** | |

Les 20 `.bas` de modules ont été rétablis le 2026-10-23 (15 manquaient) : sans
eux, `import hash.bas` échoue même avec un `hash.dll` construit.

### 4.2 Ce qui n'a pas été repris de l'amont, et pourquoi

| absent | nombre | raison |
|---|---|---|
| `html/examples/progs/*.bas` | 15 | extraits de manuel **générés** à partir des exemples ; les exemples eux-mêmes sont là |
| `cbbfc.bas`, `headerer.bas`, `heber.bas`, `hebtest.bas`, `newbuild.bas`, `sample.heb.bas` | 6 | programmes de la chaîne Perl/Heber (génération d'en-têtes, de makefiles) abandonnée |
| `extensions/curses/cursestest.bas` | 1 | essai du module `curses`, qui n'est pas construit (ncurses absent) |
| `test/mktest.pl` | 1 | pilote d'essai Perl, remplacé par `tests/run.py` |

Aucun de ces fichiers n'est nécessaire à la compilation ni aux tests actuels.

### 4.3 Ce qui manque vraiment (ce ne sont pas des `.bas`)

| manque | conséquence | prévu |
|---|---|---|
| **onze** extensions non construites (`zlib`, `curl`, `xml`, `mysql`, `dbg`…) | `import mysql.bas`, `import zlib.bas`… échouent encore | [ROADMAP P2](ROADMAP.md#p2--modules), P6 |
| un générateur de `include/error.bas` | le fichier est figé ([E1](ERRATA.md#e1)) | ROADMAP P2 |
| la cible `--debug` | allocateur instrumenté non constructible en une commande | ROADMAP P5 |
| `MANUEL.md` (manuel du langage) | engendré par `pod2md.py` depuis les blocs du code : 212 entrées, en anglais, comme les sources | ROADMAP P4, fait |
| les bibliothèques tierces | 8 modules en dépendent (`curl`, `xml`, `mysql`…) ; non planifiés, `mysql` excepté | [E13](ERRATA.md#e13), [MODULES § 6.3](MODULES.md#63-décision--les-modules-à-bibliothèque-tierce) |

---

## 5. Plateformes et formes d'exécution

### 5.1 Le serveur HTTP multi-thread (`sbhttpd`)

`sbhttpd` lie le module `mt` et le module `cgi`, et sert chaque requête dans
un thread qui exécute le `.bas` demandé.

| cible | état | preuve |
|---|---|---|
| `windows-x64` | **tourne** | page servie, page 404, 20 requêtes parallèles, journaux écrits (port 18080) |
| `windows-x86` | a tourné sous TCC ; pas réessayé avec Zig | campagne TCC antérieure |
| `linux-x64` | **tourne** | page `index.bas` servie par `curl --http0.9`, via WSL |
| `linux-arm64` | non essayé | correctif d'[E7](ERRATA.md#e7) appliqué, mais pas compilé |
| `macos-x64`, `macos-arm64` | non essayé | correctif d'[E7](ERRATA.md#e7) appliqué, mais pas compilé |

Écarts avec l'ancien serveur TCC, mesurés le 2026-10-24 :

| point | TCC | Zig |
|---|---|---|
| binaire | `bin/sbhttpd.exe` et `bin64/sbhttpd64.exe` (portage TCC 32 et 64 bits) | `dist/windows-x64/sbhttpd.exe` 689 152 octets |
| unités | 49 `SRC` + 3 objets de `ext_native` = 52 | 45 du coeur + 7 `HTTPD_EXTRA` = 52 |
| modules statiques | TCC tolérait les `versmodu`/`bootmodu` en double | `-DSTATIC_LINK=1` est obligatoire pour `cgi` et `mt`, sinon `lld` refuse le lien |
| Unix | le serveur ne compilait pas sous Linux | compile et sert sous `linux-x64` ([E7](ERRATA.md#e7)) |
| page d'essai | l'ancienne campagne vérifiait `index.bas` avec 5050 et 3628800 | `webtest/index.bas` répond seulement `sbhttpd OK` ; c'est la page qui a changé, pas le serveur |
| mode service | jamais essayé | jamais essayé |
| réponses | mêmes sources, mêmes pages | index et 404 identiques au serveur TCC exécuté sur le même port |

Contrôle Zig du 2026-10-24 : build `--httpd` en 0,3 s avec les objets déjà
présents, écoute sur 18080, `index.bas` -> `sbhttpd OK`, URL inconnue ->
`HTTP/1.0 404 Not Found` + `introuvable`, 20 requêtes parallèles -> 20 fois
`sbhttpd OK`, 10 erreurs 404 parallèles -> 10 codes 404, journaux
`app.txt`/`hit.txt` écrits, arrêt propre par suppression de `logs/pid.txt`
(environ une seconde). Le `sbhttpd64.exe` TCC, lancé sur le même port avec
la même configuration de test, rend exactement les deux mêmes réponses :
les sources du serveur sont communes, seuls le compilateur et les correctifs
de portabilité changent. Le mode service Windows et les autres cibles Unix
restent non testés.

Le serveur n'est pas un portage séparé : c'est `src/httpd.c` +
`src/variations/httpd/` (dont `service.c`, pour le mode service Windows),
compilé avec le même coeur que `scriba`. Voir
[REPRISE.md § 5](REPRISE.md#5-lancer-le-serveur) pour la recette.

### 5.2 Le CGI

Aucun binaire CGI n'existe ni n'a existé. Deux usages :

1. **CGI externe** : Apache/IIS/nginx lancent `scriba.exe` comme programme
   CGI ; le script lit `QUERY_STRING`, `CONTENT_LENGTH`… et écrit ses en-têtes
   sur la sortie standard. Exemples : `examples/cgi-bin/`. Rien à compiler.
2. **CGI interne** : le module `cgi` est lié **dans** `sbhttpd` (via
   `lmt_httpd.c`) et remplit la structure de requête ; le script appelle
   `cgi::getparam("nom")` sans variables d'environnement.

Détail : [PROGRAMMES.md § 3](PROGRAMMES.md#3-le-cgi-nest-pas-un-binaire) et
[MODULES.md § 4](MODULES.md#4-cgi).

### 5.3 Les programmes, en une ligne chacun

| programme | ce que c'est | construit ? |
|---|---|---|
| `scriba` | l'interpréteur : `.bas` → bytecode → exécution | **oui**, 6 cibles |
| `sbhttpd` | serveur HTTP multi-thread | **oui**, Windows x64 prouvé |
| `scriba -E` | exécutable **autonome** : binaire + bytecode, sans compilateur | **oui**, via `build.py --standalone` ; Windows x86/x64 et Linux x64 vérifiés |
| `stndlone` | runtime du chemin `-C` (bytecode en tableaux C) | **oui**, via `build.py --stndlone` ; Windows x86/x64 et Linux x64 vérifiés |
| `libscriba.a` | archive statique (coeur + `libscriba.c` + `lmt_none.c`) | **oui**, via `build.py --libscriba` ; six cibles |
| `re_engine` | essai C du moteur Spencer (`REG_BASIC`, `REG_PEND`, références arrière) | **oui**, via `build.py --re-engine` ; Windows x86/x64 et Linux x64 |

`basicdll.dll` et `scribawx` ont été retirés le 2026-10-24 : le premier
n'avait pas d'implémentation ([E21](ERRATA.md#e21)), le second demandait
wxWidgets ([E22](ERRATA.md#e22)). Ils ne font plus partie du dépôt.

Tableau complet, avec « ce qui manque » pour chaque ligne :
[PROGRAMMES.md](PROGRAMMES.md).

---

## 6. Les fichiers de reprise, et leur rôle

Un clone neuf doit contenir exactement ceci pour être reprenable. La colonne
« présent » est à vérifier par `ls` ; aucun de ces fichiers n'est généré.

| fichier | rôle | présent |
|---|---|---|
| `README.md` | présentation, prérequis, build, tests | oui |
| `AGENTS.md` | consignes de travail pour un agent | oui |
| `LICENSE` | LGPL 2.1 d'origine | oui |
| `.gitignore` | aucun binaire versionné | oui |
| `.gitattributes` | fins de ligne stables | oui |
| `build.py` | le seul pilote de compilation | oui |
| `scriba.conf.lsp` | configuration du runtime, en texte | oui |
| `tests/run.py` | lanceur d'essais, y compris `--upstream`, `--standalone`, `--stndlone`, `--re-engine` et `--cgi-post` | oui |
| `tests/*.bas` | 6 essais autonomes + 20 exigeant des modules + 3 scripts d'erreur (dont 9 couverts par `--modules` et 5 par `--cgi-post`) | oui |
| `tests/upstream/` | suite d'origine (32 `.bas`) et son `MANIFEST.tsv` | oui |
| `src/` | 93 `.c`, 55 `.h`, 20 `.bas` de modules | oui |
| `examples/` | 178 exemples d'origine | oui |
| `include/` | `error.bas`, `time.bas`, `modinst.bas` | oui |
| `webtest/` | 2 pages pour `sbhttpd` | oui |
| `.github/workflows/build.yml` | CI Windows / Ubuntu / macOS | oui |
| `docs/REPRISE.md` | point d'entrée | oui |
| `docs/PROGRAMMES.md` | tous les programmes | oui |
| `docs/MODULES.md` | modules et `.jam` | oui |
| `docs/ERRATA.md` | défauts connus, `E1`…`E22` | oui |
| `docs/ROADMAP.md` | priorités et critères d'acceptation | oui |
| `docs/EVOLUTIONS.md` | décisions et historique | oui |
| `docs/INVENTAIRE.md` | contenu du dépôt | oui |
| `docs/README.md` | ce sommaire | oui |

Absents **volontairement** : `Makefile*`, `*.jam` de build, `*.pl`, `*.jim`,
`*.def`, `html/`, `texi/`, `obj/`, `dist/`, `bin*/`. Voir
[INVENTAIRE.md § 5](INVENTAIRE.md#5-ce-qui-manque-par-rapport-à-lamont-et-pourquoi).

---

## 7. Tenir ces documents à jour

| événement | document à modifier |
|---|---|
| nouveau programme, nouvelle cible | `PROGRAMMES.md`, `README.md` |
| module qui compile ou qui casse | `PROGRAMMES.md § 4`, `MODULES.md` |
| défaut trouvé, limite assumée | `ERRATA.md` (nouvel `E…`, jamais réutilisé) |
| décision de conception | `EVOLUTIONS.md` |
| tâche faite ou ajoutée | `ROADMAP.md` |
| fichier ajouté ou retiré | `INVENTAIRE.md`, § 6 ci-dessus |

Une modification n'est finie que si `python build.py --target <cible>` et
`python tests/run.py --target <cible>` passent — voir
[REPRISE.md § 2](REPRISE.md#2-construire-et-tester).
