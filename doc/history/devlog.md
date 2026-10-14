# Historique de développement — BasicScript

> **Ce qui s'est passé.** Journal chronologique du projet. Pour l'historique détaillé des versions amont (ScriptBasic), voir [CHANGELOG-legacy.md](CHANGELOG-legacy.md).

## Ère amont : ScriptBasic (1998 → 2020)

- **1998** — Peter Verhas crée ScriptBasic, interpréteur BASIC portable en C, publié sur SourceForge.
- **Années 2000** — Croissance de l'écosystème : extensions (`mysql`, `xml`, `curl`, `gd`…), serveur HTTP embarqué (`httpd.c`), système de build multiplateforme (Makefiles, Jam, scripts Windows `.cmd`).
- **2010s** — Portage GitHub ([verhas/ScriptBasic](https://github.com/verhas/ScriptBasic)) ; modernisation progressive, ajout d'un LSP server (`lsp.c`), préprocesseurs interne/externe.
- **Version 2.0** — État figé dans le présent dépôt : ~635 fichiers, noyau C89/C99, licence LGPL-2.1.

## Ère fork : BasicScript (2026 → )

### 2026 — Création du fork par Mickaël CALA
- Renommage « BasicScript », intention déclarée : **réécriture pour C23 et Zig 0.17**.
- Le code reste alors identique à ScriptBasic 2.0 (aucun portage entamé) — cf. ERRATA E-002.

### 2026-10-14 — Réorganisation du dépôt (ROADMAP Phase 0) ✅
- **Archivage** : toutes les sources historiques déplacées (avec suivi Git des renommages) dans `old/`, déclarées lecture seule.
- **Nouvelle structure** : `src/` (code neuf), `test/`, `build/`, `doc/`, `extensions/`, `examples/`, `tools/`, `.github/`.
- **Fichiers cadre créés** : `README.md` (réécrit), `LICENSE`, `CONTRIBUTING.md`, `CODE_OF_CONDUCT.md`, `SECURITY.md`, `AGENTS.md`, `CHANGELOG.md`.
- **Documents de pilotage** : `ROADMAP.md` (phases 0→6), `ERRATA.md` (3 entrées initiales), `BENCHMARKS.md` (protocole + 8 bancs), `BOOTSTRAP.md` (remise en route).
- **Documentation langage** (`doc/language/`) extraite automatiquement des définitions `old/syntax.def` et `old/errors.def` : mots-clés (34), fonctions/commandes (500 identifiants, 13 domaines), erreurs (131 codes), modules (20 extensions).
- **Historique** : création de ce journal et de `CHANGELOG-legacy.md`.

### Prochain jalon (Phase 1 — Spécification & harness)
- Migrer les tests `.bas` de `old/test/` vers `test/language/` avec sorties attendues générées par l'ancien binaire recompilé.
- Choisir et implanter le système de build (`build/build.zig` vs CMake) via une première RFC dans `doc/rfcs/`.

## Convention de rédaction

Chaque entrée de ce journal : date, action, référence (issue/PR/commit). Les décisions structurantes prennent aussi une RFC datée. Ce fichier ne documente **pas** les changements de code (rôle du CHANGELOG.md).
