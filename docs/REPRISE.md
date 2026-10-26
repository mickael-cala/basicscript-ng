# REPRISE — point d'entrée pour reprendre ce dépôt

Ce document est fait pour être lu **en premier**, par une personne ou par un
agent, sans rien connaître du projet. Il dit ce qui marche, comment le
vérifier, et où regarder.

---

## 1. En trente secondes

- **Quoi** — ScriptBasic, un interpréteur BASIC écrit en C (LGPL 2.1, Peter
  Verhas), compilé ici avec `zig cc` pour six plateformes.
- **Où** — `src/` pour le code, `build.py` pour compiler, `tests/` pour
  vérifier.
- **État** — l'interpréteur se construit pour les six cibles ; il est **prouvé
  en exécution** sous Windows x86, Windows x64 et Linux x64. Le serveur HTTP
  `sbhttpd` se construit et sert sous Windows x64 et Linux x64. Neuf modules
  se construisent et passent leurs tests : `hash`, `re`, `t`, `trial`, `cgi`,
  `mt` partout, `cio` et `win` sous Windows, `unix` sous Linux/macOS. Les onze
  autres attendent une bibliothèque tierce, une ABI particulière, ou ne sont
  pas des modules (`dbg`).
- **Prochaine action** — voir [ROADMAP](ROADMAP.md) P0 (publication) puis P2
  (générateur de `error.bas`, décision sur `dbg`).

## 2. Construire et tester

```sh
python build.py --list                     # les cibles
python build.py --target windows-x64       # l'interpréteur d'une cible
python build.py --all                      # les six (≈150 s à froid)
python build.py --target windows-x64 --modules   # hash, re, t, trial, cgi, mt, cio, win
python build.py --target windows-x64 --modules --release   # sans trial
python build.py --target windows-x64 --httpd     # serveur sbhttpd
python build.py --all --modules            # les modules des six cibles
python build.py --target windows-x64 --standalone   # executable autonome (-E)
python build.py --target windows-x64 --stndlone     # second chemin (-C + stndlone)
python build.py --target windows-x64 --libscriba    # archive statique
python build.py --target windows-x64 --re-engine    # essai C du moteur Spencer
python build.py --clean                    # efface obj/ et dist/
python tests/run.py --target windows-x64   # 20/20 attendu (onze scripts, neuf refus)
python tests/run.py --target windows-x64 --modules   # 19/19 attendu
python tests/run.py --target windows-x64 --upstream   # 12 lances, 20 sautes
python tests/run.py --target windows-x64 --upstream-all   # 15 lances, 17 sautes
python tests/run.py --target windows-x64 --re-engine   # moteur re, essai C direct
python tests/run.py --target windows-x64 --cgi-post   # POST CGI urlencoded + multipart ASCII/binaire/depassement
python tests/run.py --grammaire            # controle statique, sans binaire
python tests/run.py --target linux-x64     # passe par WSL automatiquement
```

Zig est cherché dans `--zig CHEMIN`, `$ZIG`, puis
`C:\outils\zig-x86_64-windows-0.17.0\zig.exe`, puis `PATH`.

Ce que « ça marche » veut dire :

| commande | résultat attendu |
|---|---|
| `build.py --all` | `6/6 cible(s) construite(s)` |
| `build.py --all --modules` | `6/6` aussi, modules compris |
| `build.py --target windows-x64 --standalone` | `hello_standalone.exe`, sortie de `hello.bas` |
| `build.py --target windows-x64 --stndlone` | `hello_stndlone.exe`, sortie de `hello.bas` |
| `build.py --target windows-x64 --libscriba` | `libscriba.a` (47 objets) + `include/` |
| `build.py --target windows-x64 --re-engine` | `re_engine.exe`, 17 essais, `moteur re : OK` |
| `tests/run.py --target windows-x64` | `20/20 test(s) reussi(s)` |
| `tests/run.py --target windows-x64 --modules` | `19/19` |
| `tests/run.py --target windows-x64 --upstream` | `12 lance(s), 0 echec(s), 20 saute(s)` |
| `tests/run.py --target windows-x64 --upstream-all` | `15 lance(s), 0 echec(s), 17 saute(s)` |
| `tests/run.py --target windows-x64 --standalone` | `OK hello_standalone` |
| `tests/run.py --target windows-x64 --stndlone` | `OK hello_stndlone` |
| `tests/run.py --target windows-x64 --re-engine` | `OK re_engine` |
| `tests/run.py --target windows-x64 --cgi-post` | `22/22 cas CGI POST reussi(s)` |
| `tests/run.py --grammaire` | `grammaire : OK` (475 noms, 11 mots du preprocesseur, 0 NOTIMP arme) |
| `tests/run.py --target windows-x86` | idem |
| `tests/run.py --target linux-x64` | `20/20` par défaut, `18/18` avec `--modules`, via WSL |

Voir [ERRATA E3](ERRATA.md#e3) : « 20/20 » mesure le coeur (onze scripts et
neuf refus à l'analyse) ; « 19/19 » (Windows) ou « 18/18 » (Linux/macOS)
ajoutent les modules construits. Ni l'un ni l'autre ne couvre les modules
restants.
`tests/manuel.bas` se lance à part (`--test manuel`) : `156/156` assertions.

`--upstream` est une autre mesure : il exécute 12 tests de la suite d'origine
(9 du coeur et 3 des modules `hash`, `cgi` et `mt`) et saute 20 fichiers avec
une raison. `--upstream-all` en exécute 15. Ce n'est pas un « 12/12 » de plus :
beaucoup de ces programmes sont des démonstrations sans assertion, et le
manifeste dit lesquels sont hors jeu ([ERRATA E4](ERRATA.md#e4)).

`--cgi-post` exige `sbhttpd` (`--httpd`) et le module `cgi` (`--modules`).
Il ne remplace pas `tests/cgi.bas`, qui vérifie le GET et les fonctions du
module : il couvre le chemin POST, d'abord par `scriba -c`, puis par le
serveur, avec une configuration temporaire et un port libre. Onze cas sont
exercés : `application/x-www-form-urlencoded` ; `multipart/form-data` avec un
petit fichier ASCII ; `multipart/form-data` avec 65536 octets binaires,
zéros compris (`cgi_upload.bas`, `SaveFile` et contenu relu sur disque) ;
`multipart/form-data` avec un fichier de FILE_MAX + 1 octets, refusé par le
module avec `CGI_ERROR_FILEMAX` (`0x00080008`) avant la première ligne du
script ; le même refus avec 256 Ko de plus, qui laisse le reste du corps dans
la socket et vérifie donc la fermeture propre ; un corps annoncé au-delà de
CONTENT_MAX, refusé sur le seul en-tête avec `CGI_ERROR_BIG_CONTENT`
(`0x00080002`) ; et `tests/err_runtime.bas`, qui échoue à l'exécution avec le
code 22 (`0x16`) ; `tests/err_load.bas`, dont l'`include` est introuvable,
donc une erreur de chargement rapportée par le lecteur ; et
`tests/err_module.bas`, qui refuse un `cgi::SaveFile` et rend `0x00080000`,
au-delà de `MAX_ERROR_CODE`. Ces trois derniers exigent un `500` et la page
d'erreur complète, ce qui fige les trois chemins du rapporteur
([ERRATA E24](ERRATA.md#e24)). Deux cas couvrent l'erreur tardive
([ERRATA E25](ERRATA.md#e25)) : `tests/err_pending.bas` n'a pas encore
envoyé sa première ligne et garde donc droit au `500` et à la page, alors
que `tests/err_partial.bas` a déjà écrit son statut et `debut` : les deux
rapporteurs n'ajoutent plus que le diagnostic, et le lanceur interdit
`<HTML>` et `Content-Type: text/html` dans les deux modes, plus `HTTP/1.0`
dans le corps du serveur. En mode `scriba -c`, le rapporteur du CLI suit la
même règle mais écrit chaque octet aussitôt : `err_pending.bas` y a donc
déjà envoyé `debut` et ne reçoit pas de page, contrairement au serveur. Le serveur répond `413
Request Entity Too Large` dans les trois cas de refus, et le lanceur
l'exige ; pour les erreurs qui n'ont rien écrit il exige `500` et la page
HTML.
Le test du serveur ajoute `errmsgdest 3` à sa configuration
temporaire, sinon `sbhttpd` garde ses erreurs pour le journal et le client ne
reçoit ni statut ni corps ([ERRATA E23](ERRATA.md#e23)).

`--grammaire` ne lance rien : il relit `src/syntax.c` et `src/syntax.h`, et
échoue si un nom de la grammaire pointe sur un emplacement vide de
`CommandFunction[]` (la classe de l'erreur 28), si un `CMD_` n'a pas de
`#define`, ou si un `NOTIMP_*` est armé. Les onze mots du préprocesseur sans
corps y sont nommés un par un ; un nom nouveau sans corps fait tomber le
contrôle au lieu de passer inaperçu.

## 3. Carte du code

| vous cherchez… | regardez |
|---|---|
| l'analyse lexicale (mots, nombres, chaînes) | `src/lexer.c` |
| l'analyse syntaxique et l'arbre | `src/syntax.c` |
| la fabrication du bytecode | `src/builder.c` |
| l'exécution du bytecode | `src/execute.c` |
| l'évaluation des expressions | `src/expression.c` |
| les commandes natives (`PRINT`, `IF`, `LET`…) | `src/commands/` |
| la mémoire de l'interpréteur (tas, libération groupée) | `src/memory.c`, `src/myalloc.c` |
| le chargement des modules dynamiques | `src/modumana.c`, `src/dynlolib.c` |
| l'API pour embarquer l'interpréteur | `src/scriba.c`, `src/scriba.h` |
| l'API pour écrire un module | `src/basext.c`, `src/basext.h` |
| la configuration (`scriba.conf`) | `src/conftree.c`, `src/confpile.c`, `src/lsp.c` |
| les erreurs et leurs codes | `src/errcodes.c`, `src/errcodes.h` |
| le point d'entrée de l'interpréteur | `src/variations/standard/scribacmd.c` |
| le serveur HTTP | `src/httpd.c`, `src/variations/httpd/` |
| les modules (20) | `src/extensions/<nom>/interface.c` |
| le pilote de compilation | `build.py` (`SOURCES`, `TARGETS`, `HTTPD_*`) |

`src/README.md` et `src/License.txt` viennent de l'amont.

## 4. Fabriquer un exécutable autonome

Trois formes intégrées à `build.py` depuis le 2026-10-24 :

- `--standalone` : `scriba -E`, sans compilateur (recommandé) ;
- `--stndlone` : `scriba -C` puis `stndlone.o` et le coeur (second chemin) ;
- `--libscriba` : archive statique `libscriba.a` et en-têtes, pas un exécutable.

### A. `--standalone` (recommandé)

```sh
python build.py --target windows-x64 --standalone
# -> dist/windows-x64/hello_standalone.exe
./dist/windows-x64/hello_standalone.exe

python build.py --target windows-x64 --standalone chemin/vers/prog.bas
# -> dist/windows-x64/prog_standalone.exe
```

`--standalone` construit `scriba` d'abord s'il manque, puis appelle
`scriba -E` : l'interpréteur est recopié et le bytecode est ajouté à la fin.
Le PE x64 de `hello.bas` fait 609 051 octets et affiche le résultat attendu.
Quand `scriba` est déjà construit, aucun compilateur n'est nécessaire ; s'il
manque, le pilote le construit d'abord. Le binaire contient l'interpréteur et
le bytecode, **pas les modules** : un script qui importe `hash`, `re`… doit
être lancé depuis `dist/<cible>` pour retrouver `modules/` et `scriba.conf`.
La cible doit être exécutable ici : Windows x86/x64 et Linux x64 sont vérifiés ;
macOS n'a pas été exécuté. `--standalone` refuse `--all`, `--modules` et
`--httpd`. La sortie de `hello.bas` se contrôle sans script ni compilateur :

```sh
python tests/run.py --target windows-x64 --standalone
# -> OK      hello_standalone
```

### A bis. `scriba -E` à la main

```sh
./dist/windows-x64/scriba.exe -E -o hello.exe tests/hello.bas
./hello.exe
```

C'est exactement ce que fait le pilote ; la ligne directe reste utile pour
un script hors du dépôt ou pour comprendre le mécanisme.

### A ter. `--stndlone` (second chemin, avec compilateur)

```sh
python build.py --target windows-x64 --stndlone
# -> dist/windows-x64/hello_stndlone.exe
./dist/windows-x64/hello_stndlone.exe

python tests/run.py --target windows-x64 --stndlone
# -> OK      hello_stndlone
```

Le pilote écrit le C par `scriba -C`, compile `src/stndlone.c`, exclut
`scribacmd.o` (son `main`) et `lmt_none.o` (`stndlone.c` définit déjà
`StaticallyLinkedModules`), puis lie le tout. Le produit ne contient pas de
`.bas` à l'exécution. Ce chemin exige un compilateur, contrairement à
`--standalone`.

### A quater. `--libscriba` (archive)

```sh
python build.py --target windows-x64 --libscriba
# -> dist/windows-x64/libscriba.a
# -> dist/windows-x64/include/*.h
```

L'archive contient le coeur, les commandes, `lmt_none.c` et `libscriba.c`
(47 objets) ; ce n'est **pas** un exécutable. Sous UNIX, l'hôte qui la lie
doit fournir `char **_environ;` pour que l'archive ne tire pas `libscriba.o`
et son `main()`.

### B. En deux temps, à la main

```sh
# 1. bytecode + main() en C
./dist/windows-x64/scriba.exe -C -o /tmp/hello.c tests/hello.bas

# 2. compilation du C genere (C23, aucun -include necessaire)
ZIG="C:\outils\zig-x86_64-windows-0.17.0\zig.exe"
"$ZIG" cc -target x86_64-windows-gnu -std=c23 -DWIN32 -O2 \
  -c /tmp/hello.c -o /tmp/hello.o

# 3. le runtime autonome
"$ZIG" cc -target x86_64-windows-gnu -std=c23 -DWIN32 -O2 -Isrc \
  -include stdio.h -include stdlib.h -include string.h \
  -include ctype.h -include time.h -c src/stndlone.c -o obj/windows-x64/stndlone.o

# 4. le lien : tout le coeur SAUF scribacmd.o (son main) et lmt_none.o
#    (sa liste de modules, que stndlone.c definit aussi)
OBJS=$(ls obj/windows-x64/*.o | grep -v -e variations_standard_scribacmd -e lmt_none)
"$ZIG" cc -target x86_64-windows-gnu -lkernel32 -luser32 -lws2_32 -ladvapi32 \
  $OBJS /tmp/hello.o -o /tmp/hello_standalone.exe
```

Piège restant : [E9](ERRATA.md#e9) (symboles en double). [E8](ERRATA.md#e8)
est corrigé depuis la passe C23, y compris la double définition de `_environ`
sous UNIX. Le produit affiche le résultat du programme.

## 5. Lancer le serveur

```sh
python build.py --target windows-x64 --httpd    # exige scriba deja construit
mkdir -p logs
./dist/windows-x64/sbhttpd.exe &
curl --http0.9 http://127.0.0.1:18080/index.bas   # -> sbhttpd OK
curl --http0.9 http://127.0.0.1:18080/inexistant   # -> introuvable
cat logs/pid.txt                                   # le PID du serveur
```

Arrêt : supprimer `logs/pid.txt` (le thread de garde s'arrête) ou tuer le PID.

`scriba.conf` est cherché **à côté de l'exécutable** (sous Windows) ; les
chemins `webtest/`, `logs/` sont relatifs au **répertoire courant**. Détails :
[E15](ERRATA.md#e15), [E6](ERRATA.md#e6). Sous Linux le serveur compile et
sert depuis le 2026-10-23 ([E7](ERRATA.md#e7)) ; macOS n'a pas été essayé.

## 6. Les invariants du dépôt

1. **Aucun binaire versionné.** `obj/`, `dist/`, `logs/`, `*.exe`, `*.dll`,
   `*.o` sont ignorés. Un clone neuf doit tout reconstruire.
2. **Aucun outil externe.** Pas de Perl, pas de TinyCC, pas de Jamal, pas de
   générateur d'en-têtes. `build.py` + `zig cc` suffisent.
3. **Les `.h` sont des sources.** On les édite à la main, et on garde le `.c`
   d'accord avec eux. Il n'y a plus de bloc `TO_HEADER:` à maintenir.
4. **Pas de fichier généré sans générateur.** Si un fichier est produit, le
   producteur est dans le dépôt (ou le fichier est documenté comme figé,
   cf. [E1](ERRATA.md#e1)).
5. **Commentaires de code en français, sans accents** (ASCII), pour rester
   lisibles dans n'importe quel terminal ; les documents `.md` sont en UTF-8
   avec accents.
6. **`examples/` et `tests/upstream/` sont de l'amont** : on ne les reformate
   pas. On ajoute des tests ailleurs.
7. **Toute modification se termine par** `build.py` (au moins la cible hôte)
   et `tests/run.py` verts.

## 7. Ce qui est prouvé, et ce qui ne l'est pas

| cible | compilé | exécuté | par qui |
|---|---|---|---|
| Windows x86 | oui | oui, 19/19 | cette machine |
| Windows x64 | oui | oui, 19/19 | cette machine |
| Linux x64 | oui | oui, 18/18 | WSL Ubuntu 26.04 |
| Linux arm64 | oui | **non** | pas d'émulateur ici |
| macOS x64 | oui | **non** | — |
| macOS arm64 | oui | **non** en local ; oui en CI | GitHub Actions |

« Compilé » = les unités compilent, le lien réussit, `file` donne le bon
format. Voir [E5](ERRATA.md#e5).

Le serveur : Windows x64 prouvé (page, page 404, journaux) ; Windows x86
prouvé lors de la campagne TCC ; Linux x64 prouvé (page servie par
`curl --http0.9`) ; macOS non.

Les modules : neuf sont construits en bibliothèques partagées et **chargés
puis exercés** sous Windows x86, Windows x64 et Linux x64 (`--modules`).
`dbg` est un préprocesseur, pas un module. Les onze autres ne sont pas
construits. Détail et pièges :
[MODULES.md § 6](MODULES.md#6-construire-les-modules-aujourdhui),
[E18](ERRATA.md#e18), [E19](ERRATA.md#e19), [E20](ERRATA.md#e20),
[E21](ERRATA.md#e21), [E22](ERRATA.md#e22).

## 8. Par où commencer selon l'objectif

| objectif | lire | faire |
|---|---|---|
| publier sur GitHub | [ROADMAP](ROADMAP.md) P0 | ajouter le remote, pousser, taguer |
| faire marcher `import hash.bas` | [MODULES](MODULES.md#6-construire-les-modules-aujourdhui) | fait ; ajouter les modules à bibliothèque tierce, `odbc` et `dyc` |
| servir des pages sur Linux | [E7](ERRATA.md#e7) | fait le 2026-10-23 ; reste le mode service Windows et le test multi-thread |
| produire un exécutable autonome | § 4 ci-dessus | fait : `--standalone` et `--stndlone` (P5) |
| documenter les modules | [MODULES](MODULES.md) | écrire `pod2md.py` (P4) |
| comprendre une erreur du portage | [ERRATA](ERRATA.md) | les identifiants `E1`…`E22` sont stables |
| reprendre le travail avec un agent | [AGENTS.md](../AGENTS.md) | les consignes y sont |

## 9. Lexique

- **coeur** — l'interpréteur sans point d'entrée : analyse, bytecode,
  exécution, mémoire, modules.
- **variation** — le fichier qui apporte `main()`, la licence et la liste des
  modules liés (mot du projet d'origine).
- **module** / **extension** — bibliothèque partagée chargée par `import` :
  `re`, `hash`, `cgi`, `mt`…
- **bytecode** — la représentation intermédiaire produite par `builder.c` et
  exécutée par `execute.c` ; c'est elle que `-C` écrit en tableaux C.
- **configuration binaire** — `scriba.conf`, écrite par `scriba -k` à partir
  du texte `scriba.conf.lsp` ; spécifique à l'ABI (tailles de `long`).
