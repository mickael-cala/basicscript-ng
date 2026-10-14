# BENCHMARKS — mesures de performance

> **Où l'on est, côté perf.** Référence : l'ancien moteur ScriptBasic 2.0 compilé depuis `old/`. Objectif de la roadmap : le nouveau moteur (`src/`) doit atteindre ≥ 100 % de la vitesse de référence avant la v1.0.

## Méthodologie

- **Machine de référence :** à figer (CPU, RAM, OS, noyau, compilateur + flags). Actuellement : *non définie — à remplir au premier run.*
- **Protocole :** 5 exécutions par benchmark, on retient la **médiane** ; écart-type rapporté. Température ambiante stable, governor CPU fixé, sans autres charges.
- **Commande type :** `tools/bench/run.sh <binaire> <bench.bas>` (à créer en Phase 1).
- **Comparaison :** ancien vs nouveau sur **exactement les mêmes scripts**, mesurée en temps wall-clock (`hyperfine` recommandé).

## Jeux de benchmarks (à migrer depuis `old/test/` et nouveaux)

| ID | Script | Ce qu'il mesure | Cible phase |
|----|--------|-----------------|-------------|
| B-01 | `bench_hello.bas` | Démarrage à froid (init runtime) | P2 |
| B-02 | `bench_loop1e6.bas` | Boucle FOR 1..1 000 000, arithmétique entière | P2 |
| B-03 | `bench_str_concat.bas` | Concaténations de chaînes (10⁵ itérations) | P3 |
| B-04 | `bench_substr.bas` | MID$/LEFT$/RIGHT$ intensifs | P3 |
| B-05 | `bench_array.bas` | Tableaux multidimensionnels, tri simple | P3 |
| B-06 | `bench_call.bas` | Appels de fonctions/GOSUB imbriqués (pile) | P3 |
| B-07 | `bench_file_io.bas` | Écriture/lecture fichier séquentiel 10 Mo | P3 |
| B-08 | `bench_regex.bas` | Module `re` (si porté) | P4 |

## Résultats

### Référentiel — ScriptBasic 2.0 (ancien moteur, gcc -O2)

*(vide — à exécuter dès que `old/` compile sur la machine de référence)*

| Date | Version | B-01 | B-02 | B-03 | B-04 | B-05 | B-06 | B-07 | Notes |
|------|---------|------|------|------|------|------|------|------|-------|
| — | — | — | — | — | — | — | — | — | pas encore lancé |

### Nouveau moteur (`src/`)

| Date | Commit | Phase | B-01 | B-02 | Ratio vs réf. | Notes |
|------|--------|-------|------|------|---------------|-------|
| — | — | — | — | — | — | code src/ non démarré |

## Suivi des régressions

- Toute PR touchant le hot path doit joindre le delta hyperfine pour B-02 et B-03.
- Régression > 5 % = bloquante, sauf justification documentée ici.

## Historique des décisions perf

_(ex. : choix de l'allocateur, bytecode vs arbre, chaîne rope vs copie — chaque arbitrage sera consigné ici avec les chiffres à l'appui.)_
