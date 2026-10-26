# embed — les exemples consommateurs de libscriba

Trois programmes C qui prouvent l'embarquement, compilés par
`python build.py --target <cible> --libscriba` contre
`dist/<cible>/libscriba.a`, exécutés par
`python tests/run.py --target <cible> --embed` :

- `lancer.c` — charger `bonus.bas` et l'exécuter ;
- `appeler.c` — appeler `Double(21)` depuis le C et lire le retour (42) ;
- `variable.c` — lire la variable BASIC `total` après exécution.

`bonus.bas` est leur programme BASIC commun. Sous UNIX, chaque hôte
définit `char **_environ;` (voir libscriba.h). Les exécutables produits
vont dans `dist/<cible>/embed/`.
