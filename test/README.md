# test/ — suite de tests BasicScript

- `unit/`       : tests unitaires C/Zig (par module, harness à définir Phase 2)
- `language/`   : conformité sémantique du langage (scripts .bas + sorties attendues)
- `regression/` : bugs connus (ERRATA) et cas historiques de `old/test`
- `golden/`     : sorties de référence produites par l'ancien moteur (à générer une fois le binaire de référence buildé — voir BOOTSTRAP.md)
- `scripts/`    : corpus hérité — `legacy/` (149 exemples de `old/examples`) et `testsuite/` (32 scripts de `old/test`)

Lanceur prévu : `tools/test/run_all.sh` (ROADMAP Phase 1).
