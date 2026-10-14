# Contribuer à BasicScript

Merci de votre intérêt pour le projet ! Ce document explique comment contribuer.

## Code de conduite

Ce projet applique le [Code of Conduct](CODE_OF_CONDUCT.md) (Contributor Covenant). En participant, vous acceptez de le respecter. Signalez tout comportement inacceptable à l'adresse indiquée dans ce document.

## Avant de commencer

1. Lisez le [README.md](README.md) et la [ROADMAP.md](ROADMAP.md) pour comprendre où va le projet.
2. Consultez les [issues](https://github.com/Mickael-Cala-GIT/BasicScript/issues) — une issue ouverte peut déjà couvrir votre idée.
3. Pour un changement important (nouvelle syntaxe, nouvelle API, nouveau module), ouvrez d'abord une **RFC** : créez `doc/rfcs/NNN-titre.md` (numéro suivant) décrivant le problème, la solution proposée et les alternatives, puis ouvrez une issue pour la discuter.

## Environnement de développement

- Compilateur C compatible **C23** (GCC ≥ 14, Clang ≥ 19) ou **Zig ≥ 0.17** selon la phase de la roadmap.
- Sous Linux : `make -C build` (à terme ; voir [BOOTSTRAP.md](BOOTSTRAP.md)).
- Les sources historiques (C89/C99) sont dans `old/` : elles sont en **lecture seule** — ne les modifiez pas, servez-vous-en comme référence.

## Workflow

1. Forkez le dépôt et créez une branche depuis `dev` : `feat/sujet`, `fix/sujet`, `docs/sujet`.
2. Un commit = une logique. Messages de commit au format [Conventional Commits](https://www.conventionalcommits.org/) :
   `feat: add CHR$ builtin`, `fix(memory): off-by-one in buffer grow`, etc.
3. Ajoutez des tests pour tout nouveau comportement (voir « Tests » ci-dessous).
4. Lancez localement : build + suite de tests + `-Wall -Wextra -Werror` (et l'analyse statique si disponible).
5. Ouvrez une Pull Request vers `dev`, en liant l'issue correspondante (`Fixes #123`).

## Tests

- Les tests vivent dans `test/` :
  - `test/unit/` — tests unitaires du compilateur/runtime ;
  - `test/language/` — scripts `.bas` de validation du langage (attendus en stdout comparés aux fichiers `.expected`) ;
  - `test/regression/` — anciens bugs reproduits sous forme de tests.
- La suite complète doit passer avant merge. Une PR qui casse un test existant doit le réparer ou expliquer la régression.

## Style de code

- **C23** : indentation 4 espaces, accolades à la K&R, noms en `snake_case` pour les fonctions, types en `PascalCase` si struct.
- Pas de variables globales mutables sans justification ; allocation mémoire via l'allocateur du projet uniquement.
- Le code doit compiler sans warning avec `-Wall -Wextra -Wpedantic -Werror`.
- Commentaires : expliquez le *pourquoi*, pas le *quoi*. Référencez les numéros d'issue/RFC.

## Rapports de bug

Utilisez le template adéquat dans `.github/ISSUE_TEMPLATE/`. Incluez : version, plateforme, commande exacte, script reproducteur minimal, attendu vs obtenu.

## Licence

Toute contribution est soumise à la licence **LGPL v2.1** du projet. En poussant du code, vous acceptez de concéder vos droits sous cette licence.
