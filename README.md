# ScriptBasic2-ng — le noyau scriba-zig, prêt pour la fusion

L'interpréteur ScriptBasic, en C, compilé avec le compilateur de **Zig**
(`zig cc`) pour six plateformes. Les sources sont **plates** : chaque `.h` est
un vrai fichier, écrit et maintenu à la main, et aucun outil ne le fabrique.

Ce dépôt est la **fusion** de deux ateliers : le noyau vient de scriba-zig
(import verbatim, Zig + C23 + sources plates), et les apports de
ScriptBasic-master-ng (modules zlib/curl/odbc/dyc, contrats snapshot,
documentation générée depuis `syntax.def`) arrivent par phases, selon la
feuille de route [docs/PLAN.md](docs/PLAN.md).

## Ce que ce dépôt contient

- `src/` — les sources C de l'interpréteur (93 `.c`, 55 `.h`).
- `tests/` — programmes ScriptBasic autonomes, la suite amont (`upstream/`)
  et leur lanceur Python.
- `build.py` — le pilote de compilation (un seul fichier, sans dépendance).
- `pod2md.py` — la fabrique de documentation (lit le C, écrit le markdown).
- `include/` — les fichiers `.bas` d'inclusion du langage.
- `examples/` — les exemples historiques (copie de l'amont, ne pas reformater).
- `webtest/` — les pages d'essai du serveur `sbhttpd`.
- `scriba.conf.lsp` — la configuration, en texte, compilée en `scriba.conf`.
- `LICENSE` — la licence LGPL 2.1 d'origine.
- `docs/` — la mémoire du dépôt ([ERRATA](docs/ERRATA.md),
  [GRAMMAIRE](docs/GRAMMAIRE.md), [ROADMAP](docs/ROADMAP.md),
  [EVOLUTIONS](docs/EVOLUTIONS.md), [MANUEL](docs/MANUEL.md)… et
  [PLAN](docs/PLAN.md), le contrat de la fusion).

Il n'y a **ni Perl, ni TinyCC, ni générateur d'en-têtes, ni fichier `.jam` de
build**. `build.py` est le seul outil de compilation ; `pod2md.py` et
`scripts/gen_grammaire.py` ne font que de la documentation.

## Prérequis

- [Zig](https://ziglang.org/download/) **0.17.0**, version épinglée (aucun
  autre compilateur, aucune bibliothèque à installer : Zig embarque les
  en-têtes et les bibliothèques de toutes les cibles). Installation locale :
  `C:\outils\zig\0.17.0`.
- Python 3.9 ou plus récent.

Zig est cherché dans cet ordre : `--zig CHEMIN`, la variable `ZIG`, puis
`C:\outils\zig\0.17.0\zig.exe`, puis `zig` dans le `PATH`.

## Construire

```sh
python build.py --list                    # les six cibles
python build.py --target windows-x64      # une cible
python build.py --all                     # les six
python build.py --target windows-x64 --modules   # hash, re, t, trial, cgi, mt, cio, win, unix
python build.py --target windows-x64 --modules --release   # sans trial
python build.py --target windows-x64 --httpd     # serveur sbhttpd
python build.py --target windows-x64 --standalone  # executable autonome (hello.bas)
python build.py --target windows-x64 --stndlone    # chemin -C + stndlone
python build.py --target windows-x64 --libscriba   # archive statique + en-tetes
python build.py --target windows-x64 --re-engine   # essai C du moteur Spencer
python build.py --clean                   # efface obj/ et dist/
```

Les produits vont dans `dist/<cible>/` :

| cible           | triplet Zig          | binaire          | état |
|-----------------|----------------------|------------------|------|
| `windows-x86`   | `x86-windows-gnu`    | `scriba.exe`     | compilé et testé ici |
| `windows-x64`   | `x86_64-windows-gnu` | `scriba.exe`     | compilé et testé ici |
| `linux-x64`     | `x86_64-linux-gnu`   | `scriba`         | compilé et testé (WSL) |
| `linux-arm64`   | `aarch64-linux-gnu`  | `scriba`         | compilé (non exécuté ici) |
| `macos-x64`     | `x86_64-macos`       | `scriba`         | compilé (non exécuté ici) |
| `macos-arm64`   | `aarch64-macos`      | `scriba`         | compilé (non exécuté ici) |

« Compilé » veut dire : les 47 unités de la cible et l'édition de liens
réussissent, le binaire est bien au format attendu (PE, ELF, Mach-O). Un
binaire macOS ou ARM ne peut pas être exécuté sur cette machine Windows ; les
tests ne portent donc que sur les cibles qui tournent ici. La CI refait le
travail complet sur les trois familles d'OS à chaque poussée.

`--modules` produit les neuf modules sans dépendance tierce, en
bibliothèques partagées, dans `dist/<cible>/modules/` :

| module | ce qu'il apporte | fichiers produits |
|---|---|---|
| `hash` | tables de hachage (`hash::Value` rend une copie) | `hash.dll` / `.so` / `.dylib` |
| `re` | expressions régulières, moteur de Henry Spencer | `re.*` (+ le moteur compilé dedans) |
| `t` | md5, sérialisation de tableaux, fichiers | `t.*` |
| `trial` | module d'essai de l'ABI (compteur, `iff`, `pprint`) ; `test_only`, exclu par `--release` | `trial.*` |
| `cgi` | environnement CGI, paramètres GET/POST, gabarits | `cgi.*` |
| `mt` | sessions et verrous pour le serveur multi-thread | `mt.*` |
| `cio` | console Windows (curseur, touches, titre) | `cio.dll` (Windows seulement) |
| `win` | registre, services, processus (ex-`nt`) | `win.dll` (Windows seulement) |
| `unix` | `fork`, `chmod` (ex-`ux`) | `unix.*` (Linux/macOS) |

Les déclarations `hash.bas`, `re.bas`, `t.bas`, `cgi.bas`, `mt.bas`,
`cio.bas`, `win.bas` et `unix.bas` sont copiées dans `modules/include/`, ce
qui rend `import re.bas` opérationnel. `trial.bas` n'est pas copié : celui de
la distribution porte des chemins absolus `D:\MyProjects\…`. Détail des
règles (et du piège d'interposition des symboles) :
[docs/MODULES.md § 6](docs/MODULES.md#6-construire-les-modules-aujourdhui).

`--standalone` construit un exécutable autonome à partir de `tests/hello.bas`
par défaut, ou du script donné : `scriba -E` recopie l'interpréteur et ajoute
le bytecode. Le produit est `dist/<cible>/<script>_standalone[.exe]` ; il ne
demande ni compilateur ni fichier `.bas` à l'exécution. Il ne contient pas les
modules : un script qui importe `hash`, `re`… doit être lancé depuis
`dist/<cible>` pour retrouver `modules/` et `scriba.conf`. Windows x86/x64 et
Linux x64 sont vérifiés ; macOS reste à exécuter. La sortie de `hello.bas`
est contrôlée par `python tests/run.py --target <cible> --standalone`.

`--stndlone` construit le **second** chemin : `scriba -C` écrit le bytecode
et un `main()` qui appelle `stndlone()`, puis le pilote compile ce C et le lie
à `src/stndlone.c` et au coeur. Le produit est
`dist/<cible>/<script>_stndlone[.exe]` ; `stndlone` n'est pas un binaire
autonome à lui seul. `python tests/run.py --target <cible> --stndlone`
vérifie la sortie de `tests/hello.bas`.

`--libscriba` produit `dist/<cible>/libscriba.a` (47 objets : coeur,
commandes, `lmt_none.c`, `libscriba.c`) et copie les en-têtes dans
`dist/<cible>/include/`. C'est une archive, pas un exécutable ; sous UNIX,
l'hôte qui la lie doit fournir `char **_environ;` pour que l'archive ne tire
pas `libscriba.o` et son `main()`.

`--re-engine` produit `dist/<cible>/re_engine[.exe]`, l'essai C direct du
moteur d'expressions régulières : il atteint `REG_BASIC`, `REG_PEND`, les
références arrière et la table `cnames`, que l'interface du module `re` ne
peut pas exposer. Le programme sort 0 si ses 17 essais passent ; il est
exécuté par `python tests/run.py --target <cible> --re-engine`.

## Tester

```sh
python tests/run.py --target windows-x64
python tests/run.py --target windows-x64 --modules   # + hash, re, t, trial, cgi, mt, cio, win
python tests/run.py --target windows-x64 --upstream  # suite d'origine, sous-ensemble declare
python tests/run.py --target windows-x64 --upstream-all  # + essais longs et diagnostics
python tests/run.py --target windows-x64 --standalone  # hello_standalone, sans .bas
python tests/run.py --target windows-x64 --stndlone   # hello_stndlone, sans .bas
python tests/run.py --target windows-x64 --re-engine  # moteur re, essai C direct
python tests/run.py --target windows-x64 --cgi-post  # POST CGI urlencoded + multipart ASCII/binaire/depassement, scriba -c et sbhttpd
python tests/run.py --target linux-x64     # lance sous WSL si on est sous Windows
```

Le jeu par défaut exécute treize programmes : `hello`, `vars`, `loops`,
`arrays`, `dim`, `functions`, `operators`, `commandes`, `mathfunc`, `trig`,
`nombres`, `slif` et `exit` — les tests qui ne demandent aucun module, donc
les seuls qui doivent passer sur les six cibles. `dim.bas` vérifie les
déclarations (simple, imbriquée, en liste), `slif.bas` le `if` d'une ligne
avec ses `ELSEIF`/`ELSE` (E37), `exit.bas` les sorties de boucle et le
`do … loop` nu (E39). `operators.bas` couvre les opérateurs qui restent
(`<<`, `>>`, `??`, `**`, `//`, affectations composées) ; `commandes.bas`
vérifie les erreurs des commandes de fichiers, de répertoires, de dates et
des nombres (ERRATA E26 à E31) ; `mathfunc.bas` vérifie `max`, `min`,
`imax`, `imin`, `bin`, `gcd` et `lcm` ; `trig.bas` les quinze fonctions de
l'audit, les six réciproques hyperboliques (`ahsin`…`ahctan`) et
`rad2deg`/`deg2rad` (E32) ; `nombres.bas` les bornes des entiers, les
littéraux au-delà de la plage et le produit qui déborde (E33).

Le jeu comprend aussi **dix-sept refus attendus**, que le lanceur exige au
lieu de les exécuter : `refus_directory.bas` (`error &H77`, `open … for
directory`), `refus_pause.bas` (`&H77`), `refus_fork.bas` (`&H68`),
`refus_sqr.bas` (`&H68`), `refus_operateur.bas` (`&H77`, un opérateur mort
comme `==`), `refus_cv.bas` et `refus_mk.bas` (`&H68`, les familles `cvd`…
et `mkd`…), `refus_slif_for.bas` et `refus_slif_mix.bas` (`&H77`, ce qui ne
va pas dans un `if` d'une ligne), `refus_exit.bas`, `refus_exit_for.bas`,
`refus_exit_while.bas` et `refus_exit_sub.bas` (`&H74`, un `exit` hors de la
construction qui le porte), `refus_isdefined.bas` (`&H71`) et les trois
refus de `dim` (`refus_dim.bas`, `refus_dim_imbrique.bas`,
`refus_dim_liste.bas`, `&H77`). `sqr` n'existe pas : la fonction s'appelle
`sqrt` ; `pause`, `fork`, les 256 opérateurs morts et les familles
`cv*`/`mk*` sont sortis de la grammaire (E34).

`--modules` ajoute `hash`, `re`, `md5` (le test du module `t`), `trial`,
`cgi` et `mt` partout, puis `cio` et `win` sous Windows, `unix` sous Linux et
macOS ; les tests partent de `dist/<cible>` pour que la clé `module` de la
configuration trouve `modules/`.

`--upstream` lit [tests/upstream/MANIFEST.tsv](tests/upstream/MANIFEST.tsv) :
avec les modules construits, il lance 12 tests du coeur et des modules, et
saute les 20 autres avec leur raison. `--upstream-all` ajoute `test_sleep`,
`roundbugin1-0b27` et `seggfault` (15 lancés, 17 sautés). Aucun `.bas` de
`tests/upstream/` n'est modifié.

`--cgi-post` exige `sbhttpd` (`--httpd`) et le module `cgi` (`--modules`) :
il sert `tests/cgi_post.bas`, `tests/cgi_upload.bas` et les cinq scripts
d'erreur `tests/err_runtime.bas`, `tests/err_load.bas`,
`tests/err_module.bas`, `tests/err_pending.bas` et `tests/err_partial.bas`,
d'abord par `scriba -c` avec un corps POST sur stdin,
ensuite par `sbhttpd` avec une configuration temporaire sur un port libre.
Onze cas passent par les deux chemins : urlencoded ; multipart avec un petit fichier ASCII ; multipart
avec 65536 octets binaires, zéros compris ; multipart avec un fichier de
10 Mo + 1 octet, que le module refuse ; le même refus avec 256 Ko de plus,
qui vérifie que le module consomme le reste du corps et que le serveur
ferme proprement ; un corps annoncé au-delà de `CONTENT_MAX`, refusé sur le
seul en-tête `Content-Length` ; cinq erreurs, qui couvrent les chemins du
rapporteur. Les trois premières n'ont rien écrit et exigent un statut `500`
avec la page d'erreur complète. `err_pending.bas` n'a pas encore envoyé sa
première ligne et y a droit aussi ; `err_partial.bas` a déjà écrit son statut
et `debut`, donc le serveur n'ajoute que le diagnostic, sans ligne de statut
ni page ([ERRATA E25](docs/ERRATA.md#e25)). En mode `scriba -c`, le
rapporteur du CLI applique la même règle, mais il écrit chaque octet aussitôt :
`err_pending.bas` y a donc déjà envoyé `debut` et n'a pas droit à la page,
tandis que `err_runtime.bas` et `err_module.bas` la reçoivent. Le premier vérifie
`REQUEST_METHOD`, `CONTENT_LENGTH`, `QUERY_STRING`, `PostParam` et l'en-tête
`Status: 200` ; les deux suivants vérifient `ContentType`, `FileName`,
`FileLength`, `SaveFile` et le contenu sauve sur disque. Les deux cas
d'au-delà de 10 Mo exigent `CGI_ERROR_FILEMAX` (`0x00080008`), celui de
`CONTENT_MAX` exige `CGI_ERROR_BIG_CONTENT` (`0x00080002`), tous trois
l'absence de toute sortie du script et, par le serveur, un statut
`413 Request Entity Too Large` : le refus tombe pendant le démarrage du
module, donc avant la première ligne BASIC et avant que la moindre réponse ne
soit écrite ([ERRATA E23](docs/ERRATA.md#e23), [E24](docs/ERRATA.md#e24)).
Les deux cas multipart sont plus lents : le corps fait 10 Mo, et
`--cgi-post-timeout` les borne.

Ces trois derniers cas sont les seuls à ne pas passer par les refus du module.
Le serveur répond `500 Internal Error` dans les trois, puisque le script n'a
rien écrit avant d'échouer :

- `tests/err_runtime.bas` échoue à l'exécution avec le code 22 (`0x16`), dont
  le bit `0x2` est levé ; c'est lui qui fige [E24](docs/ERRATA.md#e24) :
  avant la correction, `sbhttpd` répondait sans ligne de statut et le client
  levait `BadStatusLine` ;
- `tests/err_load.bas` déclare un `include` introuvable : l'erreur tombe au
  chargement et c'est le lecteur qui la rapporte, avec ses propres drapeaux.
  En mode `scriba -c`, le CLI écrit alors une page CGI sur stderr et sort
  avec 0, comme sur toute erreur de syntaxe ;
- `tests/err_module.bas` refuse `cgi::SaveFile` sur un nom qui n'est pas un
  fichier téléversé : code `0x00080000`, au-delà de `MAX_ERROR_CODE` (133),
  donc écrit en `0x%08x` et complété par le message du module.

ScriptBasic rend **toujours 0**, même sur une erreur de syntaxe : le lanceur
considère donc comme échec un code non nul, une ligne `ECHEC` ou un message
`error &H…`. La seule exception est le cas `erreur-chargement`, où la page
CGI d'erreur est le résultat attendu (`cgi_page` dans la table des cas). Voir
[ERRATA E3](docs/ERRATA.md#e3).

Les programmes de `tests/` qui restent hors du jeu (`dyc*.bas`,
`manuel.bas`, `mt_leak.bas`, `odbc.bas`, `win_hardlink.bas`…) exigent `dyc`
(ABI 32 bits), les threads, une base ODBC ou les modules à bibliothèque
tierce — plusieurs d'entre eux deviennent jouables aux phases 2 et 3 de
[docs/PLAN.md](docs/PLAN.md).

## Comment les en-têtes ont été aplatis

Le ScriptBasic d'origine déclarait les prototypes dans un commentaire du `.c`
(`TO_HEADER:`) et un outil Perl en recopiait le contenu dans le `.h`. Ici :

1. chaque `.h` existant est devenu un fichier source normal ;
2. le bloc `TO_HEADER:` a été retiré des `.c` (le `.h` en est la copie) ;
3. plus aucun outil ne lit ni ne réécrit ces fichiers.

Conséquence : pour changer un prototype, on modifie le `.h` **et** le `.c`
s'ils doivent rester d'accord — comme dans n'importe quel projet C. Le
compilateur signale un désaccord, il ne le répare pas.

Les commentaires de documentation d'origine (`/*POD`, `/*FUNCTION*/`,
`=H ...`) restent dans les sources, et `pod2md.py` les interprète : c'est le
seul outil qui les lit. `python pod2md.py` engendre [docs/MANUEL.md](docs/MANUEL.md)
(212 entrées, classées par section) et une page par module documenté dans
[docs/modules/](docs/modules/README.md) ; `python pod2md.py --check` échoue si
un fichier a divergé des sources.

## Documentation

Tout ce qu'il faut pour reprendre le dépôt est dans [`docs/`](docs/README.md) :

| document | contenu |
|---|---|
| [docs/README.md](docs/README.md) | sommaire et questions fréquentes (`.jam`, `.bas` manquants, plateformes) |
| [docs/PLAN.md](docs/PLAN.md) | la feuille de route de la fusion, phase par phase |
| [docs/REPRISE.md](docs/REPRISE.md) | état du dépôt, commandes, carte du code |
| [docs/PROGRAMMES.md](docs/PROGRAMMES.md) | les exécutables : `scriba`, `sbhttpd`, formes embarquées, CGI |
| [docs/MODULES.md](docs/MODULES.md) | les 20 modules et le remplacement des `.jam` |
| [docs/MANUEL.md](docs/MANUEL.md) | le manuel du langage, engendré depuis le code |
| [docs/ERRATA.md](docs/ERRATA.md) | défauts connus et pièges (`E1`…`E39`) |
| [docs/ROADMAP.md](docs/ROADMAP.md) | ce qui reste, par priorité |
| [docs/EVOLUTIONS.md](docs/EVOLUTIONS.md) | décisions et historique |
| [docs/INVENTAIRE.md](docs/INVENTAIRE.md) | contenu du dépôt |
| [AGENTS.md](AGENTS.md) | consignes pour un agent |

## Licence

ScriptBasic est distribué sous **GNU LGPL 2.1** (voir `LICENSE`). Ce portage
conserve cette licence. Le projet d'origine est de Peter Verhas.
