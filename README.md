# BasicScript

> Interpréteur BASIC moderne — portage du moteur ScriptBasic 2.0 vers **C23**, avec trajectoire vers **Zig 0.17+**.

BasicScript est un fork de [ScriptBasic](https://github.com/verhas/ScriptBasic) (© Peter Verhas, LGPL-2.1) entrepris par Mickaël CALA en vue d'une réécriture complète : mêmes capacités de langage, code neuve, testée, sans dette de 25 ans d'accumulation.

**État du projet : Phase 0 terminée, le cœur `src/` est vide.** Lisez la [ROADMAP.md](ROADMAP.md) pour savoir où on en est et où l'on va ; ce dépôt contient actuellement l'archive de référence (`old/`) et la documentation de pilotage.

## Structure du dépôt

| Chemin | Rôle |
|--------|------|
| `README.md` | Ce fichier — présentation |
| `LICENSE` / `old/LICENSE.txt` | LGPL v2.1 (copie racine + archive) |
| `ROADMAP.md` | **Où l'on va** : phases 0→6 du portage |
| `ERRATA.md` | **Ce qui s'est passé** : bugs & pièges connus de l'ancien moteur |
| `BENCHMARKS.md` | **Où l'on est** : mesures de performance, objectif ≥ ancien moteur |
| `BOOTSTRAP.md` | Remise en route à partir du dépôt nu (outils, builds, tests) |
| `CHANGELOG.md` | Suivi des changements (format Keep a Changelog) |
| `CONTRIBUTING.md` · `CODE_OF_CONDUCT.md` · `SECURITY.md` · `AGENTS.md` | Cadre communautaire & instructions IA |
| `old/` | Sources ScriptBasic 2.0 complètes — **lecture seule**, référence comportementale |
| `src/` | Nouveau moteur C23/Zig (à naître) — modules : `memory/`, `lexer/`, `parser/`, `runtime/`… |
| `test/` | Tests : `unit/`, `language/` (`.bas` + `.expected`), `regression/` (`E-NNN.bas`) |
| `build/` | Système de build moderne (zig build / CMake — RFC à venir) |
| `doc/language/` | Spécification : mots-clés, fonctions, erreurs, modules |
| `doc/history/` | Historique de développement + changelog hérité amont |
| `extensions/` | Modules additionnels portés (curl, re, hash…) |
| `examples/` | Scripts BASIC de démonstration |
| `tools/` | Utilitaires de dev (harness de tests, bench) |
| `.github/` | Templates d'issues/PR, future CI Actions |

## Documentation du langage

Extraite des définitions du moteur historique (`old/syntax.def`, `old/errors.def`) :

- [Mots-clés & structure](doc/language/keywords.md) — IF/FOR/SUB/MODULE… (34 réservés de contrôle)
- [Fonctions & commandes intégrées](doc/language/functions.md) — ~500 identifiants par domaine (chaînes, math, fichiers, réseau…)
- [Gestion des erreurs](doc/language/errors.md) — ON ERROR / TRY, 131 codes
- [Extensions](doc/language/modules.md) — les 20 modules legacy et leur sort dans le portage

## Démarrer

```sh
# 1. Lire le contexte
cat ROADMAP.md ERRATA.md BOOTSTRAP.md

# 2. Compiler l'ancien moteur (référence de compatibilité)
cd old && make -f Makefile    # détails : old/build.txt

# 3. Le nouveau moteur n'existe pas encore — contribuer = prendre une case [ ] de la ROADMAP
```

Voir [BOOTSTRAP.md](BOOTSTRAP.md) pour l'outilchain complète et le parcours de première contribution.

## Contribuer

Lisez [CONTRIBUTING.md](CONTRIBUTING.md) et [AGENTS.md](AGENTS.md) (si vous êtes une IA). Toute contribution est sous LGPL-2.1. Code of Conduct : [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md). Vulnérabilités : [SECURITY.md](SECURITY.md).

## Licence

LGPL v2.1 — copyright des sources historiques : Peter Verhas et contributeurs ScriptBasic ; nouvelles contributions : leurs auteurs respectifs. Texte complet : `LICENSE` (racine) et `old/LICENSE.txt`.
