# Changelog

Tous les changements notables de ce projet sont documentés ici.

Le format suit [Keep a Changelog](https://keepachangelog.com/fr/1.1.0/),
et le projet adhère au [Semantic Versioning](https://semver.org/lang/fr/).

## [Unreleased]

### Added
- Réorganisation complète du dépôt (Phase 0 de la ROADMAP) :
  - Sources historiques ScriptBasic 2.0 archivées en lecture seule dans `old/`.
  - Nouvelle structure : `src/`, `test/`, `build/`, `doc/`, `extensions/`, `examples/`, `tools/`, `.github/`.
  - Documentation standard : README, LICENSE, CONTRIBUTING, CODE_OF_CONDUCT, SECURITY, AGENTS.
  - Documents de pilotage : ROADMAP.md, ERRATA.md, BENCHMARKS.md, BOOTSTRAP.md.
  - Documentation langage extraite de `old/` : mots-clés (`doc/language/keywords.md`), fonctions (`functions.md`), gestion des erreurs (`errors.md`).
  - Historique de développement : `doc/history/CHANGELOG-legacy.md` (ScriptBasic amont) et `doc/history/devlog.md`.

### Changed
- `License.txt` déplacé vers `old/LICENSE.txt` ; une copie conforme `LICENSE` est maintenue en racine (exigence LGPL).

### Deprecated
- Le système de build hérité (Makefiles/Jam/Jim multiples) n'est plus maintenu ; voir `old/build.txt`.

### Removed
- *(rien)*

### Fixed
- *(rien)*

### Security
- *(rien)*

## [2.0] — hérité (ScriptBasic upstream)
Voir `doc/history/CHANGELOG-legacy.md` pour l'historique complet de la base amont.
