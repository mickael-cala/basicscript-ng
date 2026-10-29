# AGENTS.md — consignes de travail

Ce fichier s'adresse à un agent (ou à un humain pressé) qui reprend le
dépôt. Il dit comment travailler ici, pas ce que fait le projet : pour cela,
lire [docs/REPRISE.md](docs/REPRISE.md), puis [docs/PLAN.md](docs/PLAN.md),
puis les autres documents de `docs/`.

## Le projet en une phrase

**ScriptBasic2-ng** : le noyau ScriptBasic (C, LGPL 2.1) en sources plates,
compilé par **`zig cc`** pour six cibles — la base de la fusion des deux
ateliers scriba-zig et ScriptBasic-master-ng, décrite dans
[docs/PLAN.md](docs/PLAN.md).

## Provenance et fusion

Le noyau (`src/`, `build.py`, `tests/`, `docs/`) est l'import verbatim de
scriba-zig (branche `backup/pre-fusion-20261026`, commit `063572f`).
Les apports de ScriptBasic-master-ng — modules zlib/curl/odbc/dyc, contrats
snapshot `.bas`/`.out`, génération des tables du manuel depuis `syntax.def`,
v1.0 — arrivent par phases. La feuille de route, les décisions d'architecture
et la liste des erreurs à ne pas reproduire vivent dans
[docs/PLAN.md](docs/PLAN.md) : c'est le contrat de la fusion, à lire avant
tout travail.

## Commandes de référence

```sh
python build.py --target windows-x64            # l'interpreteur
python build.py --all                           # les six cibles
python build.py --target windows-x64 --modules  # hash, re, t, trial, cgi, mt, zlib, odbc (Windows), bytes, dyc (32 bits), curl (64 bits), cio et win
python build.py --target windows-x64 --modules --release  # sans trial
python build.py --target windows-x64 --httpd     # le serveur (scriba d'abord)
python build.py --target windows-x64 --standalone  # executable autonome (hello.bas)
python build.py --target windows-x64 --stndlone    # chemin -C + stndlone
python build.py --target windows-x64 --libscriba   # archive statique + hotes embed + en-tetes
python build.py --target windows-x64 --standalone tests/standalone_modules.bas --modules  # autonome a modules colles
python build.py --target windows-x64 --re-engine   # essai C du moteur Spencer
python tests/run.py --target windows-x64         # 32/32 attendu (dimas inclus ; 31/31 sous UNIX, E44)
python tests/run.py --target windows-x64 --modules   # 27/27 attendu (dyc saute : 32 bits seulement)
python tests/run.py --target windows-x64 --upstream  # 12 lances, 20 sautes
python tests/run.py --target windows-x64 --upstream-all  # 15 lances, 17 sautes
python tests/run.py --target windows-x64 --standalone  # hello_standalone, sans .bas
python tests/run.py --target windows-x64 --stndlone   # hello_stndlone, sans .bas
python tests/run.py --target windows-x64 --re-engine  # moteur re, essai C direct
python tests/run.py --target windows-x64 --cgi-post  # POST CGI urlencoded + multipart ASCII/binaire/depassement
python tests/run.py --grammaire                  # statique : aucun nom sans corps
python tests/run.py --contrat                    # 9/9 contrats, cible apres cible
python tests/run.py --explicit                   # option explicit (declarevars 1)
python tests/run.py --embed                      # 3/3 hotes embed
python tests/run.py --target linux-x64           # via WSL automatiquement
python build.py --clean
python pod2md.py            # regenere docs/MANUEL.md et docs/modules/
python pod2md.py --check    # echoue si la doc a diverge des sources
python scripts/docs.py --check       # annexe grammaire a jour (syntax.c)
python scripts/inventory.py --check  # comptes de l'inventaire a jour
python scripts/arch.py               # produits conformes a leur cible
python scripts/exemples.py --target windows-x64 --write  # l'etat des exemples amont
```

Zig **0.17.0 épinglée** (installée localement en `C:\outils\zig\0.17.0`).
`build.py` la cherche dans cet ordre : `--zig CHEMIN`, la variable `ZIG`,
puis `C:\outils\zig\0.17.0\zig.exe`, puis `zig` dans le `PATH`.

Une modification n'est **finie** que si : `build.py` réussit pour la cible
concernée, `tests/run.py` passe sur au moins une cible exécutable ici, et les
documents concernés sont mis à jour.

## Interdits

- **Aucun TinyCC.** Le dépôt est un portage Zig ; les ateliers TCC vivent
  ailleurs (dépôts voisins) et ne font pas partie de celui-ci.
- **Aucune conversion de types « C23 » par substitution de texte**
  (`int`→`int32_t`, `long`→`int32_t`…) : les sources compilent déjà avec
  `-std=c23`, et ces réécritures cassent LP64 (`long` fait 64 bits sous
  Linux), `main`, les casts et les déclarations Windows. Leçon de la fusion
  ([docs/PLAN.md § 4](docs/PLAN.md)).
- **Aucun fichier `.jam`, `.jim`, `.pl` de build.** La chaîne Perl/Jamal a été
  retirée exprès ; ne pas la réintroduire « pour générer les en-têtes ».
  Seule exception : `examples/sibawa/*.jam`, qui sont des données de cet
  exemple (gabarits HTML, manuel) et non des modèles de build — voir
  [docs/README.md § 3](docs/README.md#3-les-fichiers-jam--combien-où-et-par-quoi-ils-sont-remplacés).
- **Aucun binaire versionné** : `obj/`, `dist/`, `logs/`, `*.exe`, `*.dll`,
  `*.o` sont ignorés. Ne pas les forcer avec `git add -f`.
- **Ne pas reformater** `examples/` ni `tests/upstream/` : ce sont des copies
  de l'amont, gardées telles quelles.
- **Ne jamais éditer un `.out` de contrat à la main** : il se régénère par
  l'interpréteur (`./dist/<cible>/scriba tests/contrat/x.bas >
  tests/contrat/x.out`), puis la divergence se discute — c'est elle qui
  raconte un changement de comportement.
- **Ne pas modifier un `.h` sans le `.c` qui l'accompagne** (et
  réciproquement) : rien ne les synchronise plus automatiquement.

## Conventions

- Commentaires **en français, sans accents** dans le code C et Python ;
  documents `.md` en français avec accents.
- Style existant : indentation à un espace dans le code d'origine (ne pas
  « corriger » en masse), deux à quatre espaces dans `build.py` et les scripts
  Python.
- Une correction de portage est **commentée sur place** (ce qui échouait avec
  TCC, pourquoi clang refuse) — voir les exemples dans `src/basext.h`,
  `src/extensions/mt/interface.c`, `src/variations/httpd/service.h`.
- Pas de dépendance nouvelle sans raison : `build.py` et `tests/run.py`
  n'utilisent que la bibliothèque standard de Python.

## Quand la documentation doit être mise à jour

| événement | document |
|---|---|
| nouveau programme, nouvelle cible | `docs/PROGRAMMES.md`, `README.md` |
| document ajouté ou renommé | `docs/README.md` (sommaire) |
| grammaire modifiée (`syntax.c`) | `scripts/docs.py --write` (ANNEXE-GRAMMAIRE) |
| module qui compile / ne compile plus | `docs/PROGRAMMES.md` § 4, `docs/MODULES.md` § 3 et § 6 |
| défaut trouvé, limite assumée | `docs/ERRATA.md` (nouvel identifiant `E…`, ne jamais réutiliser un numéro) |
| décision de conception | `docs/EVOLUTIONS.md` |
| décision de fusion, phase, périmètre | `docs/PLAN.md` |
| tâche terminée ou ajoutée | `docs/ROADMAP.md` (cocher / ajouter) |
| fichier ajouté ou retiré | `scripts/inventory.py --write` (comptes de INVENTAIRE) |

## Pièges à connaître (détail dans docs/ERRATA.md)

- `sbhttpd` sans en-têtes HTTP : `curl` récent refuse la réponse (E6).
- Le C produit par `scriba -C` est en C23 depuis la correction de E8 : ne pas
  y réintroduire de définition K&R ni de liste `()` vide.
- Lier ce C demande d'écarter `scribacmd.o` et `lmt_none.o` (E9).
- `--stndlone` fait exactement cela : `scriba -C`, puis `stndlone.o` + les 45
  objets du coeur ; ne pas réintroduire `lmt_none.o`, `stndlone.c` définit
  déjà `StaticallyLinkedModules`.
- `--libscriba` produit une archive, pas un exécutable ; sous UNIX l'hôte doit
  définir `char **_environ;` pour ne pas tirer `libscriba.o` et son `main()`.
- `basicdll` et `scribawx` ont été retirés le 2026-10-24, avec `lmt_wx.c` et
  `tools/calldllbasic.c` : `basicdll.c` ne contenait qu'une déclaration, sans
  implémentation (E21), et `scribawx` dépendait de wxWidgets, absent du dépôt
  (E22).
  `basicc.c` et `basiccmd.c` étaient des doublons historiques, remplacés par
  `stndlone` et `scribacmd`.
- `sbhttpd` sort en silence si `logs/` n'existe pas (E15).
- `--httpd` exige que `scriba` soit déjà construit (E14).
- Les modules `cgi` et `mt` se compilent avec `-DSTATIC_LINK=1`, sinon
  `versmodu`/`bootmodu`/`finimodu` sont en double au lien.
- `--modules` exige que `scriba` soit déjà construit (c'est lui qui charge),
  et se lance depuis `dist/<cible>` : les clés `module` et `include` sont
  relatives au répertoire courant.
- Sous UNIX, `scriba.conf` n'est pas cherché à côté du binaire : sans
  `SCRIBACONF`, l'interpréteur tourne avec une configuration vide et dit
  « module can not be loaded » (E19).
- `ScriptBasic` rend 0 même sur une erreur de syntaxe : un test ne peut pas se
  fier au code de sortie, il doit chercher `ECHEC` ou `error &H` (E3).
- Les modules se compilent avec `-fvisibility=hidden` ; une fonction destinée
  au BASIC doit passer par `besFUNCTION`/`besCOMMAND`, sinon elle est cachée
  (E18, `re` en a fait les frais).
- `curl` ne tourne qu'en 64 bits sous Windows : sa DLL d'exécution est celle
  de MinGW (dans le PATH), et la mémoire qu'elle rend se libère par
  `curl_free`, jamais par `free` (E40, E43).
- `odbc` est livré Windows seulement : la liaison Linux bute sur le sysroot
  de zig (E42). `dyc` est 32 bits (E10, E41).
- Le test d'un module absent d'une cible est **sauté avec raison** et sort
  du compte (`MODULE_TESTS_SKIP`) : un « SAUTE » n'est ni un OK ni un
  échec, et le ratio ne l'inclut pas.
- `manuel` n'est joué que sous Windows : ses attentes encodent LLP64
  (E44). Les formes historiques refusées à l'analyse (`open` sans `for`,
  `sub` dans `module`) sont notées à E45 — la passe langage les rouvrira
  avec contrat et refus.

## Git

- Ne pas committer ni pousser sans demande explicite.
- Messages en français, à l'impératif, une ligne de résumé puis un corps si
  nécessaire.
- Ne jamais toucher aux modifications d'un autre agent : `git status` d'abord.

## Où regarder pour…

| question | fichier |
|---|---|
| s'orienter, questions fréquentes | `docs/README.md` |
| la fusion, les phases, ce qui reste à faire | `docs/PLAN.md` |
| mots-clés, fonctions, numéros, renumérotation | `docs/GRAMMAIRE.md` |
| compiler | `build.py` (`SOURCES`, `TARGETS`, `HTTPD_EXTRA`) |
| tester | `tests/run.py`, `tests/*.bas` |
| configuration du runtime | `scriba.conf.lsp` |
| servir des pages | `webtest/`, `src/variations/httpd/websrv.c` |
| écrire un module | `src/basext.h`, puis `src/extensions/<nom>/interface.c` |
