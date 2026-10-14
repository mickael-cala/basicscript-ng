# AGENTS.md — instructions pour les agents IA et outils automatisés

Ce fichier est lu par les assistants de codage (Claude Code, Copilot, Cursor, etc.). Respectez ces règles lors de toute intervention sur ce dépôt.

## Contexte du projet

- **BasicScript** = portage moderne de ScriptBasic 2.0 vers **C23**, puis **Zig 0.17+**. Voir [ROADMAP.md](ROADMAP.md).
- Le dépôt vient d'être réorganisé (Phase 0 terminée). L'état réel du code neuf : `src/` est **vide**. Ne supposez aucune API existante ailleurs que dans `old/`.

## Règles structurelles (inviolables)

1. **`old/` est en lecture seule.** Ne modifiez, déplacez ni « nettoyez » aucun fichier de `old/`. C'est la référence comportementale et juridique (LGPL) du portage.
2. Tout nouveau code va dans `src/<module>/` (un module = un dossier = une responsabilité : `memory/`, `lexer/`, `parser/`, `runtime/`…).
3. Les tests accompagnent chaque module : `test/unit/` (code), `test/language/` (scripts `.bas` + `.expected`), `test/regression/` (bugs ERRATA, nommés `E-NNN.bas`).
4. La spécification du langage vit dans `doc/language/`. Si vous découvrez un comportement non documenté dans `old/`, documentez-le avant de coder.
5. Le système de build cible vivra dans `build/` (zig build ou CMake — voir ROADMAP Phase 1). N'introduisez pas de Makefiles ad hoc à la racine.

## Normes de code

- Langage : **C23** (`-std=c23 -Wall -Wextra -Wpedantic -Werror`) ; passerelle Zig autorisée via `@cImport` uniquement si une RFC l'accepte.
- Zéro warning toléré. Pas de `malloc/free` directs : utiliser l'allocateur de `src/memory/` dès qu'il existe.
- Identifiants : fonctions/variables `snake_case`, types `PascalCase`, macros `SCREAMING_SNAKE_CASE`.
- Chaque fichier source commence par l'en-tête LGPL-2.1 + mention de la référence amont (`Ported from old/<fichier>.c`).
- Commits : [Conventional Commits](https://www.conventionalcommits.org/) — `feat:`, `fix:`, `docs:`, `test:`, `refactor:`, `chore:`. Une PR = un objectif de la ROADMAP, référencé.

## Documentation & pilotage — obligations de mise à jour

Après toute contribution significative :

- [ ] `ROADMAP.md` : cocher/rayurer la tâche réalisée.
- [ ] `CHANGELOG.md` : ajouter une entrée sous `[Unreleased]`.
- [ ] `ERRATA.md` : enregistrer tout bug historique découvert (format `E-NNN`).
- [ ] `BENCHMARKS.md` : joindre les mesures si le hot path est touché.
- [ ] `doc/language/` : mettre à jour mots-clés/fonctions/erreurs si le comportement change.

## Ce qu'il NE FAUT PAS faire

- Régénérer/inventer des fonctionnalités absentes de la ROADMAP sans RFC (`doc/rfcs/`).
- Supprimer ou réécrire la doc legacy de `old/texi`, `old/html` (elles alimentent `doc/`).
- Changer la licence ou retirer les mentions de copyright amont.
- Toucher aux fichiers de CI/build sans vérifier que la suite de tests passe.

## Points d'entrée utiles pour un agent

- Démarrer une session : lire BOOTSTRAP.md §1 puis ROADMAP.md (phase en cours).
- Trouver quoi implémenter : cases `[ ]` de la phase active de ROADMAP.md.
- Vérifier un comportement BASIC : `old/test/`, `old/examples/`, `doc/language/`.
