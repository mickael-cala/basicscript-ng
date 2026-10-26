# PLAN.md — Feuille de route ScriptBasic2-ng

> Établi le 2026-10-26 après audit complet des quatre répertoires :
> `ScriptBasic-master` (dump historique), `ScriptBasic-master\scriba-zig`,
> `ScriptBasic-master-ng` et l'état réel de `scriptbasic2-ng`.
> Ce document est la référence de la fusion. Il remplace les intentions
> du README tant que les phases 0–5 ne sont pas bouclées.

---

## 0. Constat de départ (audit du 2026-10-26)

Le squelette actuel de `scriptbasic2-ng` n'est **pas un point de départ exploitable** :

| Élément | État réel |
|---|---|
| Code C | **Aucun** (un unique `src/main/commands.h` squelette de 35 lignes) |
| `scripts/build.py` | **Corrompu** : fragment PowerShell collé en tête + deux versions du script Python concaténées (723 lignes, `SyntaxError` dès la ligne 2) |
| `scripts/docs.py` | Bogué : `import sys` manquant (NameError prouvé), CLI régressé (positionnel vs `--write`/`--check` du README), parseur à code mort — la liste des fonctions est toujours vide |
| `syntax.def` | **Troisième format inventé**, incompatible avec le vrai format ScriptBasic (celui de `-ng`, 481 lignes) et avec le parseur de docs.py lui-même |
| `test/contrat/basic_syntax.bas` | Utilise `DIM a AS INTEGER`, `AS`, `INTEGER`, `STRING`… absents de syntax.def |
| `.gitignore` | Ignore `*.out` → exclurait les fixtures de contrat du versionnement |
| `src/vendor/`, `src/modules/*`, `resources/` | 13 dossiers vides |
| Git | **Aucun dépôt initialisé** |
| Zig | Non installé sur la machine |
| Restes | `finalisation.ps1` (bogué : `$modified` jamais armé dans la boucle C23), `scripts_test/` vide, et le premier jet abandonné `scriba-zig\ScriptBasic-ng\` (11 fichiers divergents) |

Conclusion : on ne « répare » pas ce squelette. On repart d'une base saine
(scriba-zig, déjà compilable et testée) et on y greffe les apports de `-ng`.
La conception de la fusion (quoi prendre d'où) reste valable ; son exécution
par scripts de copie + conversion regex est abandonnée (voir §5).

---

## 1. Décisions d'architecture — « quoi prendre d'où »

| Sujet | Choix | Source | Justification |
|---|---|---|---|
| Noyau C | `src/` de scriba-zig **verbatim** (93 .c / 55 .h, cœur 45 565 lignes) | scriba-zig | Seule version : headers plats (zéro `TO_HEADER:`), déjà compilée par `zig cc -std=c23`, campagne 30/30 verte |
| C23 | Flag compilateur uniquement. **Aucune conversion de types par regex** | scriba-zig | Le code compile déjà en C23 ; remplacer `int`→`int32_t`, `long`→`int32_t` casserait l'ABI LP64 (long = 64 bits sous Linux) et 45 000 lignes de C |
| Build | `build.py` de scriba-zig (1 015 lignes) étendu, pas réécrit | scriba-zig | Contient les leçons durement acquises : `-include stdio.h…` (les .h plats n'incluent pas leurs dépendances), `-Wno-*` ciblés, mtime des headers (E38), conf par ABI via WSL, incremental |
| Cibles | 6 cibles zig natives : windows-x86/x64, linux-x64/arm64, macos-x64/arm64 | scriba-zig | La cross-compilation zig remplace toute la machinerie qemu de `-ng` ; riscv64/armv7 ajoutables plus tard pour ~5 lignes |
| Tests — moteur | `tests/run.py` de scriba-zig (1 519 lignes) | scriba-zig | 30 essais dont 17 refus codifiés, `--modules`, `--upstream`, `--cgi-post`, `--grammaire`, `--standalone` |
| Tests — corpus | Corps BASIC de `-ng` : `manuel.bas` (114 exemples), `test/*.bas`, essais extensions | `-ng` | Volume de couverture langue que scriba-zig n'a pas ; entré comme snapshots `.bas`+`.out` (phase 3) |
| Documentation — prose | `pod2md.py` (POD dans le C → MANUEL.md, 212 entrées + docs/modules/) | scriba-zig | Cohérent avec les headers plats : la doc vit dans le C |
| Documentation — tables | `docs.py` **réécrit** pour parser le VRAI `syntax.def` de `-ng` (481 lignes) et générer les tables du manuel entre marqueurs, avec vérif croisée `syntax.def` ↔ `syntax.c` | `-ng` | C'est le vrai « doc as code » : grammaire autoritaire + vérification programmatique (`sb_doc`/`sb_docsrc` de `-ng` servent de cahier des charges) |
| Modules — base (9) | hash, re, t, trial, cgi, mt, cio, win, unix | scriba-zig | Zéro dépendance tierce, déjà construits et testés |
| Modules — ajoutés | zlib (vendor 1.3.1, 32+64 bits prouvé), odbc (Windows, E11 ouvert en 64), curl (headers vendor 8.21.0, runtime système), dyc (source conservé, exclu ARCH 32) | `-ng` | Demandés explicitement ; état réel documenté dans `-ng` |
| Modules — exclus | dbg (coquille vide, retiré dans `-ng`), bdb/curses/gd/mysql/psql/xml (tierces non planifiées, E13) | `-ng` | — |
| Renommage `t`→`serialize` | Différé (v1.1), pas bloquant pour v1.0 | `-ng` | Le renommage existe dans `-ng` avec ses tests ; à porter en un commit dédié une fois la base verte |
| Vendors | `src/vendor/zlib/` (sources), `src/vendor/curl/include/` (headers) — INTANGIBLES | `-ng` | Règle déjà inscrite dans AGENTS.md |
| Validation d'architecture | `scripts/arch.py` : lecture PE `e_machine` / ELF, refus d'un binaire renommé | `-ng` (portage de `sb_arch`) | Invariant « un produit ment rarement sur son arch » ; en Python ici, pas en C |
| Inventaire | `scripts/inventory.py` générant la section inventaire de docs/REGLES.md entre marqueurs | `-ng` (simplifié) | Rend réelle la règle n°4 d'AGENTS.md |
| Git | Dépôt frais, commits atomiques conformes à AGENTS.md, `.gitattributes` figeant les fins de ligne | `-ng` | Leçon `b8dc114` (« les fins de ligne des scripts exécutés sont figées ») |
| Version Zig | 0.17.0 épinglée (URL + sha256), script d'installation porté | scriba-zig + `-ng` | `setup-zig.sh` de `-ng` prouve la recette ; `-std=c23` exige un clang récent |

## 2. Phases

### Phase 0 — Assainissement et gardes-fous (préalable absolu)

1. **Sauvegarder le travail non commité** : scriba-zig n'a qu'un seul commit et
   toute la vague E23–E39 / passes 10–24 est dans l'arbre de travail ;
   `-ng` porte 71 fichiers modifiés + 9 non suivis sur `vibe/curl`.
   Dans chacun : `git add -A && git commit` sur une branche de sauvegarde
   (ou archive zip). Ne rien perdre avant toute manipulation.
2. **Purger le squelette scriptbasic2-ng** : supprimer `scripts/build.py`
   (corrompu), `scripts/docs.py` (régressé), `syntax.def` (3e format),
   `finalisation.ps1`, `scripts_test/`, `src/main/commands.h`,
   `test/contrat/basic_syntax.{bas,out}` (incohérents avec la grammaire),
   ainsi que `README.md`, `AGENTS.md`, `REGLES.md`, `docs/EVOLUTIONS.md`
   et `.gitignore` — remplacés en phase 1 par les versions éprouvées de
   scriba-zig, adaptées. Le squelette est archivé au préalable dans le
   commit initial du dépôt fraîchement initialisé.
3. **Corriger `.gitignore`** : retirer l'exclusion `*.out` (ou la réancrer
   pour ne viser que les produits) ; ajouter `!test/contrat/**`.
4. **Supprimer le premier jet** `scriba-zig\ScriptBasic-ng\` (11 fichiers
   divergents, non suivis) après vérification qu'il ne contient rien
   d'unique (l'audit du 2026-10-26 confirme : tout est soit obsolète, soit
   régénérable).
5. **Installer Zig 0.17.0** épinglée (recette `setup-zig.sh` de `-ng`,
   adaptée Windows : zip officiel + sha256, ex. `C:\outils\zig\0.17.0`).
6. `git init` dans scriptbasic2-ng + commit initial du squelette nettoyé.

**Critère de sortie** : dépôt git propre, `zig version` → 0.17.0, aucun
fichier corrompu restant.

### Phase 1 — Noyau scriba-zig tel quel (v0.1)

1. Copier **sans modification** : `src/` (noyau + commands + extensions +
   variations + tools), `build.py`, `pod2md.py`, `tests/` (run.py + .bas +
   upstream + re_engine.c + MANIFEST.tsv), `include/`, `scriba.conf.lsp`,
   `LICENSE`, `.gitignore` et les docs utiles (ERRATA, GRAMMAIRE, ROADMAP) ;
   importer aussi `AGENTS.md` et `README.md` adaptés à scriptbasic2-ng
   (ils remplacent les squelettes supprimés en phase 0).
2. Nettoyer en copiant : ne pas emporter `panic.txt`, `__pycache__/`,
   `obj/`, `dist/`, `logs/` (produits/déchets).
3. Adapter uniquement les chemins si besoin (aucun prévu : arborescence
   conservée à l'identique — pas de déplacement `src/`→`src/main/`).
4. `python build.py --target windows-x64` puis
   `python tests/run.py --target windows-x64` → **30/30 attendu**.
5. `python build.py --target windows-x64 --modules` → **21/21 attendu**.
6. `python pod2md.py --check` → OK.
7. Commit par bouchon atomique (`feat: noyau scriba-zig importé tel quel`,
   `feat: campagne de tests importée`, `docs: ERRATA et grammaire importés`).
8. Corriger au passage deux scories héritées : la table sommaire d'ERRATA
   (n'a que E1–E31, le corps va jusqu'à E39) et le paramètre mort
   `--jobs` (l'implémenter ou le retirer).

**Critère de sortie** : windows-x64 compilé + 30/30 + 21/21 modules +
doc vérifiée, premier run CI (porter `.github/workflows/build.yml`).

### Phase 2 — Modules complémentaires de `-ng` (v0.2)

1. **zlib** : vendoriser les sources zlib 1.3.1 dans `src/vendor/zlib/`
   (depuis `-ng/src/modules/zlib/`), porter le module `zlib` de `-ng`
   dans `src/extensions/zlib/`, ajuster includes/lien dans build.py
   (`-Isrc/vendor/zlib`, lien statique vers les objets vendor).
   Test : `-ng/test/` essai zlib rejoué sur windows-x64 et linux-x64.
2. **curl** : vendoriser les headers 8.21.0 (`-ng/resources/curl/`) dans
   `src/vendor/curl/include/` ; porter le module curl de `-ng` ; décision
   runtime : liaison vers libcurl **système** (documentée, comme `-ng` ;
   la vendorisation du binaire est exclue du périmètre v1). Cible
   windows-x64 uniquement au départ (`vibe/curl` de `-ng` = cahier des
   charges, le module n'y est pas encore produit).
3. **odbc** : porter le module de `-ng` ; windows-x64 d'abord, E11
   (échec 64 bits connu) traité en boîte de temps bornée ; si ouvert,
   le documenter dans ERRATA et livrer windows-x86.
4. **dyc** : copier la source, exclusion `ARCH 32` documentée dans
   build.py (leçon d'ABI : `call` nu 32 bits, SIGSEGV garanti en 64).
5. Optionnel (peut glisser en v1.1) : renommage `t`→`serialize` (+ ses
   tests) en un commit dédié.

**Critère de sortie** : `--modules` étendu vert avec zlib (unix+win) et
curl/odbc (win-x64) ou leurs exclusions documentées dans ERRATA.

### Phase 3 — Contrats snapshot façon `-ng` (v0.3)

1. Créer `test/contrat/` au format `.bas` + `.out` (+ `.rc`, `.err` si
   besoin) : chaque script BASIC a sa sortie figée, comparaison octet par
   octet après normalisation des fins de ligne (`.gitattributes`).
2. Semer le corpus depuis `-ng` : découper `manuel.bas` (114 exemples) en
   contrats par thématique ; porter `test/*.bas` utiles ; convertir
   `retires.txt` en essais de refus (le mécanisme `REFUS_ANALYSE` de
   run.py existe déjà, il suffit d'ajouter les entrées).
3. Ajouter à `tests/run.py` un mode `--contrat` qui joue et compare.
4. Régénérer les `.out` avec l'interpréteur de la phase 1 (jamais écrits
   à la main) ; les fixture `test/contrat/**` sont **versionnées**.

**Critère de sortie** : campagne contrat verte sur windows-x64 et
linux-x64 (WSL), exécutée dans la CI.

### Phase 4 — Doc as code réelle (v0.4) — réalisée, amendée

**Amendement d'autorité** : adopter le `syntax.def` de `-ng` comme
référence créerait un faux oracle — les deux grammaires ont divergé
(95 terminaux déclarés chez `-ng`, 475 symboles dans notre `syntax.c`,
et des noms différents : `curdir` ici, `curdir$` là). L'autorité de la
phase 4 est donc **notre `src/syntax.c`** ; une grammaire déclarative
(`syntax.def`) fidèle au langage réel sera recréée à la passe langage
(phase 8), où les changements de grammaire la mettront à jour de
conserve.

1. `scripts/docs.py` : extrait les tables de `syntax.c` (symboles
   d'expression `CMD_`, mots-clés d'instruction avec leur id), vérifie la
   cohérence des codes avec `syntax.h`, et engendre
   `docs/ANNEXE-GRAMMAIRE.md` entre marqueurs, `--write`/`--check`
   déterministes (pas d'horodatage).
2. `scripts/arch.py` : lit chaque produit de `dist/` (PE, ELF, Mach-O) et
   refuse un binaire qui ment sur son architecture.
3. `scripts/inventory.py` : les comptes de `docs/INVENTAIRE.md` § 2 sont
   générés entre marqueurs depuis `git ls-files` ; les natures restent
   une décision écrite dans le script.

**Critère de sortie** : `docs.py --check`, `inventory.py --check`,
`arch.py` verts et reproductibles ; toucher `syntax.c` sans régénérer
l'annexe fait échouer la vérification. (Le branchement CI attend la
réactivation, phase 5.)

### Phase 5 — Fonctions killer et finitions (v1.0)

1. Confirmer/porter `--standalone` (`scriba -E` : interpréteur + bytecode
   embarqué), `--stndlone` (`scriba -C` + `stndlone.c`), `--libscriba`
   (archive statique + headers) — déjà fonctionnels dans scriba-zig ;
   leurs essais (`hello_standalone`, `hello_stndlone`) tournent déjà
   dans run.py.
2. CI élargie : build des 6 cibles + tests natifs (windows-x64, linux-x64
   via WSL) + doc + contrat + arch + inventory.
3. Pédagogie (recommandation héritée de l'audit `-ng`) : `docs/START_HERE.md`
   (premier pas guidé : build, test, ajout d'un mot-clé) + diagramme
   d'architecture (lexer→parser→bytecode→exécuteur→modules).
4. Consolidation docs : ERRATA à jour (table E1–E39+), ROADMAP, tag `v1.0.0`.

## 3. Pièges hérités à respecter (extraits d'AGENTS/ERRATA/REGLES)

- **dyc** : 32 bits uniquement (ABI `call` nu). Exclusion `ARCH 32`, jamais un build 64.
- **odbc 64 bits** : E11 ouvert — échec constaté, à temps borner.
- **`waitpid`/`execute`** : exigent des lvalues pour PID et status (erreur `&H5c` sinon).
- **Vendors intangibles** : `src/vendor/` ne se modifie pas, se remplace par script dédié.
- **Opérateurs étendus** : les 256 opérateurs (`+^`, `!<`…) restent morts ; seul `#` survit.
- **Headers** : plat, maintenus à la main ; ne jamais toucher un `.h` sans le `.c`.
- **Fins de ligne** : `.gitattributes` dès le premier commit (leçon `-ng` b8dc114).
- **Conf Linux depuis Windows** : `scriba.conf` binaire généré via WSL (même ABI), réutiliser `make_config()` de build.py.
- **Flags** : conserver le jeu `-Wno-*` + `-include` de scriba-zig ; ne jamais ajouter `-Werror` global sur le noyau legacy.
- **Zig** : 0.17.0 épinglée ; `zig ar` n'existe pas en 0.17 → utiliser `zig cc` en mode archive ou l'`ar` de la chaîne.

## 4. Ce qu'on ne fera PAS (erreurs de la conversation Qwen à ne pas reproduire)

1. **Aucune « conversion C23 » par regex** (`int`→`int32_t`, `long`→`int32_t`…) :
   fausse pour LP64 (long=64 bits Linux), casse `main`, les casts, les
   déclarations Windows, et inutile — le code compile déjà avec `-std=c23`.
2. **Pas de `--standalone` maison** via `scriba -C file -o out.bc` (option
   inexistante) ni de `scriba_execute_bytecode()` (fonction inventée) :
   les chemins réels sont `-E` et `-C`+`stndlone.c`.
3. **Pas de copie en bloc** `src/* → src/main/` qui écraserait le layout
   (extensions/ et variations/ se retrouveraient sous main/) ni de
   contrat sbtool copié depuis `-ng` (ses 330 baselines testent la
   **parole de sbtool**, un outil qui n'existe pas ici).
4. **Pas de `syntax.def` réinventé** : le format autoritaire est celui de `-ng`.
5. **Pas de génération magique de headers** : règle n°3 d'AGENTS.md.
6. **Jamais revendiquer un fichier créé sans relecture** : chaque écriture
   est suivie d'une vérification (syntaxe Python, exécution, diff).

## 5. Estimation

| Phase | Contenu | Effort indicatif |
|---|---|---|
| 0 | Sauvegardes, purge, zig, git | 2–3 h |
| 1 | Import noyau + campagne verte + CI | 0,5–1 j |
| 2 | zlib (+ curl, odbc bornés) | 1–2 j |
| 3 | Contrats snapshot (corpus `-ng`) | 1–2 j |
| 4 | docs.py réel + arch + inventory | 1–2 j |
| 5 | Killer features déjà là + docs + tag v1.0.0 | 0,5–1 j |

Total réaliste : **5 à 9 jours** de travail effectif, l'essentiel du risque
étant concentré sur curl/odbc (phase 2).

---

## 6. Suivi d'exécution

| phase | quand | résultat |
|---|---|---|
| 0 — assainissement | 2026-10-26 | squelette Qwen archivé (`f41536b`) puis purgé ; Zig 0.17.0 épinglée (SHA-256 vérifié) installée en `C:\outils\zig\0.17.0` ; dépôt git initialisé |
| 1 — noyau scriba-zig | 2026-10-26 | import verbatim prouvé (`diff -rq` vide sur `src/`) ; build windows-x64 ; campagnes 30/30 et 21/21 ; `pod2md --check` vert ; CI des six cibles importée ; push GitHub (`master`), l'ancien contenu du distant archivé sur `archive/portage-20261026` |
| 2 — modules de -ng | 2026-10-26 | zlib vendorisé 1.3.1 (win32/64 + linux) ; odbc win32/64 avec E11 fermé (E42 ouvert : liaison Linux) ; dyc 32 bits (E41 : ébauche x64 écartée) ; curl win64 avec E43 corrigé (`curl_free`) ; vendors `src/vendor/` intangibles ; campagnes : 30/30 noyau ×3 cibles, modules 24/24 + 24/24 + 21/21, chaque saut documenté et hors compte ; 563 fichiers versionnés |
| 3 — contrats snapshot | 2026-10-26 | `tests/contrat/` : 8 contrats plateforme-neutres, 8/8 sur windows-x64, windows-x86 et linux-x64 (WSL) ; `manuel` (156 affirmations) entre en jeu Windows — 31/31, 31/31, 30/30 ; E44 (écarts LLP64) et E45 (strictures de grammaire) documentés |
| 4 — doc as code | 2026-10-26 | amendé : l'autorité est `syntax.c` (le `syntax.def` de `-ng` documenterait une autre langue) ; `scripts/docs.py` engendre ANNEXE-GRAMMAIRE (557 symboles, déterministe), `scripts/arch.py` vérifie 36/36 produits (PE/ELF/Mach-O), `scripts/inventory.py` génère les comptes ; défaut corrigé au passage : `zig_path` de build.py retenait le chemin Windows sans vérifier son existence (c'était la cause des échecs CI en 32 s) ; **CI en pause** (déclencheur manuel), réactivation = phase 5 |
| 5 — consolidation v1.0 | 2026-10-26 | CI rebranchée : première run complète (7 min) — windows ✓ vert d'emblée, ubuntu et macos révèlent E46/E47, périmètres restreints explicitement ; `COMPATIBILITE.md` écrite ; 178 exemples mesurés par `scripts/exemples.py` (6 écartés de sécurité après deux arrêts machine) ; tag **v1.0.0** posé après la première run CI verte sur les trois systèmes (run 40331258934 : ubuntu 59 s, windows 3m52, macos 1m38) |
| 6 — embarquer | 2026-10-26 | `libscriba.h` remplie ; trois hôtes C prouvés (lancer, appeler Double(21)=42, lire total=21) sur windows-x64, windows-x86 et linux-x64 — campagne `--embed` 3/3 ; archive sans objet de démonstration ; `--standalone --modules` : variante `scriba_modules`, 8 SLFST ajoutées, `besDLL_MAIN` gardé hors STATIC_LINK — l'autonome répond « standalone modules : OK » depuis un répertoire sans modules ni conf (10 modules win64, 7 linux) |
| 7 — servir | 2026-10-26 (en cours) | **E6 corrigé et prouvé** : sbhttpd parle HTTP/1.x (statut, Content-Type, Content-Length) dès que le script n'écrit pas sa première ligne ; passthrough préservé pour `Status:` ; `servers.autoheaders 0` restaure l'ancien mode ; cgi-post win64 22/22 sans régression ; l'arrêt « une instruction » documenté (suppression de pid.txt = arrêt gracieux ; kill du pid = arrêt immédiat qui tue toutes les instances avec le processus) ; **E46 corrigé** (message du module rempli sur le chemin bootmodu + code relu dans la page ; cgi-post 22/22 sur windows-x64 ET linux-x64, restriction CI retirée) ; E47/E42 hors périmètre serveur, restent ouverts |

### Phase 6 — Embarquer (v1.1) — réalisée

1. `libscriba.h` remplie : inclut `scriba.h` et les standards que les
   en-têtes supposent ; documente le cheminement (`new` → `Load` →
   `Run` → `Call`/`GetVariable` → `destroy`) et les pièges éprouvés
   (noms de fonctions en minuscules dans la table interne —
   `main::double`, pas `main::Double` ; exécuter une fois avant
   d'appeler ; `GetC`/`_environ` à fournir par l'hôte ; formats
   d'arguments `u i r s b`, un `L` est ignoré).
2. `examples/embed/` : `lancer`, `appeler` (Double(21)=42 depuis le
   C) et `variable` (total=21 lu du BASIC), compilés par
   `--libscriba` contre `libscriba.a`, joués par `--embed` — 3/3 sur
   windows-x64, windows-x86 et linux-x64 (WSL). L'archive n'embarque
   plus l'objet de démonstration (main/GetC/_environ) : l'hôte
   fournit les siens, sans exception de plateforme.
3. `--standalone --modules` : variante `scriba_modules` liée avec les
   modules en `-DSTATIC_LINK=1` + `lmt_standalone.c` généré. Les
   SLFST manquantes ont été ajoutées à 8 interfaces (hash, re, t,
   win, unix, zlib, odbc ; curl en avait déjà une) d'après leurs
   déclarations ; `besDLL_MAIN` est gardé hors `STATIC_LINK` dans
   basext.h (un module statique n'est pas une DLL — trois DllMain
   en doublon sinon). Preuve : l'autonome exécuté depuis un
   répertoire sans `modules/` ni `scriba.conf` répond
   « standalone modules : OK » (windows-x64 : 10 modules ;
   linux-x64 : 7). Le lien statique macOS reste à éprouver.

1. : l'API vit déjà dans `src/scriba.h`
   (`scriba_Call`, `scriba_CallArg`, `scriba_GetVariable`, `scriba_GetString`…,
   71 fonctions) ; décider et documenter statique vs dynamique (`.so`/`.dll`).
2. Exemples consommateurs C dans `examples/embed/` : lancer un script ;
   appeler une fonction BASIC avec arguments ; lire une variable — compilés
   contre `libscriba.a` et exécutés sur windows-x64, windows-x86 et
   linux-x64 (WSL), rejoués par la CI.
3. `--standalone --modules` : bytecode ET modules liés en statique
   (technique `lmt_*.c` de sbhttpd) — l'exécutable autonome ne demande plus
   rien à côté de lui.

### Phase 7 — Servir (v1.1)

1. sbhttpd parle HTTP/1.x : ligne de statut et `Content-Length` émis par le
   serveur (l'ancien mode « le script écrit ses en-têtes » reste disponible,
   par configuration) — ferme E6.
2. `--cgi-post` étendu : en-têtes servis et corps exact vérifiés, connexion
   fermée proprement, curl sans `--http0.9`.

### Phase 8 — Le langage, rapproché du classique (par passes, après v1.0.0)

**Quand** : après le tag v1.0.0. Verdict argumenté : avant, chaque
changement de mot-clé invaliderait le corpus (phase 3) et la doc (phase 4)
pendant qu'on les consolide ; après, chaque passe s'appuie sur la machine
qui existe — contrat écrit d'abord (rouge), implémentation (vert), doc
régénérée, refus ajouté. La refonte se fait **par passes thématiques,
jamais en big-bang** : une famille par passe (types déclarés
`DIM x AS INTEGER`, `PRINT USING`, `DATA/READ/RESTORE`, `INPUT`,
`ELSEIF`…), et les formes historiques refusées (E45) reviennent par la
même porte.

**Politique « ancien »** (à consigner dans `docs/COMPATIBILITE.md` à la
phase 5) : pas de compatibilité bug-pour-bug ; les 178 exemples restent le
corpus pédagogique (les jouables passent, les autres sont annotés «
pourquoi ») ; la suite upstream reste un instrument de mesure des
divergences volontaires, jamais un contrat ; le « look » BASIC (mots-clés
insensibles à la casse, `module`/`declare … lib`, suffixes `$`) est
garanti.

**Identité** : le renommage `basicscript` — non décidé — se fera le jour J
par la macro d'identité centralisée (phase 5) plus une passe de
documentation ; rien d'autre ne devra bouger.
