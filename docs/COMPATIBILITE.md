# COMPATIBILITE — ce qu'on a cassé exprès, ce qu'on garantit

La question tenait en une ligne : *que doit-on encore à ScriptBasic
d'origine ?* La réponse tient en une page, et elle est appliquée plutôt
que débattue : **pas de compatibilité bug-pour-bug**. La rupture est le
produit ; ce document dit où elle s'arrête et comment la mesurer.

## 1. Ce qui a été cassé exprès (et où c'est écrit)

| retiré / changé | référence | pourquoi |
|---|---|---|
| familles `cv*`/`mk*` (conversion binaire legacy) | ERRATA E34 | sources de portage et d'abus mémoire |
| `pause`, `fork`, `sqr` | ERRATA E34 | refusés à l'analyse au lieu de planter à l'exécution |
| les 256 opérateurs étendus (`==`, `+^`…) | ERRATA E34, EVOLUTIONS 2.2 | bruit du lexer, ambiguïtés |
| modules renommés : `nt`→`win`, `ux`→`unix` | EVOLUTIONS | noms honnêtes |
| formes de grammaire non portées (`open` sans `for`, `sub` dans `module`…) | ERRATA E45 | pas de règle, pas de forme ; elles reviendront par la passe langage avec contrat et refus |
| erreurs d'ABI documentées, non corrigées au prix de la compat | ERRATA E11, E33, E43 | juste avant facile |

## 2. Ce qui reste acquis à l'ancien

- **Les 178 exemples** (`examples/`) restent le corpus pédagogique. Leur
  état réel est mesuré par `scripts/exemples.py` et publié dans
  [EXEMPLES.md](EXEMPLES.md) : jouables aujourd'hui, ou hors jeu avec leur
  raison (module absent, interaction, spécifique Windows…). On ne les
  reformatte pas — c'est une copie de l'amont.
- **La suite amont** (`tests/upstream/`, 32 programmes) est un
  **instrument de mesure**, jamais un contrat : `--upstream` la rejoue et
  chaque saut porte sa raison. Elle dit objectivement de combien on a
  divergé, elle n'oblige à rien.
- **Le « look » BASIC** est garanti : mots-clés insensibles à la casse,
  `module`/`end module`, `declare sub … alias … lib`, suffixes `$` des
  chaînes, `if/then/else` en bloc et sur une ligne. Les contrats de
  `tests/contrat/` et les 156 affirmations de `tests/manuel.bas` le
  verrouillent : toute divergence devient consciente ou n'a pas lieu.

## 3. Ce qu'on garantit, et qui le vérifie

| garantie | qui le vérifie |
|---|---|
| le langage de base se comporte comme le manuel dit | `tests/manuel.bas` (156 affirmations, Windows) |
| la sortie des constructions courantes ne bouge pas sans décision | `tests/contrat/` (snapshots, trois cibles) |
| un nom retiré ne revient pas sans contrat de refus | les 17 refus de la campagne |
| chaque module livré se charge et répond | `--modules` (24/24, 24/24, 21/21) |
| un produit ne ment pas sur son architecture | `scripts/arch.py` |

## 4. Identité et renommage (`basicscript`, non décidé)

Aucun nom de projet n'est codé en dur dans le C : les symboles
(`scriba_*`, `bes*`) sont l'ABI historique et ne bougent pas ; les noms
visibles sont des fichiers (`scriba`, `scriba.conf`, `sbhttpd`) et des
documents. Le jour du renommage, la liste fermée des points à toucher est :

1. le nom du dépôt GitHub et du dossier local ;
2. les noms de binaires produits (`build.py` : `scriba`, `sbhttpd`) et le
   nom de la configuration (`scriba.conf`, `scriba.conf.lsp`) ;
3. la documentation (une passe `grep` guidée par ce fichier) ;
4. `src/version.txt` — l'unique lieu de la version.

Le symbole d'appel des modules (`versmodu`, `bootmodu`, `besFUNCTION`) ne
change pas : c'est une ABI, pas une marque.
