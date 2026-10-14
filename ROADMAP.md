# ROADMAP — BasicScript (portage C23 / Zig)

> **Où l'on va.** Document vivant : mettre à jour les cases `[x]` au fil de l'eau.
> État actuel : **Phase 0**. Dernière mise à jour : 2026-10-14.

## Vision

Réécrire ScriptBasic 2.0 (C89/C99, hérité des années 1998–2020) en un interpréteur BASIC moderne, propre et testable, en **C23** avec une trajectoire vers **Zig 0.17+**, sans régression fonctionnelle visible pour les scripts existants.

## Principes directeurs

1. **Compatibilité d'abord** : les scripts `.bas` de `old/test/` et `old/examples/` sont la spécification comportementale.
2. **Petits pas vérifiables** : chaque module porté passe ses tests avant le suivant.
3. **Pas de big-bang** : on ne réécrit pas tout d'un coup ; cohabitation `src/` (nouveau) ↔ `old/` (référence).
4. **Zéro warning** : `-Wall -Wextra -Wpedantic -Werror` dès le premier commit de code.

---

## Phase 0 — Fondations du dépôt ✅ *(achevée)*
- [x] Archiver les sources historiques dans `old/`
- [x] Structure standard : `src/`, `test/`, `build/`, `doc/`, `extensions/`, `examples/`, `tools/`, `.github/`
- [x] Fichiers cadre : README, LICENSE, CONTRIBUTING, CODE_OF_CONDUCT, CHANGELOG, SECURITY, ROADMAP, ERRATA, BENCHMARKS, BOOTSTRAP
- [x] Documentation langage dans `doc/language/` (mots-clés, fonctions, gestion d'erreurs) extraite de `old/`

## Phase 1 — Spécification & harness ⬜
- [ ] Inventaire complet des mots-clés/fonctions/modules (à partir de `old/syntax.def`, `old/errors.def`) → `doc/language/`
- [ ] Migrer les scripts de test historiques dans `test/language/` avec sorties `.expected` générées par l'ancien binaire
- [ ] Mise en place du build moderne (`build/build.zig` ou CMake + Ninja) et de la CI GitHub Actions
- [ ] Gel du périmètre v1 : ce qui est porté, ce qui est abandonné (ex. modules exotiques `bdb`, `odbc` ?)

## Phase 2 — Noyau C23 (lexer → parser → runtime minimal) ⬜
- [ ] `src/memory` : allocateur + gestion des pools (remplace `old/myalloc.c`, `old/memory.c`)
- [ ] `src/lexer` : tokeniseur (réf. `old/lexer.c`) + tests unitaires
- [ ] `src/parser` : syntaxe & arbres (réf. `old/syntax.c`, `old/expression.c`)
- [ ] `src/runtime` : pile, variables, types de base, chaînes
- [ ] Bootstrap : faire tourner `PRINT "HELLO"` puis les boucles/conditions
- [ ] Atteindre « Hello world + calculs » aussi rapide que l'ancien moteur (cf. BENCHMARKS.md)

## Phase 3 — Langage complet ⬜
- [ ] Fonctions intégrées (string, math, conversion) — réf. `old/basext.c`, `old/commands/*`
- [ ] Fichiers & E/S (`filesys.c`)
- [ ] Préprocesseurs interne/externe (`ipreproc.c`, `epreproc.c`)
- [ ] Gestion des erreurs & exceptions (`errcodes.c`, `error.html` → doc)
- [ ] GOTO/PERFORM/MODULE/DECLARE, tableaux dynamiques
- [ ] 100 % des tests `test/language/` verts

## Phase 4 — Extensions & outils ⬜
- [ ] API module revisitée (`extensions/`) : curl, re, hash, zlib, socket… (tri des modules legacy)
- [ ] Serveur HTTP/LSP : décision garder/réécrire (`old/httpd.c`, `old/lsp.c`)
- [ ] Débogage (`dbg`), profiling
- [ ] Outils dans `tools/` : générateurs de doc, convertisseurs

## Phase 5 — Portage Zig (optionnel, progressif) ⬜
- [ ] Décision RFC : Zig comme outil de build uniquement vs. réécriture module par module
- [ ] Premiers modules portés en Zig (lexer candidat n°1)
- [ ] Interop C↔Zig stabilisée, LTO, compilation croisée native (wasm inclus)

## Phase 6 — Stabilisation & release v1.0 ⬜
- [ ] Campagne de benchmarks contre ScriptBasic 2.0 (objectif : ≥ 100 % de la vitesse)
- [ ] Audit mémoire (ASan/UBSan/Valgrind) sur toute la suite de tests
- [ ] Documentation utilisateur finale, paquets (deb/rpm/tar.gz)
- [ ] Release publique v1.0 + gel d'API

---

## Hors roadmap (décisions prises)
- Pas de compatibilité binaire des anciens modules `.so` : seule la source compte.
- `old/` n'est jamais modifié (référence en lecture seule).

## Comment contribuer à cette roadmap
Proposer une RFC dans `doc/rfcs/` puis ouvrir une issue. Les milestones GitHub reflètent les phases ci-dessus.
