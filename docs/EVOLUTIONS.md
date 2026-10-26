# ÉVOLUTIONS — journal des décisions

Ce fichier raconte **ce qui a changé et pourquoi**. Il ne décrit pas l'état
courant (c'est [REPRISE.md](REPRISE.md) et [INVENTAIRE.md](INVENTAIRE.md)),
il explique les choix pour qu'on ne les refasse pas dans l'autre sens.

Les dates sont celles du dépôt ; le projet d'origine, lui, date de 1999-2005.

---

## 2026-10-25 — vingt-quatrième passe : `DIM`, un tableau de taille connue

L'audit de `build_tcc/EVOLUTIONS.md` §2.2 listait encore `dim` : « créer un
tableau de taille connue et rendre la zone `undef` est aujourd'hui
impossible ». C'est exact — les tableaux naissent au premier accès, et le seul
moyen de connaître `ubound(a)` était d'écrire dans la case la plus haute. La
commande est maintenant dans la grammaire et dans le corps.

### 1. La forme et le mot

`DIM a[10]` déclare les indices **0 à 10** (onze cases), toutes `undef` ;
`lbound` vaut 0 et `ubound` 10 dès avant la première écriture. Les crochets
sont ceux du langage (`a[10]`, jamais `a(10)` : les parenthèses sont l'appel
de fonction). Chaque crochet supplémentaire ajoute une dimension, et chaque
dimension est allouée **dans chaque élément** de la précédente : `DIM m[2,3]`
construit ce que l'accès `m[i,j]` attend, sinon `ubound(m[1])` resterait
`undef` avant la première écriture.

Le mot est un nouveau mot-clé (code 272), la commande un nouvel opcode
(`CMD_DIM` 893). Les tables parallèles de `syntax.c` sont indexées par
`opcode - START_CMD` : `END_CMD`, `NUM_CMD` et `END_EXEC` montent donc d'un
cran (893/544/894), et `COMMANDSYMBOLS`, `CommandFunction`, `ASYMBOLS` et
`COMMANDS` reçoivent leur entrée. `--grammaire` le vérifie : 544 emplacements,
aucun nom sans corps.

### 2. Ce que `DIM` ne fait pas

Il ne fixe pas une limite. Écrire `a[20]` après `DIM a[10]` étend toujours le
tableau, comme partout dans le langage : `DIM` fixe l'étendue **initiale**.
Il ne conserve rien non plus : re-déclarer remplace l'ancien tableau, valeurs
comprises. Enfin la taille est une expression, évaluée une seule fois, au
moment où la commande s'exécute — `DIM a[n]` ne suit pas `n` ensuite.

Une taille indéfinie ou non numérique rend `&H7`, une taille négative ou
hors plage rend `&H8`, et la variable garde alors sa valeur : le nouveau
tableau est construit avant que l'ancien ne soit libéré.

### 3. Les refus, à l'analyse

`DIM a` (sans crochets), `DIM a[1][2]` (une case comme cible) et
`DIM a[1], 2` (un terme de liste sans crochets) sont refusés pendant
l'analyse, avec le code fatal `&H77`, plutôt que d'être acceptés puis rejetés
à l'exécution. C'est la règle que la passe précédente avait fixée.

Deux détails ont coûté une itération chacun :

- `EX_LEX_LVALL` range **toute** la liste des valeurs à gauche dans un seul
  argument de commande (`Argument.pNodeList`), et non en chaîne d'arguments.
  La première version parcourait donc cette liste comme si c'était un nœud :
  elle refusait `dim a[2]` lui-même. Le contrôle parcourt maintenant
  `pNodeList->rest` pour les valeurs et `Arguments.Argument->rest` pour les
  indices ;
- le contrôle valide **après** l'analyse générique, qui a déjà consommé la
  ligne : le rapporteur désignait la ligne suivante, ou `(0)` en fin de
  fichier. La position du premier mot est sauvegardée puis restaurée avant le
  refus, pour que le message nomme la bonne ligne.

### 4. Couverture

`tests/dim.bas`, 41 vérifications : bornes et `undef`, deux dimensions,
taille calculée et évaluée une fois, plusieurs tableaux sur une ligne,
remplacement, extension au-delà de la borne, portée locale, argument par
référence, tailles `0`, réelle tronquée et chaîne entière, puis les quatre
refus d'exécution avec la valeur ancienne conservée. Trois fichiers fixent les
refus d'analyse : `refus_dim.bas`, `refus_dim_imbrique.bas`,
`refus_dim_liste.bas`.

Vérifié : 30/30 sous Windows x64, Windows x86 et Linux x64 (dont 17 refus) ;
21/21 avec `--modules` ; `--grammaire` vert ; `pod2md.py --check` sans écart.
La commande est documentée dans `docs/MANUEL.md`, section `array`.

---

## 2026-10-25 — vingt-troisième passe : `isdefined(f(...))`, le plantage qui n'existe plus

L'audit plaçait en tête de ses défauts reproduits
`print isdefined(crypt("a","b"))`, avec cette cause : « `ISDEF` évalue son
argument sans que l'appel de fonction imbriqué soit protégé ». Re-mesuré sur
cet arbre, le programme rend `-1` et le processus survit — sous Windows x64 et
x86, sous Linux x64, et dans les deux variations `--standalone` et
`--stndlone`. Vingt mille appels dans une boucle ne changent rien.

### 1. Le défaut vit dans l'ancien portage, et pas dans `ISDEF`

Les binaires du portage TCC (`build_tcc/bin64/scriba64.exe`,
`build_tcc/bin/scriba.exe`) tombent toujours sur ce programme. La mesure les
sépare : `print crypt("a","b")` **seul** les tue aussi, alors que `ucase`,
`len` et `hex` passent. Le défaut est donc dans `crypt` du portage TCC, pas
dans le test de définition : `ISDEF` n'était que le premier endroit où la
valeur mourante était lue. L'attribution de l'audit était fausse ; elle n'est
pas reportée dans cet arbre, dont `crypt` fonctionne.

### 2. La protection, rendue locale

Le contrat est désormais écrit et vérifié dans `ISDEF` et son miroir
`ISUNDEF` :

- `CAR(nItem) == 0` — aucune expression à inspecter — répond faux (vrai pour
  `ISUNDEF`) au lieu de faire lire `CommandArray[-1]` ;
- le résultat passe par `memory_Type()`, qui rend `VTYPE_UNDEF` même pour
  `NULL` : un appel de fonction qui ne rend aucune valeur répond faux sans
  être déréférencé ;
- la documentation POD dit que l'argument n'est évalué qu'une fois et qu'un
  appel rendant `undef` vaut faux.

Cette garde est défensive : elle ne corrige pas un plantage observable dans
cet arbre, elle empêche qu'une régression du même genre repasse par `ISDEF`.

### 3. Couverture

La section 6 de `tests/commandes.bas` passe de deux à quatorze contrôles :
`crypt` et `len` (les deux cas d'origine), puis une fonction utilisateur qui
rend une valeur, une qui n'en rend pas, un élément de tableau local, une
chaîne de `crypt` transmise par une fonction, un argument `undef`, une
fonction récursive, un sous-programme, `isdefined(isdefined(f()))`, les deux
miroirs `isundef`, et deux cents appels dans une boucle. Un contrôle compte
les appels : l'argument n'est évalué qu'une fois.

L'autre moitié de la garde est fixée par `tests/refus_isdefined.bas` :
`print isdefined` est refusé à l'analyse (`&H71`), donc la branche `CAR(0)`
reste inatteignable en pratique.

Vérifié : 26/26 sous Windows x64, Windows x86 et Linux x64 ; `--grammaire`
vert ; `pod2md.py --check` sans écart.

---

## 2026-10-25 — vingt-deuxième passe : EXIT quitte les boucles

L'audit classait `exit for` parmi les dix ajouts à plus forte valeur, avec la
règle « un refus à l'analyse vaut mieux qu'un refus silencieux à
l'exécution ». Cette passe ajoute `EXIT`, `EXIT FOR`, `EXIT WHILE`, `EXIT DO`
et `EXIT REPEAT`.

### 1. Le saut réutilise les labels des boucles

Aucun mécanisme nouveau à l'exécution : chaque boucle pousse déjà deux labels
sur la pile de compilation, celui de sortie (`GO_FORWARD`) puis celui de
début (`COME_BACK`). `EXIT` prend le premier des deux et s'en sert comme
`GOTO`.

- `EX_LEX_GO_LOOP_EXIT` ne pousse ni ne retire aucun label : il lit la pile.
  C'est ce qui permet de le placer en fin de règle sans les précautions des
  `GO_FORWARD`/`COME_BACK` ;
- le type demandé est porté par la règle (`CMD_FOR`, `CMD_WHILE`, `CMD_DO`,
  `CMD_REPEAT`), zéro pour la forme nue qui prend la boucle la plus interne
  quelle qu'elle soit ;
- la recherche s'arrête au premier label de `SUB` ou `FUNCTION` : une boucle
de l'appelant n'est pas visible depuis le corps d'une subroutine ;
- sans boucle de l'espèce demandée, l'analyse rend `&H74` (« erroneous
  nesting »), comme un `WEND` sans `WHILE`.

La cible du saut est le `CDR` du label, pas le label : l'analyseur donne au
label de sortie le numéro de la ligne du mot-clé de fermeture (`NEXT`, `WEND`,
`LOOP`, `UNTIL`), et la première ligne après la boucle est celle qui la suit,
comme pour le saut d'un `WHILE` dont la condition est fausse.

### 2. Un défaut trouvé en chemin : `DO … LOOP` nu

La tête `DO` pousse des labels de type `CMD_LOOP`, mais la ligne `LOOP` nue
les attendait en `CMD_DO`. `_ex_PopLabel()` ne compare que le sommet de la
pile : tout `do … loop` sans condition après `LOOP` était donc refusé à
l'analyse (`&H74`), et `exit do` ne trouvait aucun label. Le défaut n'avait
pas été vu parce qu'aucun test n'utilise cette forme : `tests/loops.bas` ne
ferme qu'avec `loop while`. La règle `CMD_LOOP` attend désormais `CMD_LOOP`,
comme `LOOP WHILE` et `LOOP UNTIL` le faisaient déjà, et `EXIT DO` ramène
`CMD_DO` et `CMD_LOOP` à une seule espèce. Détail dans [ERRATA E39](ERRATA.md).

### 3. Couverture

`tests/exit.bas` vérifie les cinq formes, et surtout ce que le saut ne doit
pas faire :

- `exit for` depuis un `for` imbriqué ne quitte que l'interne, et la commande
  placée juste après la boucle est bien la première exécutée ;
- `exit while`, `exit do` et `exit repeat` rendent la valeur du compteur au
  moment du saut ;
- `exit` nu depuis un `for` contenant un `do` ne quitte que le `do` ;
- `exit do` traverse un `for`, `exit for` traverse un `while` : c'est bien
  l'espèce demandée qui est quittée ;
- dans une subroutine et dans une fonction, `exit for` ne voit que leur propre
  boucle et l'appelant continue ;
- un `exit for` sur la dernière construction du programme termine proprement
  (le label de sortie n'a pas de ligne suivante).

Quatre refus complètent le fichier : `refus_exit.bas` (aucune boucle),
`refus_exit_for.bas` (`for` demandé sous un `while`), `refus_exit_while.bas`
(`while` demandé sous un `for`) et `refus_exit_sub.bas` (boucle de l'appelant
invisible depuis une subroutine). Le jeu par défaut passe de 20 à 25 tests.

Vérifié : 25/25 sous Windows x64 ; `--grammaire` vert (475 noms, 543
emplacements, 296 codes `CMD_`, 0 `NOTIMP` armé) ; `pod2md.py --check` sans
écart ; construction Linux x64 OK.

---

## 2026-10-25 — vingt et unième passe : les formes mortes, verrouillées

L'audit §2.2 de `build_tcc` proposait de retirer de la grammaire les 256
opérateurs, `pause`, la famille `cv*`/`mk*` et `fork`, « pour qu'un refus à
l'analyse remplace un refus silencieux à l'exécution ». Cette passe ne
l'applique pas : elle était déjà appliquée, par la quatrième passe pour les
opérateurs et la dix-neuvième pour `pause`, `fork`, `sqr` et la
trigonométrie. Ce qui manquait, c'est la preuve — et deux des quatre familles
n'avaient aucun test.

### 1. Mesuré, pas raconté

Dix-huit formes essayées sur `dist/windows-x64/scriba.exe` :

| forme | verdict |
|---|---|
| `==`, `!=`, `+^`, `&&`, `@`, `!`, `?` | `error &H77` à l'analyse |
| `pause 1` | `error &H77` à l'analyse |
| `fork()`, `cvd()`, `cvi()`, `cvl()`, `cvs()` | `error &H68` (fonction inconnue) |
| `mkd()`, `mki()`, `mkl()`, `mks()` | `error &H68` |

Aucune ne s'exécute, aucune ne rend l'erreur 28. `$` n'est pas un opérateur
retiré : c'est un caractère de nom de variable, et `print $1` lit une variable
non définie — comportement inchangé.

### 2. L'invariant, vérifié statiquement

`extops.c` n'existe plus, et les 290 `#define NOTIMP_*` valent tous 0 : aucun
bloc `COMMAND` n'est remplacé par `NOTIMPLEMENTED`. Restait le cas d'un nom
dont l'emplacement dans `CommandFunction[]` est `NULL`. Un relevé des tables
(475 noms, 542 emplacements, 295 codes `CMD_`) en trouve onze : `CONST`,
`VAR`, `GLOBAL`, `MODULE`, `ENDMODULE`, `LOCAL`, `LABEL`, `NLABEL`, `EMPTY`,
`PRAGMA`, `GCONST`. Ce sont des mots du préprocesseur, et la mesure le
confirme : `const`, `var`, `global` et `module` fonctionnent ; `local`,
`label`, `nlabel`, `empty`, `pragma` et `gconst` sont refusés à l'analyse
(`&H77`), jamais à l'exécution. L'invariant tient donc : **un nom de la
grammaire est soit exécutable, soit refusé à l'analyse**.

### 3. Ce qui manquait : la couverture

`pause`, `fork` et `sqr` avaient un fichier de refus depuis la dix-neuvième
passe ; les opérateurs et la famille `cv*`/`mk*` n'en avaient pas.
`tests/operators.bas` le disait lui-même : « le refus des autres se vérifie à
la main ». Trois fichiers comblent le trou, et le lanceur les compte comme
les autres refus :

- `tests/refus_operateur.bas` — `print 1 == 1`, attendu `&H77` ;
- `tests/refus_cv.bas` — `print cvd("ab")`, attendu `&H68` ;
- `tests/refus_mk.bas` — `print mki(1)`, attendu `&H68`.

Le jeu par défaut passe de six à neuf refus, donc de 17 à 20 tests.
`--modules` ne bouge pas (19/19 sous Windows, 18/18 sous Linux) : le lanceur
écarte les refus des campagnes de modules pour que leurs comptes ne dépendent
que des modules construits.

### 4. Le relevé devient un contrôle permanent

Le relevé statique ci-dessus était un script jetable : rien ne le rejouait, et
la réponse à « reste-t-il un nom sans corps ? » redevenait une enquête à
chaque fois. `tests/run.py --grammaire` le remplace. Le contrôle ne demande
aucun binaire : il lit `src/syntax.c` et `src/syntax.h` et nomme chaque
problème.

Trois défauts le font échouer :

- un nom dont l'emplacement dans `CommandFunction[]` vaut `NULL`, c'est-à-dire
  accepté à l'analyse et sans rien à appeler à l'exécution — la classe de
  l'erreur 28 ;
- un nom qui renvoie à un `CMD_` sans `#define` ;
- un `NOTIMP_*` armé, qui remplacerait un corps par `NOTIMPLEMENTED`.

Les onze mots du préprocesseur sans corps sont une liste explicite en tête de
`tests/run.py`, avec leur raison. C'est volontaire : un nom nouveau sans corps
fait échouer le contrôle au lieu de se fondre dans le décor, et la seule
façon de le faire taire est d'écrire pourquoi il n'a pas de corps. Les
commentaires sont retirés avant la recherche des `NOTIMP_*`, sinon l'exemple
cité par `src/mynotimp.c` passerait pour un corps armé.

Mesure : 475 noms, 26 codes d'arité, 165 fonctions internes, 295 codes `CMD_`,
542 emplacements, 0 `NOTIMP` armé. Trois contrôles négatifs réels : `ZZZ`
ajouté avec `CMD_PAUSE` puis `CMD_CONST` donne « code sans #define » puis
« NULL au lieu de COMMAND_CONST » ; `NOTIMP_OPEN` passé à 1 dans `notimp.h`
donne « `src/notimp.h` : NOTIMP_OPEN = 1 » et fait sortir en 1. Sources
restaurées, contrôle vert.

Vérifié : 20/20 sous Windows x64, Windows x86 et Linux x64 (WSL) ; `--modules`
19/19 sous Windows x64 ; `--grammaire` vert ; `pod2md.py --check` sans écart.

---

## 2026-10-24 — vingtième passe : le IF d'une ligne, avec ELSE et ELSEIF

[E37](ERRATA.md#e37) laissait la forme d'une ligne sans `ELSE` et sans
`ELSEIF`. Elle est maintenant une vraie construction, avec les mêmes labels de
saut que la forme multi-ligne.

### 1. Ce que l'audit disait, et ce qu'il fallait corriger

L'entrée E37 affirmait que l'amont accepte `if … then … else …` sur une ligne.
C'est faux, et la vérification est dans `expression.c` de l'amont :
`ex_IsCommandIF()` refuse explicitement `ELSE`, `ELSEIF` et `ENDIF` après le
`THEN` d'une forme d'une ligne, et sa documentation dit « There is no way to
specify any ELSE part for the command in the single line version ». La forme
étendue est donc une décision du dépôt, pas un retour à l'amont ; E37 le dit
désormais.

La cause du refus, elle, était double. `ELSE` et `ELSEIF` n'avaient qu'une
règle exigeant la fin de la ligne juste après le mot-clé, et `SLIF` n'était pas
une construction : elle sautait « la commande suivante » sans aucun label, donc
un `ELSE` n'avait rien à quoi s'accrocher.

### 2. Comment la forme d'une ligne est construite

- `ex_IsCommandIF()` émet `CMD_SLIF` avec `[condition, label]` et pousse le
  label de la branche sur la pile de compilation, comme le fait `IF` ;
- le corps de la branche est une commande ordinaire, mais le lecteur de règles
  accepte qu'elle se termine sur `ELSE`/`ELSEIF` au lieu du retour à la ligne
  (`fSingleLineIFBody`) ; `LET`, `OPEN`, `CALL` et la forme générale
  connaissent ce cas, et le mot-clé n'est pas consommé ;
- `ELSE` et `ELSEIF` sont analysés par `ex_IsCommandELSE()` et
  `ex_IsCommandELSIF()`, qui reconnaissent la forme seule sur sa ligne (bloc)
  et la forme suivie d'une commande (une ligne) ; dans le second cas ils
  émettent les opcodes internes `CMD_ELSE_SL` (890) et `CMD_ELSIF_SL` (891) ;
- `ex_Command_l()` ferme la construction à la fin de la ligne physique : le
  label resté sur la pile passe dans la liste d'attente et reçoit le nœud de la
  commande suivante ;
- `COMMAND(SLIF)` reprend la boucle de saut de `COMMAND(IF)`, mais ne reconnaît
  comme marqueurs que `ELSE_SL`/`ELSIF_SL`. Un `ELSE` de bloc englobant reste
  pour elle une simple cible de saut : c'est ce qui permet à un `if` d'une
  ligne placé en fin de corps de ne pas sauter dans le `else` du bloc.

Les opcodes 890 et 891 sont neufs ; `END_CMD` et `NUM_CMD` passent à 891 et
542, et les deux tableaux de `syntax.c` (`CommandFunction`, `COMMANDSYMBOLS`)
sont étendus en conséquence. `END_EXEC`, qui bornait la copie de la table de
commandes à l'opcode 872, passe à 892 : sans cela, une extension qui appelle
`besCOPYCOMMANDTABLE` aurait lu hors de sa copie pour ces deux opcodes, comme
elle le faisait déjà pour les dix-sept derniers. Les deux règles de syntaxe qui
décrivent les nouveaux opcodes ne servent qu'au constructeur : l'analyse passe
toujours par les règles de `ELSE` et `ELSEIF`, qui les précèdent.

### 3. La règle, et ses limites

Une commande exactement par branche, et la construction entière sur une seule
ligne physique. `IF`, `FOR`, `WHILE`, `REPEAT`, `SUB`, `FUNCTION` et les
déclarations sont refusés à l'analyse dans une branche : elle sortirait de la
ligne. Les deux formes ne se mélangent pas dans une même construction ; un
`elseif` d'une ligne dans un `if` multi-ligne est refusé (`&H74`), et
inversement. `elif`, `elsif` et `else if` restent absents, comme décidé à la
passe des commandes mortes.

### 4. Un piège de construction corrigé au passage

Le premier binaire de cette passe plantait sur `print "hi"`. La cause n'était
pas la grammaire : `build.py` ne recompilait un objet que si son `.c` avait
changé, et les en-têtes modifiés (`expression.h`, `syntax.h`) laissaient des
objets anciens liés à des objets neufs, avec des structures de tailles
différentes. `newest_header_mtime()` fait désormais recompiler tout objet plus
ancien que le plus récent des `src/**/*.h`, dans les trois constructeurs
([E38](ERRATA.md#e38)).

### 5. Vérifié

```text
windows-x64  par défaut   17/17   (11 scripts + 6 refus)
windows-x86  par défaut   17/17
linux-x64    par défaut   17/17   (via WSL)
windows-x64  --modules    19/19
linux-x64    --modules    18/18
windows-x64  --test manuel   156/156
windows-x64  --upstream      12 lancés, 0 échec, 20 sautés
python pod2md.py --check     16 fichiers à jour
```

Nouveaux fichiers : `tests/slif.bas` (30 vérifications) et les deux refus
`refus_slif_for.bas` / `refus_slif_mix.bas`.

Contrôle négatif réel, armé puis désarmé, source restaurée et reconstruite
verte : la branche `case CMD_ELSE_SL` de `COMMAND(SLIF)` remplacée par un
`break` — `if 0 then … else …` n'exécute plus la branche `else` et affiche
`vide` au lieu de `sinon`, et `tests/slif.bas` compte 7 échecs avant de
bloquer sur la boucle `while` du test ; le `timeout` de 10 s coupe là.

---

## 2026-10-24 — dix-neuvième passe : toute la trigonométrie, plus aucune commande morte

La passe précédente laissait dix-sept blocs `COMMAND` dont les deux branches
valaient `NOTIMPLEMENTED` : quinze fonctions trigonométriques, `fork` et
`pause`. Elles sont toutes traitées, et le compte est désormais **zéro** :
plus aucune commande acceptée par la grammaire ne peut être refusée à
l'exécution par l'erreur 28.

### 1. Grammaire : deux retraits, un renommage

`pause` et `fork` quittent la grammaire, comme l'audit le proposait, plutôt
que d'être construits : `pause` n'a pas de sens portable dans un interpréteur
qui n'a pas de threads à endormir, et le module `unix` fournit déjà `fork()`.
Un programme qui les utilise est refusé à l'analyse (`error &H77` pour
`pause`, `&H68` « fonction inconnue » pour `fork`).

`sqr` devient `sqrt`, dans la table des symboles comme dans le code
(`CMD_SQRT`, `COMMAND_SQRT`). L'ancien nom n'est pas gardé en alias : c'est
un refus à l'analyse (`&H68`), ajouté au jeu de refus.

`tests/run.py` compte maintenant quatre refus attendus
(`refus_directory`, `refus_pause`, `refus_fork`, `refus_sqr`).

### 2. Les fonctions, en radians

Toutes rendent un entier quand le résultat est entier, un réel sinon, comme
`SIN`, `COS` et `ATN`.

| famille | fonctions | conventions |
|---|---|---|
| directe | `tan`, `tan2`, `secant`, `cosecant`, `cotan`, `cotan2` | `tan2(x,y)` et `cotan2(x,y)` prennent la tangente (cotangente) du **rapport** `x/y`, la seule lecture compatible avec leur documentation d'origine ; diviseur nul → `undef` |
| hyperbolique | `hsin`, `hcos`, `htan`, `hsecant`, `hcosecant`, `hctan` | `sinh`, `cosh`, `tanh` de la libc ; `hsin(0)`, `htan(0)` → `undef` |
| réciproque | `actan`, `asecant`, `acosecant` | `actan` est l'arc **cotangente**, pas un alias : branche continue `PI/2-ATN(x)`, donc `actan(-1)` = `3*PI/4` ; `asecant`/`acosecant` exigent `abs(x)>=1` |
| réciproque hyperbolique | `ahsin`, `ahcos`, `ahtan`, `ahsecant`, `ahcosecant`, `ahctan` | aires hyperboliques : `asinh`, `acosh`, `atanh` de la libc ; `ahcos` demande `x>=1`, `ahtan` `abs(x)<1`, `ahsecant` `0<x<=1`, `ahcosecant` `x<>0`, `ahctan` `abs(x)>1` ; hors domaine → `undef` |

Les pôles ne sont détectés qu'à zéro exact du dénominateur : `sin`, `cos` et
`tanh` valent rarement zéro pour la valeur machine de `PI/2`, et le résultat
est alors un très grand réel, pas `undef`. C'est dit dans chaque bloc.

Six fonctions d'aire hyperbolique complètent la famille des six hyperboliques
directes : `ahsin`, `ahcos`, `ahtan`, `ahsecant`, `ahcosecant` et `ahctan`.
L'amont n'en avait aucune ; ce sont les réciproques manquantes, celles que
`hsin`/`hcos`/`htan` ne couvraient pas. Le nom suit `hsin` : `ah` pour
« area hyperbolica », comme `actan` pour l'arc cotangente.

Deux conversions sont ajoutées, `rad2deg` et `deg2rad`, avec `SBPI` comme
seule constante de pi du fichier.

### 3. `PI()` valait 3.1415926

Huit chiffres, donc `rad2deg(pi())` ne tombait pas sur 180 et `sin(pi())`
valait 6.5e-9. La constante est maintenant celle du double, et trois
vérifications la tiennent : `rad2deg(pi())` = 180, `rad2deg(pi()/2)` = 90 et
`round(sin(pi()),12)` = 0.

### 4. Arithmétique : deux défauts, une même cause

Un littéral au-delà de `LONG_MAX` saturait : `3000000000` valait
-2147483648 sous Windows, où `long` fait 32 bits. Le lexer accumulait dans un
`double` puis convertissait en `long` sans regarder la plage ; il rend
maintenant un **réel** dans ce cas, ce qui est déjà ce que fait l'addition
sur débordement.

`1000000*10000` valait 1410065408 : le produit était calculé en entier,
**débordait** (comportement indéfini en C), puis la vérification
« `lResult/lop1 == lop2` » était supprimée par le compilateur, qui a le droit
de supposer qu'un débordement signé n'arrive jamais. Le nouveau
`multiplyLongExact()` teste les opérandes **avant** de multiplier, par
division, et bascule en réel quand le produit ne tient pas.

```text
3000000000          → 3.000000e+09   (réel sous Windows, entier sous Linux)
1000000*10000       → 1.000000e+10
2147483647*2        → 4.294967e+09
1000*1000           → 1000000       (entier : rien ne change pour ce qui tient)
```

### 5. Documentation engendrée

`pod2md.py` lit les blocs `/**` des commandes (noyau et modules) et les
blocs `/*POD`, convertit le balisage POD (`=section`, `=H`, `=verbatim`,
`=item`, `T<>`, `R<>`, `B<>`, `I<>`) et écrit :

- `docs/MANUEL.md`, 210 entrées classées par section, avec sommaire ;
- `docs/modules/<module>.md` pour les quatorze modules qui ont des blocs, plus
  leur index `docs/modules/README.md`. `re` et `cgi` sont les deux pages
  exigées par le critère du ROADMAP.

`python pod2md.py --check` échoue si un fichier a divergé des sources : la
documentation ne peut plus pourrir en silence. Le texte reste celui des
sources, en anglais ; les documents de `docs/` restent en français.

Trois défauts de documentation sont corrigés au passage, et le contrôle
`--check` ne les voyait pas, puisqu'il compare les fichiers aux sources :

- `SGN` était écrite dans un bloc `/*SGN` à une étoile, que `pod2md.py` ne
  lisait pas : la fonction marchait mais n'avait aucune page ; le bloc est
  passé à `/**SGN` ;
- les pages de `GMTIMETOLOCALTIME` et `LOCALTIMETOGMTIME` s'intitulaient
  `GmToLocalTime()` et `LocalToGmTime()`, noms qui ne s'appellent pas ; elles
  portent maintenant le nom réel, l'ancien restant cité dans le texte ;
- `ISDEFINED` s'affichait `IsDefined()`, pour la même raison.

Les alias sont désormais écrits dans la page de leur fonction : `UPPER`,
`LOWER`, `TRIM$`, `LTRIM$`, `RTRIM$`, `LEFT$`, `RIGHT$`, `MID$`, et
`FORMATTIME` (second nom de `FORMATDATE`).

### 6. Vérifié

```text
windows-x64  par défaut   14/14   (10 scripts + 4 refus)
windows-x86  par défaut   14/14
linux-x64    par défaut   14/14   (via WSL)
windows-x64  --modules    18/18
windows-x64  --test manuel   156/156
windows-x64  --upstream      12 lancés, 0 échec, 20 sautés
```

Nouveaux fichiers de test : `tests/trig.bas` (73 vérifications),
`tests/nombres.bas` (19) et les trois refus. `tests/mathfunc.bas` reste à
76 vérifications ; `tests/commandes.bas` passe de 25 à 34.

Contrôles négatifs réels, armés puis désarmés, sources restaurées et
reconstruites vertes :

- `actan` réécrit en `ATN(1/x)` : `actan(-1)` rend -0.785398 au lieu de
  2.356194, le test tombe à 1 échec ;
- ancienne vérification de produit et ancienne conversion de littéral :
  `tests/nombres.bas` tombe à **13 échecs** sur 19.

### 7. Deux régressions trouvées en écrivant les tests d'alias

Les cinq alias ne sont pas le seul écart : en les essayant, `ucase("ab")`
suivi de `print "ab"` affichait `AB`, puis `AB`. `UCASE` et `LCASE`
écrivaient dans le tampon de leur argument, et pour un littéral ce tampon est
la constante partagée du programme. Les deux fonctions rendent maintenant une
nouvelle chaîne ([E35 et E36](ERRATA.md#e35)).

Le contrôle qui tient la correction n'est pas le résultat de `ucase`, qui
était juste : c'est la comparaison du littéral avec `chr(97) & chr(98)`,
construit à l'exécution. L'ancien code échouait sur cette ligne, le nouveau
passe.

---

## 2026-10-24 — dix-huitième passe : max, min, imax, imin, bin, gcd, lcm

Sept fonctions étaient annoncées dans le code (`=display GCD()`, `=display
BIN()`) et rendaient `undef` sans un mot : `COMMAND(X)` contenait
`NOTIMPLEMENTED` dans les deux branches de `#if NOTIMP_X`. Elles sont
maintenant construites.

### 1. Ce que chaque fonction décide

`GCD` et `LCM` acceptent deux arguments ou plus, comme la grammaire
l'exigeait déjà (2 à 100000). Chaque argument passe par `GETLONGVALUE`, donc
un réel est **tronqué vers zéro** — la règle déjà suivie par `HEX`, `OCT` et
`ROUND` — et le signe est ignoré : `gcd(-12,18)` vaut 6. Euclide est fait en
`unsigned long` pour `gcd`, en `double` pour `lcm` : un multiple de deux
`long` dépasse presque toujours un `long`, et la division avant la
multiplication évite l'overflow intermédiaire. `lcm` rend donc un entier
quand le résultat tient dans un `long`, un réel au-delà.

`MAX` et `MIN` comparent en réel (`GETDOUBLEVALUE`, donc une chaîne
numérique est acceptée) mais rendent **la valeur gagnante avec son type** :
`max(1,2)` est entier, `max(1,2.5)` est réel. En cas d'égalité c'est le
premier argument qui gagne.

`IMAX` et `IMIN` font la même comparaison et rendent **la position du
gagnant** au lieu de sa valeur. La position se compte à partir de **zéro**,
comme le premier indice d'un tableau dans ce langage, et c'est la première
qui gagne en cas d'égalité : `imax(5,3,9)` vaut 2, `imax(3,3)` vaut 0. Le
choix de l'indice zéro plutôt que un suit la mesure du langage, pas une
convention extérieure : `lbound(t[0]=1)` vaut déjà 0.

`BIN` est resté dans `string.c`, à côté de `HEX` et `OCT` dont il est le
frère : même conversion en `unsigned long`, mêmes chiffres écrits à l'envers
dans le tampon. Contrairement à `OCT`, le zéro est écrit explicitement, sans
quoi `BIN(0)` rendait un octet non initialisé.

Mesure de ce qui reste : **17** blocs `COMMAND` ont encore `NOTIMPLEMENTED`
dans leurs deux branches — `acosécante`, `actan`, `asécante`, `cosécante`,
`cotan`, `cotan2`, `fork`, `hcos`, `hcosécante`, `hctan`, `hsécante`, `hsin`,
`htan`, `pause`, `sécante`, `tan`, `tan2`. Les quatre familles d'accord :
les réciproques et hyperboliques trigonométriques (`tan`, `secant`,
`cosecant` et leurs variantes), `fork` et `pause`, que l'audit proposait
justement de retirer de la grammaire plutôt que de les construire.

### 2. Le piège des macros de retour

Première version de `MAX`, mesurée : `max(2.5)` rendait `error &H1:Not enough
memory`. La cause n'était pas l'allocation mais l'écriture
`if( cond )RETURN_LONG_VALUE(x)` : la macro se développe en **quatre
instructions**, dont seule la première reste sous le `if`. `RESULT` restait
`NULL`, puis `ASSERTNULL` levait le manque de mémoire. Toutes les macros de
retour du dépôt ont ce défaut de forme ; elles doivent être employées entre
accolades dès qu'elles sont conditionnelles, comme le fait déjà `ABS`.

### 3. Ce que la plateforme impose au test

Sous Windows, `long` fait 32 bits : `lcm(1000000,999999)` vaut exactement
999999000000 mais ne tient pas dans un `long`, il est donc rendu en réel,
alors que la même cible Linux le rend entier. Le test compare la **valeur**
(`= 9.99999e11`) et non le type. Deux mesures voisines, non corrigées ici et
à traiter dans une passe d'arithmétique :

- un littéral au-delà de `LONG_MAX` sature — `3000000000` vaut -2147483648 ;
- une multiplication de réels dont le produit dépasse `LONG_MAX` est calculée
  en entier — `1000000.0*10000` vaut 1410065408 au lieu de 10000000000,
  alors que l'addition correspondante, elle, passe en réel.

### 4. Vérifié

`tests/mathfunc.bas`, 76 vérifications, entre dans le jeu par défaut :

```text
windows-x64  par défaut   9/9     (8 scripts + 1 refus)
windows-x86  par défaut   9/9
linux-x64    par défaut   9/9     (via WSL)
windows-x64  --modules   16/16
```

Deux contrôles négatifs réels, armés puis désarmés, sources restaurées et
reconstruites vertes chaque fois :

- en retirant le cas zéro de `BIN` et la valeur absolue de `GCD`, le test
tombe à **3 échecs** (`gcd(-12,18)`, `gcd(-12,-18)`, `bin(0)`) ;
- en rendant l'indice de `IMAX`/`IMIN` à partir de 1, il tombe à **17
échecs**, dont les deux vérifications qui lisent `a[imax(...)]` — l'indice
décalé sort du tableau et rend `undef`.

---

## 2026-10-24 — dix-septième passe : les commandes disent la vérité

L'audit du langage vivait dans `build_tcc/EVOLUTIONS.md` §2.2. Re-mesuré sur
cet arbre, il avait vieilli : `isdefined(crypt("a","b"))` ne plante plus,
les 256 opérateurs morts, `pause`, la famille `cvd`/`mki` et `fork` ne sont
plus dans la grammaire, `atan` vit. En revanche la vérification a trouvé deux
défauts que l'audit ne nommait pas — un plantage et un silence — et montré
que « `open directory` échoue » recouvrait deux problèmes distincts.

### 1. Un plantage et deux silences

`nextfile(1)` sur un numéro jamais ouvert déréférençait `pDCO->dp[1]`, qui
vaut `NULL` : violation d'accès `0xC0000005`, pas une erreur. Même moule dans
`eod`, `reset directory` et `close directory`. Le piège se refermait d'autant
plus facilement que `open "." for directory as #1` s'analysait (point 3) et
échouait en 22 : le `nextfile` suivant tuait le processus. Les quatre
commandes rendent maintenant `33` (`DIR_NO_OPEN`), et `close directory` sur
un numéro libre aussi ([E26](ERRATA.md#e26)).

`print #9,"a"` sur un numéro libre n'écrivait rien et rendait la main, code
0 : la sortie était perdue sans un mot. `FPRINT` et `FPRINTNL` rendent
maintenant `20` hors plage et `23` sur une case libre
([E27](ERRATA.md#e27)). Les fonctions de lecture (`loc`, `lof`, `eof`)
gardent leur `undef` : elles interrogent, elles n'écrivent pas.

### 2. Trois réponses fausses

`isnumeric("12")` rendait faux pour **toute** chaîne : la branche
examinée était le type, pas le contenu. La chaîne est maintenant analysée
(espaces, signe, chiffres, point, exposant), sans `strtod` qui avalerait
`nan`, `inf` et la virgule selon la locale ([E29](ERRATA.md#e29)).

`formatdate("%Y-%m-%d")` rendait le motif tel quel : le langage de format
est verbal (`YEAR`, `MON`, `DD`…), donc `%` était un littéral et une faute de
format passait pour un résultat. Un `%` suivi d'une lettre ou d'un chiffre
rend désormais `&H30`, l'erreur prévue pour ce cas ; un `%` final reste
littéral ([E30](ERRATA.md#e30)).

`on error resume next` remettait `error` à zéro : l'échec devenait
indiagnosticable. La reprise ne touche plus `LastError`, comme `on error
goto` ne le faisait déjà pas ([E31](ERRATA.md#e31)).

### 3. `open … for directory` : refus à l'analyse

L'analyseur acceptait n'importe quel mot après `FOR` et le passait à `OPEN`
comme mode ; `directory` y tombait sur un `fopen` du nom, d'où le 22 à
l'exécution. Une liste blanche des modes (`socket`, `input`, `output`,
`append`, `random`, `binary` et leurs `_binary`) remplace cette tolérance :
tout autre mot est une erreur de syntaxe `0x77`. C'est la décision retenue
plutôt que l'alias : la seule forme de parcours de répertoire reste
`open directory … pattern … option … as` ([E28](ERRATA.md#e28)).

### 4. Deux assertions du manuel étaient rouges

`tests/manuel.bas` n'était plus vert avant cette passe, pour deux raisons
sans rapport avec elle : il testait `isdirectory("test")` (le dossier
s'appelle `tests`) et ouvrait `"."` avec le motif `"MANUEL.md"`, fichier qui
n'est pas à la racine de cet arbre. Les deux assertions sont corrigées
(`tests`, `README.md`), et les deux affirmations que cette passe change
(`isnumeric("12")`, `resume next`) sont retournées.

### 5. Couverture et vérification

`tests/commandes.bas` vérifie les six corrections (34 vérifications), et
`tests/refus_directory.bas` est le premier cas d'une table `REFUS_ANALYSE`
du lanceur : il exige `error &H77` et interdit l'exécution, là où un test
ordinaire exige l'absence de `error &H`.

Vérifié : jeu par défaut `8/8` (sept scripts et un refus) sous Windows x64 et
Linux x64, `--modules` `15/15` (Windows) et `14/14` (Linux), `--cgi-post`
`22/22`, `tests/manuel.bas` `156/156`. Les deux chaînes de compilation
`windows-x64` et `linux-x64` passent. Avant correctif, `nextfile` sur un
numéro libre sortait en `3221225477`.

---

## 2026-10-24 — seizième passe : les erreurs de script sortent en 500

### 1. `httpd_report` choisit le statut quand rien n'a été écrit

Jusqu'ici, tout échec autre qu'un refus de taille partait en `HTTP 200` avec
la page d'erreur : un client ne pouvait pas distinguer un script qui a
échoué d'un script qui a répondu. C'est le même défaut que [E23](ERRATA.md#e23)
pour les refus, étendu aux erreurs ordinaires.

`httpd_report` répond maintenant `500 Internal Error` quand deux conditions
sont réunies : la sévérité est au moins `REPORT_ERROR`, et le script n'a
encore rien écrit. Le second point est testé sur l'état du tampon de sortie
(`pATD->FirstHeaderLine` encore levé, `cbBuffer` à zéro) : si le script a
déjà commencé sa réponse, le statut est parti et le rapporteur se contente
d'ajouter le diagnostic (voir le point 3). Les refus `FILE_MAX` et `CONTENT_MAX`
gardent leur `413`.

La table `StateMessages` de `httpd.c` avait déjà l'entrée `500 Internal
Error` ; seul le choix du statut manquait.

### 2. Ce que le changement touche

Les deux chemins qui écrivaient une page d'erreur avant toute sortie sont
concernés : les erreurs de **chargement** (lecteur, include absent) et les
erreurs d'**exécution** avant la première écriture (script, premier appel de
module). Les trois cas d'erreur de `--cgi-post` passent donc de `200` à
`500`,et le
lanceur l'exige désormais. En mode `scriba -c` il n'y a pas de statut HTTP :
c'est le rapporteur du CLI qui écrit sa page CGI `Status: 200 OK`, et le cas
`erreur-chargement` continue de l'accepter comme résultat attendu. Sa propre
règle d'écriture est décrite au point 4.

La page d'erreur elle-même est inchangée : même `<HTML>`, même titre, même
ligne `error …`. Seule la ligne de statut diffère.

### 3. `httpd_report` ne réécrit plus de statut après le début de la réponse

Le choix du statut ne se lisait jusqu'ici que sur `REPORT_F_FRST`, le
drapeau du rapporteur : il dit « ce rapport est le premier », pas « le
script a déjà répondu ». Une erreur tardive repassait donc par le bloc
complet et insérait `HTTP/1.0 500 Internal Error`, `Content-Type: text/html`
et `<HTML>` au milieu du corps ([E25](ERRATA.md#e25)).

La condition est maintenant lue sur la sortie du script. `pfStdOut` retient
sa première ligne dans `szBuffer` et laisse `FirstHeaderLine` levé jusqu'au
saut de ligne qui l'envoie : tant qu'il est levé, rien n'est parti et le
statut est encore libre ; une fois retombé, la réponse a commencé et le
rapporteur se contente d'un saut de ligne puis du diagnostic.

Le détail qui compte est le contrôle négatif : un `szBuffer` non vide **ne**
signifie pas que la réponse a commencé. Traiter ce tampon comme une sortie
déjà envoyée supprime la ligne de statut et le client ne peut plus lire la
réponse — mesuré, `requete POST impossible`. C'est pourquoi
`tests/err_pending.bas` (première ligne sans saut, puis échec) exige un `500`
complet, pendant que `tests/err_partial.bas` (statut déjà écrit, puis échec)
exige un `200` sans seconde ligne de statut.

Vérifié : `--cgi-post` à 22/22 sous Windows x64, onze cas par mode, avec les
trois refus en `413`, les trois erreurs précoces en `500` et les deux cas
tardifs ci-dessus. `--modules` à 14/14, le jeu par défaut à 6/6, et la
compilation `linux-x64` de `sbhttpd` passe. Contrôle négatif : en rétablissant
le choix inconditionnel du bloc, le cas `erreur-apres-sortie` échoue sur les
trois lignes interdites du corps.

### 4. Le rapporteur du CLI suit la même règle

`scriba -c` portait le défaut du point 3, en pire : `scribacmd.c` passait au
rapporteur générique un mot de drapeaux qu'il n'initialisait pas. Une erreur
d'exécution recevait donc une page CGI complète ou rien du tout, selon ce que
la pile laissait dans `fErrorFlags`. Le lanceur n'avait mesuré que le
symptôme : il n'exigeait, en externe, que le code 22 et le diagnostic.

Le CLI a maintenant son rapporteur (`cli_CgiReport`), installé par
`scriba_SetReportFunction` quand `-c` est donné, et un crochet de sortie
standard qui retient qu'un octet est parti. Même règle que le serveur :
`Status: 200 OK`, `Content-Type: text/html` et la page tant que rien n'est
sorti ; ensuite un saut de ligne et le diagnostic, sans en-tête ni page. Le
mot de drapeaux local est remis à zéro, et `stndlone.c` comme
`variations/standalone/libscriba.c` — qui passaient le même mot non initialisé
à `report_report` — aussi : un bit `0x1` laissé par la pile y faisait
apparaître une page CGI dans un programme de console.

Une différence avec `sbhttpd` est assumée : le CLI écrit chaque octet
aussitôt, il ne tamponne pas la première ligne. `err_pending.bas`, dont la
sortie n'a pas de saut de ligne, a donc déjà envoyé `debut` quand il échoue :
le serveur peut encore répondre `500` avec la page, le CLI non. Le lanceur
exige les deux : `forbid` interdit `<HTML>` et `Content-Type: text/html` en
externe, `forbid_httpd` y ajoute `HTTP/1.0`, et `erreur-execution` comme
`erreur-module`, qui n'écrivent rien, doivent contenir la page complète et sa
ligne `Status: 200 OK`.

Les deux rapporteurs ne se doublent pas non plus : le moteur ne rapporte
qu'au chargement, `scriba_Run` rend son code sans rien écrire, et les deux
hôtes sortent dès qu'un chargement échoue. Le lanceur le compte maintenant
(`duplicate_report_problems` : un exemplaire du diagnostic attendu, un seul
`<HTML>` par réponse). Contrôles négatifs : un second rapport côté CLI, puis
un second côté serveur, font tomber la suite à 15/22 en nommant « diagnostic
en double », le second nommant aussi la page quand `REPORT_F_FRST` est remis
à zéro entre les deux.

Vérifié : `--cgi-post` à 22/22, `--modules` à 14/14, le jeu par défaut à
6/6, et les constructions `libscriba`, `stndlone` et `linux-x64 --httpd`
passent. Contrôle négatif : en forçant `response_started` à zéro (comportement
d'avant), les deux seuls cas en échec sont `externe/erreur-ligne-en-attente`
et `externe/erreur-apres-sortie`, sur les lignes interdites attendues.

---

## 2026-10-24 — quinzième passe : un refus d'upload sort en 413

### 1. `httpd_report` distingue le refus de taille

La quatorzième passe avait montré qu'un fichier plus gros que `FILE_MAX`
était refusé par le module `cgi` (code `0x00080008`), mais que `sbhttpd`
répondait `HTTP 200` avec sa page d'erreur : pour un client, un refus
ressemblait à un succès ([E23](ERRATA.md#e23)).

`httpd_report` reconnaît maintenant ce code. Comme le refus tombe au
démarrage du module, avant la moindre écriture du script, la réponse est
encore vierge : elle commence par `413 Request Entity Too Large`, puis
`Content-Type: text/plain`, puis le même texte d'erreur qu'avant. Un échec
ordinaire garde la page HTML en 200 (porté à 500 à la seizième passe). Le
statut 413 manquait à la table
`StateMessages` de `httpd.c`, qui aurait écrit `HTTP/1.0 413` sans phrase.

Le corps texte est conservé pour que le code reste lisible :
`error 0x00080008:Extension specific error: program terminated with error`.
Le journal `err.txt` n'a pas changé et porte toujours `error &H32`.

### 2. Le lanceur exige le statut

`cgi_httpd` prend un `expect_status` (200 par défaut) et les cas de refus
demandent 413. Les contrôles existants sont conservés : code `0x00080008`
dans le corps, absence de `cgi_upload : OK`, aucun fichier sauve. La page
HTML en 200 est vérifiée à part sur un autre échec (`unix.bas` sous Windows,
include absent).

### 3. Le module consomme le reste du corps

Le 413 n'arrivait pas toujours : avec un fichier nettement plus gros que
`FILE_MAX`, le module rendait l'erreur en laissant des kilo-octets non lus
dans la socket ; `sbhttpd` fermait alors une socket non vide, le noyau
envoyait un RST et le client perdait la réponse. `cgi_DrainInput` lit et
jette ce reste avant de rendre `CGI_ERROR_FILEMAX`, borné par le
`Content-Length` restant (lui-même borné par `lContentMax`) et arrêté au
premier EOF. Avec un dépassement d'un seul octet, le tampon du module avait
déjà absorbé la fin du corps : le défaut ne se voyait pas.

Le lanceur gagne donc un cinquième cas, `multipart-filemax-256k` (limite +
256 Ko), qui exige le même 413 et ne peut passer que si la fermeture est
propre. Il a servi de contrôle négatif : construit contre l'ancien `cgi` lié
en statique dans `sbhttpd`, il échoue sur `WinError 10054`. `--cgi-post`
comptait alors 10/10 sous Windows x64.

### 4. `CONTENT_MAX` et la ligne de statut

`CGI_ERROR_BIG_CONTENT` (`0x00080002`) est le pendant de `FILE_MAX` pour le
corps entier : le module refuse dès `Content-Length > lContentMax`, sans lire
un octet, ce qui est justement la protection. Le serveur le traduit en 413
comme l'autre refus. Le cas `contentmax` annonce `CONTENT_MAX + 1` sans
envoyer de corps : le refus tombe sur l'en-tête, la socket du serveur reste
vide et le 413 arrive sans compétition avec l'envoi. En contrepartie, un
client réel qui est encore en train d'envoyer au moment du refus peut perdre
la réponse : lire un corps jugé trop gros annulerait la protection.

Ce cas a révélé un second défaut : la réponse n'avait aucune ligne de statut
et le client levait `BadStatusLine`. `HttpProc` passait `&iErrorCode` comme
mot de drapeaux à `httpd_report`, qui y teste `REPORT_F_FRST` (`0x2`) ; or
`0x00080002` porte ce bit, donc l'en-tête était considéré déjà écrit. Une
vraie variable `fErrorFlags`, remise à zéro à chaque exécution, corrige le
tout ([E24](ERRATA.md#e24)). `--cgi-post` passe à 12/12 sous Windows x64.

### 5. Une erreur d'exécution fige E24

Les cas ci-dessus ne couvrent que des refus rendus par le module `cgi` ;
aucun ne passait par une erreur d'exécution ordinaire, celle qui fait
appeler `httpd_report` une seule fois, avec le code rendu par `scriba_Run`.
`tests/err_runtime.bas` ouvre un fichier dont le dossier n'existe pas :
l'erreur tombe à l'exécution, avant toute écriture du script, avec le code
22 (`0x16`), dont le bit `0x2` est levé. Le lanceur exige le code 22 en
externe, puis en interne un `200` et la page d'erreur complète.

Contrôle négatif fait : construit avec l'ancien `&iErrorCode`, `sbhttpd`
répond sans ligne de statut et le client lève `BadStatusLine` ; le cas
échoue donc bien sur le défaut qu'il doit figer. `--cgi-post` passe à 14/14
sous Windows x64, et `tests/` compte un `.bas` de plus.

Deux scripts complètent la couverture des chemins du rapporteur :

- `tests/err_load.bas` inclut un fichier absent. L'erreur est détectée au
  chargement par le lecteur, qui rapporte avec ses propres drapeaux ; le cas
  exige un `200` et `error &H44` côté serveur. En mode `scriba -c`, le CLI
  bascule son rapporteur en CGI et écrit la page complète sur stderr avant de
  sortir avec 0 : le lanceur accepte cette page comme résultat attendu
  (`cgi_page`), au lieu de la compter comme un échec du rapporteur.
- `tests/err_module.bas` appelle `cgi::SaveFile` sur un nom qui n'est pas un
  fichier téléversé. Le module rend `0x00080000`, au-delà de
  `MAX_ERROR_CODE` : `httpd_report` écrit le code en `0x%08x`, remplace le
  message par `COMMAND_ERROR_EXTENSION_SPECIFIC` et y insère le texte du
  module. Le test exige `error 0x00080000` et « The named file is not
  uploaded. ».

`--cgi-post` compte maintenant 18/18 sous Windows x64, avec neuf cas par
mode.

---

## 2026-10-24 — quatorzième passe : le refus au-delà de `FILE_MAX` est testé

### 1. Un quatrième cas POST, à la borne exacte

`--cgi-post` couvrait le POST urlencoded, un petit multipart ASCII et un
multipart de 65536 octets binaires. Il couvre maintenant le refus de taille :
un fichier de `FILE_MAX + 1` octets (`0xA00000 + 1`, soit 10 Mo + 1). Le
module `cgi` rend `CGI_ERROR_FILEMAX` (`0x00080008`) pendant son démarrage,
le script ne s'exécute pas et le lanceur exige : code de sortie `524296` et
ligne `error 0x00080008` en mode externe ; page d'erreur contenant le même
code et le message `program terminated with error` en mode interne ; aucune
ligne `cgi_upload : OK` dans les deux cas.

La borne a été mesurée avant d'être figée : `FILE_MAX` exact passe, `FILE_MAX
+ 1` est refusé. Le corps fait 10 Mo, donc ce cas coûte quelques secondes :
il a son propre délai (`"timeout"` du cas, borne `--cgi-post-timeout`) plutôt
que de ralentir les trois autres.

### 2. Ce que le test a révélé

Trois comportements du module et du serveur, désormais dans
[ERRATA E23](ERRATA.md#e23) :

- `sbhttpd` répondait `HTTP 200` et une page d'erreur pour ce refus, pas un
  413 (corrigé à la quinzième passe) ;
- sans `errmsgdest 3`, le serveur ne dit rien au client (défaut : journal
  seul) et le client voit une connexion fermée sans corps ; le lanceur ajoute
  donc `errmsgdest 3` à sa configuration temporaire, sans toucher à celle du
  dépôt ;
- dans `err.txt`, le code est remplacé par `COMMAND_ERROR_EXTENSION_SPECIFIC`
  (`error &H32`) ; seul le corps de la réponse porte `0x00080008`.

Le mode externe affiche `Extension specific error: %s` : `report_report`
reçoit un message `NULL` et imprime la chaîne de format. Le test vérifie le
code, pas ce texte.

---

## 2026-10-24 — treizième passe : `sbhttpd` vérifié, `re_engine` réimporté, audit des sources

`sbhttpd` a été reconstruit et exercé avec Zig sous Windows x64 : 52 unités
(45 du coeur + 7 `HTTPD_EXTRA`), écoute sur 18080, `index.bas` rend
`sbhttpd OK`, une URL inconnue rend le 404 personnalisé, 20 requêtes
parallèles rendent 20 fois `sbhttpd OK`, 10 erreurs 404 parallèles rendent
10 codes 404, et l'arrêt par suppression de `logs/pid.txt` fonctionne. Le
`sbhttpd64.exe` TCC, lancé sur le même port et la même arborescence, rend
les mêmes réponses : les sources sont communes, seuls le compilateur et les
correctifs de portabilité changent. Le binaire Zig fait 689 152 octets (le
`bin64/sbhttpd64.exe` présent en fait 825 856), le nombre d'unités est le
même (52), et `-DSTATIC_LINK=1` est devenu obligatoire pour `cgi`/`mt` : TCC
tolérait les symboles `versmodu`/`bootmodu` en double, lld non. Le mode service
Windows reste non testé.

`re_engine` est réimporté de `build_tcc/test/re_engine.c` vers
`tests/re_engine.c` ; `build.py --re-engine` le lie aux quatre fichiers du
moteur `re`, et `tests/run.py --re-engine` exige le verdict `moteur re : OK`.
Les 17 essais passent sous Windows x86, Windows x64 et Linux x64. C'est un complément à
`tests/re.bas`, pas un remplacement : il atteint `REG_BASIC`, `REG_PEND`, les
références arrière et `cnames`, que l'interface du module ne peut pas exposer.

Enfin, l'audit des `.c` sans cible a recensé 27 fichiers. `tools/fork.c` est
retiré (internes Cygwin, en-têtes `winsup.h`/`dll_init.h` absents). Les 26
restants sont gardés avec un statut explicite : modules non construits
(en-tête tiers, ABI Windows ou 32 bits, préprocesseur `dbg`), dépendances
(`rpool` pour `mysql`, `prepext`/`dbg`),
sources des `.h` (`command`, `matchc`, `notimp`, `buildnum`), diagnostics
(`testalloc`, `testconf`, `cftd`) et références historiques (`lmt_cio`,
`tools/md5`, `tools/mt19937`, `regex_test`). Le détail est dans
[PROGRAMMES.md § 2.2](PROGRAMMES.md#22-sources-sans-cible--décision-daudit-2026-10-24).

---

## 2026-10-24 — douzième passe : retrait de `basicdll` et `scribawx`

`src/variations/win32dll/`, `src/variations/wx/`, `src/lmt_wx.c` et
`src/tools/calldllbasic.c` ont été supprimés. Aucun des deux programmes
n'était constructible ici, et aucun n'avait de cible dans `build.py`.

`basicdll.c` ne contient que la déclaration exportée de `basic()` : dans
l'archive d'origine il pèse 550 octets et se termine sans corps de fonction.
Il n'y a donc pas d'implémentation à compiler, et `basicdll.h`, réclamé par
`calldllbasic.c`, n'est même pas fourni par l'archive. Sans corps de
`basic()`, aucune DLL utile ne peut être produite ([E21](ERRATA.md#e21)).

`scribawx` est un cas différent : l'archive d'origine fournit bien
`variations/wx/scribawx.c` **et** `wxmain.cpp`, et ce dernier porte le `main()`
wxWidgets qui appelle `mainwx()`. Le portage n'avait copié que `scribawx.c` et
`scribawx.h` ; il manquait donc l'hôte, et wxWidgets n'est pas dans le dépôt.
Aucune cible n'étant prévue et la dépendance n'étant pas disponible, la forme a
été retirée avec `lmt_wx.c`, qui n'existait que pour elle ([E22](ERRATA.md#e22)).

`calldllbasic.c` n'était que l'hôte d'essai de `basicdll` : il disparaît avec
elle. `lmt_cio.c` reste en place comme référence historique, car il est lisible
sans dépendance et ne gêne aucune cible. Le ménage ne change ni `SOURCES`, ni
`HTTPD_EXTRA`, ni les tests : aucun de ces fichiers n'était compilé. Les
documents et les liens croisés ont été mis à jour.

---

## 2026-10-24 — onzième passe : retrait de `basicc` et `basiccmd`

`src/variations/standalone/basicc.c` et `src/variations/standard/basiccmd.c`
ont été supprimés. Les deux étaient des doublons historiques, mais pour des
raisons différentes.

`basicc` était le runtime de l'ancien chemin `-C` : il portait son `main()` et
attendait un fichier C généré **sans** `main`, contenant seulement les
tableaux `szCommandArray`, `szStringTable` et leurs compteurs. Or `-C` produit
désormais lui-même un `main()` qui appelle `stndlone` ([builder.c:566](../src/builder.c#L566)).
Lier `basicc` avec cette sortie donne donc deux `main` ; et même avec un
ancien fichier sans `main`, `basicc` ne suit plus l'initialisation actuelle :
il contourne `execute_InitStructure`, laisse des champs de `ExecuteObject`
non initialisés, ne transmet pas les arguments de ligne de commande et sort
toujours avec le code 0. `stndlone` fait le même travail correctement.

`basiccmd` était l'ancien pilote `basic` : il compilait la source et
l'exécutait en manipulant directement `ReadObject`, `LexObject`, `eXobject`,
`BuildObject` et `ExecuteObject`. Il ne compilait plus en C23 (`main()` sans
type) et son appel à `epreproc` passait `malloc` là où l'interface attend un
`void *(*)(unsigned int)`, ce qui est incohérent sur 64 bits. `scribacmd` le
remplace, avec l'API `scriba_*`, les préprocesseurs internes et `-E`.

Les documents ont été mis à jour. `basicdll` et `scribawx` n'étaient pas des
doublons de code : ils ont été examinés à part et retirés à la douzième passe,
faute d'implémentation ou de dépendance disponible.

---

## 2026-10-24 — dixième passe : `--stndlone` et `--libscriba`

### 1. Le second chemin autonome entre dans le pilote

`--stndlone` construit `scriba -C`, compile le C obtenu, `src/stndlone.c` et
le coeur, puis lie le tout. Il exclut `scribacmd.o` (son `main`) et
`lmt_none.o` (`stndlone.c` définit déjà `StaticallyLinkedModules`). Le produit
est `dist/<cible>/hello_stndlone[.exe]` ; `tests/run.py --stndlone` vérifie
sa sortie sans lui donner de `.bas`. Vérifié sous Windows x86/x64 et Linux
x64 ; macOS reste à exécuter.

Deux corrections ont été nécessaires : `libscriba.c` déclarait `main()` sans
type de retour (refusé en C23), et le C produit par `-C` définissait aussi
`_environ`, ce qui donnait un symbole multiple sous UNIX avec `stndlone.c`.
La définition appartient au runtime ; elle a été retirée de `builder.c`.

### 2. `libscriba.a`, archive et non exécutable

`--libscriba` assemble 47 objets (coeur, commandes, `lmt_none.c`,
`libscriba.c`) dans `dist/<cible>/libscriba.a` et copie les en-têtes dans
`dist/<cible>/include/`. L'archive est construite pour les six cibles et
liée avec un hôte C sous Windows x64 et Linux x64. Sous UNIX, l'hôte doit
définir `char **_environ;` pour que l'archive ne tire pas `libscriba.o` et son
`main()`.

### 3. Les formes incomplètes sont nommées puis retirées

`basicdll.c` est une copie de `basicdll.h` : aucune implémentation de
`basic()` dans l'amont, donc aucune cible ([E21](ERRATA.md#e21)). `scribawx`
demande wxWidgets, absent du dépôt ([E22](ERRATA.md#e22)). `basicc` et
`basiccmd` étaient des doublons historiques ; ils ont été retirés lors de la
onzième passe, `basicdll` et `scribawx` à la douzième.

---

## 2026-10-24 — neuvième passe : `--standalone` dans le pilote

### 1. `scriba -E` plutôt que `-C` + `stndlone`

La recette manuelle `scriba -E` était déjà vérifiée : elle recopie
l'interpréteur et ajoute le bytecode. Elle ne demande aucun compilateur, alors
que le chemin `-C` + `stndlone.o` en demande un. `build.py --standalone`
industrialise donc `-E` :

```sh
python build.py --target windows-x64 --standalone
# -> dist/windows-x64/hello_standalone.exe
```

Le script vaut `tests/hello.bas` par défaut et peut être passé en argument. Le
produit s'appelle `<script>_standalone[.exe]`, à côté de `scriba.conf`. Le
pilote construit `scriba` d'abord s'il manque, et refuse `--all`, `--modules`
ou `--httpd` avec `--standalone` : une seule cible, exécutable ici.

### 2. Ce qui est vérifié, et ce qui ne l'est pas

`hello_standalone.exe` affiche bien les deux lignes de `tests/hello.bas` sous
Windows x86 et Windows x64 ; le PE x64 fait 609 051 octets. Sous Linux x64,
le pilote passe par WSL et produit `dist/linux-x64/hello_standalone`.
`tests/run.py --standalone` exécute ensuite le binaire sans lui donner de
`.bas` et exige les deux lignes ; c'est ce contrôle que la CI utilise. macOS
n'a pas été exécuté. Le chemin `-C` + `stndlone` restait alors une variante
manuelle ; la dixième passe en a fait `--stndlone` et a construit `libscriba`.
`basicc` et `basiccmd` n'avaient pas de cible ; ils ont été retirés.

### 3. Les 20 `manual.texi.jam` restent supprimés

Aucun n'est revenu avec cette cible. Les 20/20 sont absents de
`src/extensions/` ; il ne reste que deux `.jam` de données dans
`examples/sibawa/`. Le remplacement prévu reste `pod2md.py` ([ROADMAP P4](ROADMAP.md#p4--documentation)).

---

## 2026-10-24 — huitième passe : la suite upstream entre dans le lanceur

### 1. Un manifeste plutôt qu'un lancement des 32 fichiers

`tests/upstream/` contient 32 `.bas`, mais ce n'est pas un jeu homogène :
`testwin.bas` écrit dans le registre, `testcio.bas` boucle sur une console
interactive, les cinq tests `curl/` demandent un module non construit et un
réseau externe, `test_ext_trial.bas` charge `ext_trial`, un nom qui n'existe
plus. Lancer les 32 produirait des échecs sans rapport avec le portage.

`tests/upstream/MANIFEST.tsv` décrit donc chaque fichier : catégorie, modules
requis, OS, répertoire de travail, mode (`defaut`, `all`, `jamais`), résultat
attendu et raison du saut. Aucun `.bas` de l'amont n'est modifié. Le lanceur
refuse un `.bas` présent sans ligne dans le manifeste, et une ligne qui ne
correspond à aucun fichier : le manifeste ne peut pas devenir une liste
partielle.

### 2. `--upstream` et `--upstream-all`

`tests/run.py --upstream` lit le manifeste et lance les tests du coeur et des
modules réellement construits. `--upstream-all` ajoute `test_sleep` (20 s),
`roundbugin1-0b27` et `seggfault`. Un module absent, un OS différent ou un
mode `jamais` donne une ligne `SAUTE` avec sa raison ; ce n'est pas un échec.

Les tests `testmt` et `testmtlock` reçoivent le même faux environnement CGI
que le test `cgi`, sans quoi leur chargement échoue en 0x00080004. Le lanceur
continue de considérer comme échec un code non nul, `ECHEC`, `error &H` ou
`error 0x` : ScriptBasic rend 0 même sur une erreur (E3).

Mesure du 2026-10-24, Windows x64 et Linux x64, avec les modules construits :
`12 lance(s), 0 echec(s), 20 saute(s)` ; avec `--upstream-all`,
`15 lance(s), 0 echec(s), 17 saute(s)`.
Les 17 sauts restants sont ceux du manifeste : réseau externe, `curl`,
`mysql`, `odbc`, `bdb`, `cio`, `testwin`, `test_ext_trial`, `stresstest`,
`kill`, `mkdir`. `stresstest.bas` est explicitement hors jeu : il dépasse la
pile du parseur (0xC00000FD sous Windows x64) et ne prouve pas une
non-régression.

---

## 2026-10-23 — septième passe : `hash` réparé, `trial` séparé

### 1. `hash::gethv` et `hash::thishv` rendent une copie

Les deux fonctions renvoyaient une « référence mortelle » via
`besNEWMORTALREF`. Or `memory_NewMortalRef` est un bouchon volontaire : la
structure `FixSizeMemoryObject` utilise déjà `next` pour la chaîne des
références, et ne peut donc pas ranger une référence dans une liste mortelle.
Le gestionnaire rendait `NULL`, et chaque lecture échouait avec l'erreur 1
(`Not enough memory`). `examples/hash.bas` échouait ainsi avant même d'afficher
la première valeur.

Décision : **ne pas toucher au gestionnaire mémoire**, et faire des deux
fonctions des copies. `sethv` n'accepte déjà que `string`, `long`, `double` et
`undef` ; `gethv`/`thishv` recopient ces mêmes types avec
`besALLOC_RETURN_*`. Le module ne peut plus modifier une valeur par référence,
mais aucun programme du dépôt ne le faisait : les exemples ne faisaient que
lire.

`hash.bas` expose maintenant `hash::Value` et `hash::ThisValue` directement.
Les noms `refValue` et `refThisValue` disparaissent : ils promettaient une
référence qui n'a jamais fonctionné. `tests/hash.bas` couvre les trois types,
la copie indépendante et `ThisValue`, et `examples/hash.bas` affiche bien
`666` deux fois.

### 2. `trial` sort des artefacts de production

`trial` est un module d'essai de l'ABI. Il reste construit par `--modules`
pour que `tests/trial.bas` continue de le charger, mais il est marqué
`test_only` dans `build.py`. `--modules --release` le saute et supprime un
`trial.dll`/`.so`/`.dylib` resté d'une construction précédente. Le binaire
`scriba` et `sbhttpd` ne le liaient déjà pas.

### 3. `dyc` reste exclu

Rien ne change : `dyc` n'est pas dans `MODULES`, et son ABI d'appel par pile
est structurellement 32 bits. Ce n'est pas une correction de type ; un
portage 64 bits demanderait une autre couche d'appel.

Vérifié : `build.py --all` 6/6 ; `--modules` 14/14 sous Windows x86 et x64,
13/13 sous Linux x64 ; `tests/hash.bas` et `examples/hash.bas` passent ;
`--modules --release` omet `trial` et laisse `trial.dll` absent.

---

## 2026-10-23 — sixième passe : doublons et alias du langage

Après l'audit des opérateurs, un second audit a porté sur les **noms** :
alias, variantes grammaticales et fonctions mortes. Décision prise avec le
mainteneur, nom par nom :

| nom | décision |
|---|---|
| `elif`, `elsif`, `else if` | retirés ; seul `elseif` reste |
| `ucase$`, `lcase$` | retirés ; `ucase`/`lcase` et `upper`/`lower` restent |
| `mkd`, `mki`, `mks`, `mkl` et leurs `$` | retirés |
| `cvd`, `cvi`, `cvs`, `cvl` | retirés |
| `atan` | implémenté ; `atn` reste son alias |
| `now` | conservé comme seule forme ; `time` retiré |
| `kill`, `delete` | conservés : ce ne sont pas des doublons |
| `formatdate`, `formattime` | laissés pour une passe ultérieure |

Les 12 noms morts (`mkd`/`mki`/`mks`/`mkl`, avec et sans `$`, plus
`cvd`/`cvi`/`cvs`/`cvl`) disparaissent de `NASYMBOLS` et de
`INTERNALFUNCTIONS` ; leurs entrées dans `CommandFunction[]` deviennent
`NULL`, ce qui conserve la numérotation, et leurs corps `COMMAND(...)`,
leurs déclarations dans `syntax.h` ainsi que les macros `NOTIMP_*` sont
retirés de `string.c`, `syntax.h` et `notimp.h`.
`ucase$` et `lcase$` disparaissent de même, sans toucher à `ucase`, `lcase`,
`upper` et `lower`.

`time` n'était qu'un second nom de `now` : il disparaît aussi de `NASYMBOLS`.
Les trois programmes qui l'appelaient encore (`examples/time.bas`,
`examples/testtime.bas` et `tests/upstream/time.bas`) passent à `now` ; le
reste de leur code, notamment `TimeValue`, `GmTime`, `LocalTimeToGmTime` et
`GmTimeToLocalTime`, est inchangé. Comme `time` n'est plus un symbole, un
programme qui l'emploie encore ne reçoit pas une erreur mais une variable
ordinaire, de valeur 0 : c'est une rupture silencieuse, et la raison pour
laquelle les trois exemples ont été migrés dans le même changement.

Pour les variantes de `if`, les quatre blocs de `syntax.c` se réduisent à
celui de `elseif` : `EX_LEX_NSYMBOL 305` + `302` (`else if`), `306`
(`elif`) et `307` (`elsif`) sont supprimés, ainsi que les symboles et les
macros `KEYWORDCODE_ELSIF`/`KEYWORDCODE_ELIF` correspondants. `END IF` et
`ENDIF` restent équivalents, comme `goto`/`go to` et `gosub`/`go sub`.

`atan` reçoit le corps de `sin` et `cos` (`atan(GETDOUBLEVALUE(Op1))`),
dans `mathfunc.c` ; `atn` reçoit le même corps. Aucun nouvel opérateur ni
nouvelle dépendance.

Les exemples qui utilisaient la forme deux mots sont migrés :
`examples/sibawa/cgi-bin/editacl.bas`, `setacl.bas`, et les chaînes
produites par `examples/createtestif.bas` et `ctestif.bas`.

Vérifié : `build.py --all` 6/6 ; `tests/run.py --target windows-x64` 6/6,
`--modules` 14/14 sous Windows x86 et x64, 13/13 sous Linux x64 ;
`tests/manuel.bas` avec trois assertions supplémentaires sur `atan`/`atn` ;
`examples/time.bas`, migré vers `now`, s'exécute et affiche une différence
nulle entre `now` et `TimeValue`. Le refus de `else if`, `elsif` et `elif`
est vérifié par des programmes
temporaires, qui rendent une erreur **&H77** à l'analyse ; `ucase$`,
`lcase$`, `mkd` et `cvd` rendent **&H68** (« user function … is not
defined »), puisqu'ils ne sont plus des symboles mais restent des appels
de fonction inconnus.

---

## 2026-10-23 — quatrième passe : 245 opérateurs étendus retirés, cinq ajoutés

Le langage déclarait **257 opérateurs à deux caractères**. L'audit du langage
en comptait 256 refusés par `extops.c` (`NOTIMP`, erreur 28) **comme
opérateurs d'expression**, seul `#` vivant. En les reprenant un par un, un
détail est apparu : six d'entre eux (`+=`, `-=`, `*=`, `/=`, `&=`, `\=`)
**fonctionnaient déjà** comme instructions, parce que `expression.c` les
intercepte dans `LET` et les convertit en
`LETP`/`LETM`/`LETS`/`LETD`/`LETC`/`LETI` sans jamais appeler le corps
`NOTIMP` de `extops.c`.

Décision : garder ces six affectations composées, `#`, et ajouter cinq
opérateurs qui comblent un vrai manque :

| opérateur | sens | mise en œuvre |
|---|---|---|
| `<<` | décalage à gauche | `EXTOPDD`, en `unsigned long` pour éviter le débordement signé |
| `>>` | décalage à droite arithmétique | `EXTOPEE`, par complément pour ne pas dépendre de l'implémentation |
| `??` | gauche si définie, sinon droite | `EXTOPFF`, la droite n'est évaluée que si nécessaire |
| `**` | puissance | alias de `^` : `NASYMBOLS` pointe directement sur `CMD_POWER` |
| `//` | division entière | alias de `\` : `NASYMBOLS` pointe sur `CMD_IDIV` |

Les 245 autres symboles ont été **retirés de `NASYMBOLS`**, ainsi que de
`BINARIES`, `UNARIES`, `extops.c` et `syntax.h` ; leurs entrées dans
`CommandFunction[]` deviennent `NULL`, ce qui conserve la numérotation des
commandes (l'index est `code - START_CMD`). Leur emploi est désormais une
erreur **&H77 à l'analyse**, plus une erreur 28 à l'exécution. Vérifié sur
`+^`, `==`, `!=`, `&&` et `@`.

`&&` a été essayé puis écarté : le corps ne recevait qu'un argument, et la
cause n'a pas été élucidée ; `and` et `xor` couvrent déjà le besoin. Ne pas
l'ajouter vaut mieux qu'un opérateur à moitié défini.

Vérifié : `build.py --all` 6/6 cibles ; `tests/run.py --target windows-x64` 6/6
(nouveau `tests/operators.bas`), `--modules` 14/14 sous Windows x86 et x64,
13/13 sous Linux x64.

---

## 2026-10-23 — troisième passe : `cgi` et `mt` chargeables, `sbhttpd` sous Linux, décision sur les modules tiers

### 1. `cgi` et `mt` deviennent des modules, et sont testés

Ils étaient liés en statique dans `sbhttpd` et n'avaient donc aucun test : un
module qu'aucun hôte ne charge ne se vérifie pas. `build.py --modules` les
produit maintenant comme les autres, et deux tests les exercent :

- `tests/mt.bas` fait le cycle complet d'une session (création d'un
  identifiant MD5 de 32 octets, affectation au fil, relecture, compteur,
  suppression) ;
- `tests/cgi.bas` simule une requête GET en posant les variables
  d'environnement (`REQUEST_METHOD`, `QUERY_STRING`…), puis vérifie
  `QueryString`, `GetParam` et l'en-tête HTTP imprimé par `cgi.bas`.

Ce dernier point est une contrainte du module, pas du test : son `bootmodu`
lit la requête dès le chargement, et sans `REQUEST_METHOD` il bascule en mode
« fichier de débogage » et rend `0x00080004`. `tests/run.py` a donc une table
`TEST_ENV`, volontairement limitée à `cgi`. Vérifié : 13/13 sous Windows x86
et x64, 12/12 sous Linux x64.

### 2. `sbhttpd` compile et sert sous Linux (E7)

Trois corrections : `<arpa/inet.h>` dans `httpd.c` et `websrv.c`,
`stricmp`/`strnicmp` ramenés à `strcasecmp`/`strncasecmp` hors Windows, et
`static dummy()` de `service.c` remis en C23. Vérifié par
`build.py --target linux-x64 --httpd`, puis par une requête réelle : le
serveur rend `sbhttpd OK` à `curl --http0.9`.

### 3. Les modules à bibliothèque tierce ne sont pas planifiés, sauf `mysql`

Le critère est l'usage dans le dépôt, pas l'utilité théorique : aucun exemple,
test ou page ne charge `psql`, `bdb`, `xml`, `curses` ou `zlib` ; `mysql` est
la seule bibliothèque qui rend exécutable une application présente (SIBAWA).
La décision complète, avec les coûts et les licences, est dans
[MODULES.md § 6.3](MODULES.md#63-décision--les-modules-à-bibliothèque-tierce).

---

## 2026-10-23 — création du dépôt `scriba-zig`

Décision de départ : **un dépôt neuf**, indépendant de `build_tcc` et du
dossier amont, qui ne contient que ce qui est nécessaire pour compiler avec
`zig cc`.

- sources **plates** : chaque `.h` est un vrai fichier ; plus de `TO_HEADER:`
  recopié par un outil Perl ;
- `build.py` remplace les `Makefile*`, les `.jam`, les `.cmd` et TinyCC ;
- six cibles d'un seul coup (`windows-x86`, `windows-x64`, `linux-x64`,
  `linux-arm64`, `macos-x64`, `macos-arm64`) ;
- `tests/run.py` remplace les scripts d'essai épars ;
- `.github/workflows/build.yml` reconstruit et teste sur trois systèmes.

Vérifié alors : 6/6 cibles compilées, tests 5/5 sur Windows x86, Windows x64
et Linux x64 (WSL), clone Git à froid reconstruit à l'identique.

---

## 2026-10-23 — deuxième passe : ménage, compléments, serveur

### 1. Les 20 `manual.texi.jam` sont supprimés

Ils ne contenaient que `MODULE=…` et `FILE=interface.c`, et pointaient vers
un `skeleton.jim` absent du dépôt. La documentation réelle est dans les blocs
`/*POD` des `interface.c`, qui restent. Le remplacement est un **tableau**
dans [MODULES.md](MODULES.md), pas un nouveau fichier par module.

*Pourquoi pas les convertir en Markdown ?* Parce qu'ils ne portaient aucun
texte : convertir un fichier de trois lignes en un autre fichier de trois
lignes n'apporte rien. Ce qui manque, c'est le générateur, et il est proposé
au [ROADMAP](ROADMAP.md).

### 2. Les `.bas` de déclaration des modules sont rétablis

15 manquaient (`bdb`, `cio`, `curl`, `curses`, `dbg`, `gd`, `hash`, `mt`,
`mysql`, `odbc`, `psql`, `t`, `trial`, `ux`, `zlib`) ; ils ont été repris de
la distribution d'origine, ainsi que `include/error.bas`, `include/time.bas`
et `include/modinst.bas`. Sans eux, `import hash.bas` ne peut pas fonctionner,
même avec un `hash.dll` construit : c'était un trou fonctionnel, pas
cosmétique.

### 3. `examples/` et `tests/upstream/` reviennent dans le dépôt

178 programmes d'exemple et les 32 fichiers de la suite d'origine. Ils ne
sont pas exécutés par `tests/run.py` (beaucoup exigent des modules), mais ils
sont la matière première des prochains tests et la seule documentation
« par l'exemple » du langage.

### 4. `sbhttpd` devient une cible de `build.py`

`python build.py --target windows-x64 --httpd` construit le serveur HTTP
multi-thread, et il a été **lancé et interrogé** : page servie, page 404
servie, journaux écrits.

Trois corrections ont été nécessaires pour passer de TCC à clang :

| fichier | correction | raison |
|---|---|---|
| `src/variations/httpd/service.h` | prototypes de `ReportStatusToSCMgr`, `ServiceStart`, `ServiceStop` | TCC acceptait les déclarations implicites, clang non |
| `src/extensions/mt/interface.c` | `STRINGVALUE((*p)->vV) = NULL` → `(*p)->vV->Value.pValue = NULL` (2 endroits) et `besPROCFREE(STRINGVALUE(…))` → `besPROCFREE((*p)->vV->Value.pValue)` | un transtypage n'est pas une valeur affectable en C standard |
| `src/basext.h` | `_init`/`_fini` déclarés à portée de fichier, avec la liaison de `STATIC_LINK` | une fonction ne peut pas être déclarée `static` dans un bloc |

`-DSTATIC_LINK=1` est appliqué aux deux modules liés (`cgi`, `mt`) : sans lui,
chacun exporte `versmodu`, `bootmodu`, `finimodu`… et `lld` refuse les
doublons.

### 5. `webtest/` et `logs/`

Deux pages d'essai (`index.bas`, `404.bas`) sont versionnées : elles rendent
le serveur démontrable en une commande. `logs/` est ignoré (journaux et
fichier pid écrits à l'exécution).

### 6. Les documents de suivi sont écrits

`PROGRAMMES.md`, `MODULES.md`, `ERRATA.md`, `ROADMAP.md`, `INVENTAIRE.md`,
`REPRISE.md` et `AGENTS.md`. Ils remplacent les notes éparses de la campagne
TCC et donnent à un humain — ou à un agent — le point d'entrée qui manquait.

### 7. Un sommaire, et les deux `.jam` de l'exemple SIBAWA

`docs/README.md` sert de sommaire : ordre de lecture, état en un tableau,
réponse aux questions récurrentes (`.jam`, `.bas` manquants, plateformes du
serveur, CGI, formes embarquées) et liste des fichiers qu'un clone doit
contenir pour être reprenable.

Décision sur les **deux `.jam` restants** (`examples/sibawa/`) : ils sont
conservés, parce que ce sont les données de l'application d'exemple et non
des modèles de build. Aucun outil du dépôt ne les lit ; les régénérer demande
Jamal, absent. Le jour où SIBAWA tourne, on pré-générera ses gabarits ou on
réécrira `mktm.cmd` en Python ([ROADMAP P7](ROADMAP.md#p7--exemple-sibawa)).

Ce choix évite deux erreurs symétriques : réintroduire Perl/Jamal dans le
build pour deux fichiers d'exemple, ou supprimer des données d'exemple pour
pouvoir dire « il n'y a plus de `.jam` ».

---

## 2026-10-23 — troisième passe : les modules `hash`, `re`, `t`, `trial`

`build.py --modules` construit les quatre modules sans dépendance tierce en
bibliothèques partagées, pour les six cibles, et copie leurs déclarations dans
`modules/include/`. `tests/run.py --modules` les éprouve. Résultat vérifié :
6/6 cibles construites, 9/9 tests sur Windows x86, Windows x64 et Linux x64.

### 1. Quatre modules, pas vingt

Le choix est celui des dépendances, pas de l'envie : `hash`, `re`, `t` et
`trial` ne demandent que la libc. Les quatorze autres attendent une bibliothèque
(`curl`, `xml`, `zlib`…), l'API Windows (`cio`, `win`, `dbg`), l'ABI 32 bits
(`dyc`) ou une base de données (`mysql`, `psql`, `odbc`). Les mélanger dans une
première cible aurait rendu l'échec d'un seul module indistinguable des
autres. Voir [MODULES.md § 6](MODULES.md#6-construire-les-modules-aujourdhui).

### 2. Deux pièges de plateforme, tous deux dans le chargement

**L'interposition ELF** ([E18](ERRATA.md#e18)) : le moteur de `re` définit
`regcomp`, `regexec`, `regerror` et `regfree`, comme la libc. Chargé par
`dlopen`, le module résolvait ces noms vers la libc, plus tôt dans la portée
globale : `regcomp` de glibc écrivait une `regex_t` de 48 octets et plus dans
une de 32. Symptôme : `re::n()` rendait 224, puis le tas mourait
(`malloc(): unaligned tcache chunk detected`). Windows ne pouvait pas le
montrer, faute de `regcomp` dans sa CRT.

La correction ne touche pas le moteur : `DLL_EXPORT` déclare désormais
`__attribute__((visibility("default")))` hors Windows, et les modules se
compilent avec `-fvisibility=hidden`. Les fonctions du BASIC restent visibles
(elles passent toutes par `besFUNCTION`/`besCOMMAND`), le reste est privé.

**La configuration introuvable sous UNIX** ([E19](ERRATA.md#e19)) :
`scriba.conf` n'est cherché à côté de l'exécutable que sous Windows. Sous
Linux et macOS, sans `SCRIBACONF`, l'interpréteur démarre avec une
configuration vide, donc sans clé `module` — et le message est
`The requested module can not be loaded`, qui accuse le module alors que
c'est la configuration qui manque. `tests/run.py` pose `SCRIBACONF` et se
place dans `dist/<cible>`.

### 3. Le lanceur de tests ne croyait pas au code de sortie

En écrivant ces tests, une mesure a invalidé une hypothèse de `run.py` :
ScriptBasic rend **0** même sur une erreur de syntaxe, un include introuvable
ou un module absent. Un test qui se contentait du code de sortie passait donc
à tort. Le lanceur traite maintenant comme échec un code non nul, une ligne
`ECHEC` ou un message `error &H…` (voir [ERRATA E3](ERRATA.md#e3)).

### 4. `trial.bas` écrit pour l'occasion, `re.bas` corrigé

`tests/trial.bas` est nouveau : la distribution ne livrait aucun test de ce
module, et sa déclaration porte des chemins `D:\MyProjects\…`. Il exerce le
compteur du bloc interne, la commande paresseuse `iff` et `pprint`.

`src/extensions/re/re.bas` déclarait `CaseInsensitive = &HFFFFFFFE` et
`MultiLine = &HFFFFFFFD`. Ces littéraux valent -2 et -3 quand `long` fait
32 bits (Windows), mais 4294967294 et 4294967293 en LP64 : les constantes
étaient fausses sur Linux et macOS 64 bits. Elles sont désormais écrites en
décimal négatif, ce qui donne la même valeur partout.

---

## 2026-10-23 — quatrième passe : `cio`, `win`, `unix` et le renommage

`build.py --modules` construit maintenant **sept modules** : les quatre
précédents, plus `cio` et `win` sous Windows et `unix` sous Linux/macOS.
Aucun n'a de dépendance tierce : `cio` et `win` n'appellent que l'API
Windows, `unix` que la libc POSIX. Vérifié : 6/6 cibles construites,
11/11 tests sous Windows x64, 10/10 sous Linux x64 (WSL).

### 1. `nt` devient `win`, `ux` devient `unix`

Même renommage que la vague NG, et pour la même raison : `nt` nomme un
système disparu, pas ce que fait le module. Le renommage va jusqu'au bout :
dossier, `.bas`, `lib "win"`, et les fonctions exportées (`win_shutdown`,
`win_startservice`, `unixfork`, `unixchmod`…). Aucun script livré n'utilisait
les anciens noms, donc rien n'est cassé — et le portage ne garde pas une
bizarrerie de plus que NG.

### 2. `cio` sous redirection

`GetCSBI` lisait `CONSOLE_SCREEN_BUFFER_INFO` sans l'initialiser quand la
sortie n'était pas une console (fichier, tube, CI) : `GetConsoleScreenBufferInfo`
échouait et `SizeX` rendait par exemple -2535. La structure est désormais
remise à zéro avant l'appel ; le module rend des zéros au lieu de valeurs
fausses. C'est ce qui permet à `tests/cio.bas` de tourner sans terminal.

### 3. Tests

`tests/cio.bas`, `tests/win.bas` (registre, liste des processus) et
`tests/unix.bas` (`fork`, `chmod`) entrent dans le jeu `--modules`, filtrés
par plateforme. `tests/win_hardlink.bas` reste dehors : il faut les droits
administrateur pour créer un lien dur. `tests/upstream/testnt.bas` devient
`testwin.bas` et appelle `win::`.

---

## 2026-10-23 — cinquième passe : C23

`build.py` compile en `-std=c23`. Le passage a demandé de corriger dans les
sources ce que C23 refuse et que `gnu99` tolérait :

- **définitions K&R** (60) : `sym.c` (1), `md5.c` (8), moteur `re` (51, dont 41
  dans `regcomp.c` et 6 dans `engine.ci`). Converties en prototypes typés, sans
  toucher aux types de retour (`int __stdcall`, `size_t __stdcall`…).
- **listes `()`** (43) : en C23, `()` veut dire `(void)`. Vrais prototypes dans
  `expression.h` (`CommandFunctionType`, `pfAnalyzeFunction`), `httpd.h` (les 15
  pointeurs de `_fun`), `basext.h`, `myalloc.h`, `thread.h`, et
  `tools/global.h` (`PROTOTYPES` passe à 1). `pLineSyntax` et `pThreadData`
  n'étant pas encore déclarés à cet endroit, les prototypes nomment les
  structures (`struct _LineSyntax *`, `struct _ThreadData *`).
- **`int` implicites** (4) : `isinset` dans `lsp.c`, `lexer.c`, `expression.c` ;
  `main` dans `scribacmd.c` et `websrv.c`.
- **`extern char *index();`** dans `getopt.c` : prototype `(const char *, int)`
  sous `#ifndef index`.
- **C généré par `-C`** : `builder.c` écrit `int main` et le prototype de
  `stndlone` ; [E8](ERRATA.md#e8) est corrigé, la recette autonome n'exige plus
  `gnu89`.

Deux ajustements de cible : `_DEFAULT_SOURCE` (Linux) et `_DARWIN_C_SOURCE`
(macOS), car `-std=c23` est un mode ISO strict qui cache `fileno`, `ftruncate`,
`kill`, `S_IFDIR` et `h_addr`.

Effet de bord utile : la signature réelle de `pWriteClient` a révélé deux
`WriteClient(&ch, 1)` qui passaient un `int *` pour un `char *` ; ils passent
désormais un `char`.

Vérifié : 6/6 cibles, 6/6 modules, 11/11 sous Windows x86 et x64, 10/10 sous
Linux x64, 152/152 assertions du manuel, `sbhttpd` (page et 404), et le C
produit par `-C` compilé en `-std=c23`, lié puis exécuté.

---

## Décisions prises une fois pour toutes

| question | décision | raison |
|---|---|---|
| quel compilateur ? | **Zig uniquement** (`zig cc`) | un seul outil, six cibles, en-têtes et bibliothèques fournis |
| TinyCC ? | abandonné | pas de 64 bits fiable, pas de macOS, pas de ARM |
| en-têtes générés ? | non, **plats** | un `.h` qu'on peut lire et corriger ; plus de « ne pas éditer » |
| Perl, Jamal, TeXinfo ? | absents | ils ne servaient qu'à la chaîne d'en-têtes et de manuels |
| bibliothèques tierces ? | **aucune** pour l'interpréteur | les modules qui en dépendent restent optionnels |
| modules liés en statique ? | seulement dans `sbhttpd` (`cgi`, `mt`) | c'est la liste de `lmt_httpd.c`, héritée du projet |
| version de C ? | **`-std=c23`** | plus de K&R ni de `()` vide ; `_DEFAULT_SOURCE`/`_DARWIN_C_SOURCE` pour POSIX |
