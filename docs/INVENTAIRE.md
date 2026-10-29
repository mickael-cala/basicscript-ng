# INVENTAIRE — ce que contient le dépôt, et ce qu'il ne contient pas

État au **2026-10-26** (mise à jour pour la fusion scriptbasic2-ng). Les
nombres sont ceux de Git : les produits (`obj/`, `dist/`, `logs/`) sont
ignorés et donc exclus.

## 1. Vue d'ensemble

```
scriptbasic2-ng/
├── build.py                 le seul outil de compilation (6 cibles, sbhttpd, modules)
├── pod2md.py                la fabrique de documentation (lit le C, écrit le markdown)
├── scriba.conf.lsp          configuration en texte
├── README.md                présentation et démarrage rapide
├── LICENSE                  GNU LGPL 2.1 (licence d'origine)
├── AGENTS.md                consignes pour un agent qui reprend le dépôt
├── .gitignore .gitattributes
├── .github/workflows/build.yml
├── scripts/                 gen_grammaire.py, le générateur de docs/GRAMMAIRE.md
├── src/                     le code C
│   └── vendor/              zlib 1.3.1, en-têtes curl 8.21.0 et unixODBC (intouchables)
├── tests/                   les programmes d'essai + run.py
│   ├── contrat/             les contrats snapshot (.bas + .out)
│   └── upstream/            la suite d'origine, telle quelle, et son manifeste
├── examples/                les exemples d'origine
├── webtest/                 deux pages servies par sbhttpd
└── docs/                    ces documents, plus PLAN.md (la feuille de route de la fusion)
```

## 2. Comptes

<!-- DEBUT COMPTES ENGENDRES - scripts/inventory.py -->

| dossier | fichiers | nature |
|---|---|---|
| `src/vendor/` | 49 | zlib 1.3.1, en-tetes curl 8.21.0 et unixODBC - intouchables, avec leur README de provenance |
| `src/` | 183 | le code C : 93 `.c`, 55 `.h`, 20 `.bas` de modules, donnees (`regex.h`, `notunix.txt`, `copyright.txt`...) |
| `tests/contrat/` | 18 | les contrats snapshot (`.bas` + `.out`), compares octet par octet |
| `tests/upstream/` | 33 | la suite d'origine, telle quelle, et son manifeste |
| `tests/` | 61 | les programmes d'essai, le harnais du manuel et `run.py` |
| `docs/modules/` | 15 | pages engendrees par `pod2md.py` |
| `docs/` | 16 | la memoire du depot (dont PLAN, ERRATA, et ANNEXE-GRAMMAIRE engendree par `scripts/docs.py`) |
| `examples/` | 209 | les exemples d'origine (copie de l'amont, ne pas reformater) |
| `include/` | 3 | `error.bas`, `time.bas`, `modinst.bas` |
| `webtest/` | 2 | `index.bas`, `404.bas` |
| `scripts/` | 5 | l'outillage documentaire : `gen_grammaire.py`, `docs.py`, `arch.py`, `inventory.py` |
| `.github/` | 1 | le workflow de CI (en pause, PLAN phase 5) |
| racine | 8 | `build.py`, `pod2md.py`, `scriba.conf.lsp`, `README.md`, `AGENTS.md`, `LICENSE`, `.gitignore`, `.gitattributes` |

Total : **603 fichiers versionnes**, dont **307 fichiers `.bas`** (modules, tests, exemples, pages web d'essai et `include/`). `GRAMMAIRE.md`, `MANUEL.md`, `ANNEXE-GRAMMAIRE.md` et la table ci-dessus sont engendres (`scripts/gen_grammaire.py`, `pod2md.py`, `scripts/docs.py`, `scripts/inventory.py`) et versionnes avec eux.

<!-- FIN COMPTES ENGENDRES -->

## 3. Les sources C, en détail

| catégorie | fichiers | rassemblés dans `build.py` |
|---|---|---|
| coeur | 26 | `SOURCES` |
| commandes natives | 14 | `SOURCES` (`src/commands/`) |
| utilitaires du coeur (logger, threads, handlers, licence) | 6 | `SOURCES` |
| point d'entrée standard | 1 | `SOURCES` (`variations/standard/scribacmd.c`) |
| serveur HTTP | 4 | `HTTPD_EXTRA` |
| modules liés au serveur (`cgi`, `mt`) | 3 | `HTTPD_EXTRA`, avec `-DSTATIC_LINK=1` |

Total compilé pour `scriba` : **47 unités** ; pour `sbhttpd` : **52** (les 45
du coeur, sans `scribacmd.c` ni `lmt_none.c`, plus les 7 du serveur).
L'union des deux listes fait 54 fichiers.

`build.py --modules` ajoute **14 unités** : les `interface.c` de `hash`, `re`,
`t`, `trial`, `cgi`, `mt`, `cio`, `win`, `unix`, le moteur `cgi.c`, et les
quatre fichiers du moteur de `re` (`regcomp.c`, `regexec.c`, `regerror.c`,
`regfree.c`). `cgi` et `mt` sont aussi liés en statique dans `sbhttpd` ; les
autres ne sont liés à aucun exécutable : ce sont des bibliothèques chargées
à l'exécution.

`build.py --libscriba` assemble 47 objets : les 47 unités de `scriba` moins
`scribacmd.c`, plus `variations/standalone/libscriba.c` (et `lmt_none.c` reste
présent). `build.py --stndlone` compile `src/stndlone.c` et le C produit par
`scriba -C`, puis lie les 45 objets du cœur qui restent (sans `scribacmd.c`
ni `lmt_none.c`).

Les autres `.c` du dépôt ne sont dans aucune cible : les onze extensions
restantes (`zlib`, `curl`, `xml`, `gd`, `curses`, `mysql`, `psql`, `bdb`,
`odbc`, `dyc`, `dbg`), la variante de liste de modules `lmt_cio`, les
outils de référence (`tools/md5.c`, `tools/mt19937.c`), et quelques fichiers
de diagnostic ou sources d'en-têtes (`testalloc`, `testconf`, `cftd`,
`prepext`, `notimp`, `mynotimp`, `command`, `matchc`, `buildnum`).
`tools/fork.c` a été retiré le 2026-10-24 (internes Cygwin inutilisables) ;
les formes `basicdll`, `scribawx`, `lmt_wx` et `calldllbasic` l'avaient été
juste avant. Le détail et les décisions sont dans
[PROGRAMMES.md § 2.2](PROGRAMMES.md#22-sources-sans-cible--décision-daudit-2026-10-24).

## 4. Ce qui est ignoré par Git

| motif | pourquoi |
|---|---|
| `obj/` | objets intermédiaires, un sous-dossier par cible |
| `dist/` | binaires et `scriba.conf`, un sous-dossier par cible |
| `logs/` | journaux et fichier pid écrits par `sbhttpd` |
| `*.o *.obj *.exe *.dll *.pdb *.ilk` | sorties de compilation, où qu'elles soient |
| `__pycache__/`, `*.pyc` | Python |
| `.vscode/`, `.idea/`, `*.swp`, `.DS_Store` | éditeurs |

Règle : **le dépôt ne contient aucun binaire**. Un clone neuf doit tout
reconstruire avec `python build.py --all`.

## 5. Ce qui manque par rapport à l'amont, et pourquoi

Comparaison avec le dossier `ScriptBasic-master` d'origine. Les fichiers
`*.c` et `*.h` de la racine amont sont **tous** présents ici (46/46) ; ce qui
manque est de l'outillage, de la documentation ou des binaires.

| absent | nature | pourquoi |
|---|---|---|
| `html/`, `texi/` | manuels HTML/TeXinfo générés | produits, pas sources ; la source est dans les `interface.c` |
| `docs/` de l'amont | documentation du projet (PROGRESS, PATCHES, MANUEL…) | remplacée par ces documents, propres à ce portage |
| `Makefile*`, `make_*.jim`, `Makefile.nt.jam`, `configure.jim` | chaîne de build d'origine | remplacés par `build.py` |
| `*.jam` de build (20 `manual.texi.jam`) | modèles de manuels | supprimés, voir [MODULES.md](MODULES.md) |
| `headerer.pl`, `generrh.pl`, `esd*.pl`, `ssplit.pl`, `t2h.pl`… | chaîne Perl | l'aplatissement des en-têtes les a rendus inutiles |
| `setup.pl`, `setup.cmd`, `install*`, `modinst.sh`, `modmake*.cmd` | installation | hors périmètre ; à réécrire si un installeur est voulu |
| `*.def` (`linkedmodules.def`, `lmt_*.def`) | listes d'exports TinyCC | fabriquées pour TCC ; Zig n'en a pas besoin |
| `obj/`, `bin*/`, `modules*/` de l'amont | binaires 32/64 bits | non versionnables, reconstruits ici |
| `ZIP-SRC/`, `scriba-*.pl`, `scriba-rpm.spec` | empaquetage | à refaire le jour d'une release |
| `test/*.pl`, `test/mktest.pl` | pilote d'essai Perl | remplacé par `tests/run.py` |
| `variations/win32dll/`, `variations/wx/`, `lmt_wx.c`, `tools/calldllbasic.c` | formes historiques | retirées le 2026-10-24 : `basicdll` n'avait pas d'implémentation, `scribawx` demandait wxWidgets ([E21](ERRATA.md#e21), [E22](ERRATA.md#e22)) |
| `examples/sibawa/*.cmd` | script Windows de l'exemple | inutile au portage |

Les deux `.jam` encore présents dans `examples/sibawa/` (`sibawa.texi.jam`,
`templates.txt.jam`) sont **volontairement** conservés : ce sont les données
de l'application d'exemple SIBAWA, pas des modèles de documentation du build.
Aucun outil du dépôt ne les lit ; les régénérer demande Jamal. Détail :
[docs/README.md § 3.2](README.md#32-ce-qui-reste--2-fichiers-et-ce-quils-sont-vraiment).

## 6. Provenance

- 2026-10-26 : import **verbatim** dans scriptbasic2-ng depuis scriba-zig
  (branche `backup/pre-fusion-20261026`, commit `063572f`) ; `PLAN.md`,
  `pod2md.py` en racine et `scripts/gen_grammaire.py` sont propres à la
  fusion, la suite est pilotée par [PLAN.md](PLAN.md) ;
- 2026-10-26 (phase 2) : `src/vendor/` entre — zlib 1.3.1 (via -ng,
  tarball fossile officiel), en-têtes libcurl 8.21.0 et `.def` de la DLL
  MinGW, en-têtes unixODBC d'Ubuntu 26.04 — dossiers intouchables, chacun
  avec son `README.txt` de provenance ; les interfaces de `zlib`, `odbc`,
  `curl` et `dyc` viennent de -ng quand elles portent une correction ;
- code C, tests, exemples : distribution **ScriptBasic** de Peter Verhas
  (LGPL 2.1), reprise telle quelle sauf les corrections listées dans
  [EVOLUTIONS.md](EVOLUTIONS.md) ;
- `src/extensions/re/regex.h`, `regex2.h`, `cclass.h`, `cname.h`, `utils.h`,
  `engine.ci` : reconstitués lors du portage TCC (le moteur Spencer était
  livré sans ses en-têtes) ; ils portent l'en-tête de licence d'origine ;
- `src/variations/httpd/service.h` : écrit pour le portage, la distribution ne
  le fournissait pas alors que `service.c` et `websrv.c` l'incluent ;
- le reste (`build.py`, `tests/`, `webtest/`, docs) : écrit pour ce dépôt.
