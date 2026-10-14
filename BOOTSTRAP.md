# BOOTSTRAP — démarrer de zéro

> **Comment repartir du dépôt nu jusqu'au premier `PRINT`.** Ce guide s'adresse à toute personne (humaine ou IA) qui doit remettre en place l'environnement de développement.

## 1. Comprendre l'état du projet

Ordre de lecture recommandé :

1. [README.md](README.md) — ce qu'est le projet ;
2. [ROADMAP.md](ROADMAP.md) — où l'on va (phases 0→6) ;
3. [ERRATA.md](ERRATA.md) — les pièges connus de l'ancien code ;
4. [BENCHMARKS.md](BENCHMARKS.md) — la barre de performance à atteindre ;
5. `doc/` — spécification du langage (mots-clés, fonctions, erreurs) ;
6. `old/` — référence d'implémentation (lecture seule, ne jamais modifier).

## 2. Prérequis outilchain

| Outil | Version mini | Usage |
|-------|--------------|-------|
| GCC ou Clang | GCC ≥ 14 / Clang ≥ 19 | compilation C23 (`-std=c23`) |
| Zig | ≥ 0.17 | build system moderne + futur portage |
| GNU Make / Ninja | récent | orchestration |
| Git | ≥ 2.40 | workflow branches |
| hyperfine | ≥ 1.18 | benchmarks |
| Perl | ≥ 5.30 | uniquement pour régénérer des artefacts depuis `old/` (`setup.pl`, générateurs `.pl`) |

Vérification rapide :

```sh
gcc --version    # ou clang --version
zig version
git --version
```

## 3. Compiler l'ancien moteur (référence de compat)

Nécessaire pour générer les sorties `.expected` des tests et les benchmarks de référence.

```sh
cd old
make -f Makefile        # cible gcc unix ; lire build.txt si échec
./scriba ../test/samples/hello.bas   # fumet test : doit afficher quelque chose
```

Notes :
- Sur macOS/Cygwin, utiliser les variantes `Makefile.*` correspondantes.
- Les dépendances des extensions (mysql, curl…) ne sont **pas** requises : ne construire que le noyau.

## 4. Construire le nouveau squelette (`src/`)

État actuel : **squelette vide** (Phase 1 non démarrée). Dès que le build existe :

```sh
zig build -p build-out run -- --version     # cible prévue
# ou, pendant la transition C23 :
cmake -S build -B _b && cmake --build _b
```

Tant que ces cibles n'existent pas, cette section est un contrat à implémenter (voir ROADMAP Phase 1).

## 5. Lancer les tests

```sh
tools/test/run_all.sh          # à créer en Phase 1
```

Conventions (voir CONTRIBUTING.md) :
- `test/language/*.bas` + `.expected` → diff stdout/code de sortie ;
- `test/regression/E-NNN.bas` → liés aux entrées d'ERRATA.md ;
- unitaires via `zig test` ou framework C choisi en Phase 1.

## 6. Premier commit de code neuf — checklist

- [ ] Un module autonome avec son header d'API dans `src/<module>/`
- [ ] Ses tests unitaires dans `test/unit/`
- [ ] Build vert sans warning (`-Werror`)
- [ ] Référence croisée vers le fichier `old/` correspondant dans la doc du module
- [ ] ROADMAP.md mise à jour (case cochée)

## 7. Problèmes fréquents au démarrage

| Symptôme | Cause probable | Remède |
|----------|----------------|--------|
| `make` d'`old/` échoue sur les `.pl` | artefacts générés manquants | lancer `perl setup.pl` puis relire `old/build.txt` |
| Erreurs `-Wincompatible-pointer` en C23 | code old/ en C89/K&R | ne pas compiler `old/` en C23 ; cible `-std=c99` pour la référence |
| Tests `.expected` instables | horodatages/aléa dans les scripts | filtrer par normalisation dans le harness |
