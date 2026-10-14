# ERRATA — écarts connus entre le code historique et sa documentation

> **Ce qui s'est passé, côté bugs.** Cet inventaire recense les comportements inattendus, bogues historiques et divergences doc/code identifiés dans ScriptBasic 2.0 (`old/`). Chaque entrée doit servir de **cas de test** dans `test/regression/` lors du portage, et d'avertissement aux utilisateurs migrateurs.

## Format des entrées

```
### E-NNN — Titre court
- **Composant :** fichier/module concerné (dans old/)
- **Gravité :** critique | majeure | mineure | cosmétique
- **Symptôme :** ce qui se passe
- **Cause racine :** pourquoi (si connue)
- **Contournement :** comment l'éviter avec l'ancien moteur
- **Statut portage :** à reproduire en test | corrigé dans src/ (phase N) | non corrigé (compat)
```

---

## Registre

_(À remplir au fil de l'audit du code `old/`. Pistes initiales issues de la lecture des sources :)_

### E-001 — Ambiguïté de nommage des sources dupliquées
- **Composant :** `old/match.c` vs `old/matchc.c`, `old/reader.c` (et `old/setup.log`, `old/Makefile.old` présents en production)
- **Gravité :** mineure (hygiène)
- **Symptôme :** fichiers quasi homonymes et artefacts de build versionnés ; difficile de savoir quel fichier est réellement compilé selon les plateformes.
- **Cause racine :** accumulation de 20 ans de forks de build (Jam, Jim, Makefiles régionaux).
- **Contournement :** se fier uniquement à `old/Makefile` (gcc) comme liste de sources canonique.
- **Statut portage :** inventaire à faire en Phase 1 ; ne pas porter les doublons.

### E-002 — Le README annonce une réécriture Zig/C23 alors que le code est en C89/C99
- **Composant :** `README.md` vs intégralité de `old/`
- **Gravité :** cosmétique (documentaire)
- **Symptôme :** aucun fichier `.zig` ni usage C23 dans le dépôt malgré le titre « Rewriting for C23 and Zig 0.17 ».
- **Cause racine :** intention de fork posée avant le travail de portage.
- **Contournement :** consulter la ROADMAP.md pour l'état réel.
- **Statut portage :** clarifié par la présente organisation du dépôt.

### E-003 — Licence : README dit LGPL v2.1, vérifier la cohérence des en-têtes de fichiers
- **Composant :** `old/LICENSE.txt`, en-têtes de tous les `.c/.h`
- **Gravité :** majeure (juridique)
- **Symptôme :** à auditer : certains fichiers portent des mentions GPL/LGPL ambiguës ou des clauses « linking exception » propres à ScriptBasic.
- **Cause racine :** évolutions de licence historiques du projet amont.
- **Contournement :** lire `old/LICENSE.txt` en entier avant toute redistribution.
- **Statut portage :** audit licence obligatoire avant Phase 6 (release).

### E-004 — Codes d'erreur implicites absents du catalogue `errors.def`

- **Constat :** le générateur historique Jamal injecte automatiquement des constantes
  qui n'existent pas dans `old/errors.def`, et 9 codes sont utilisés par le code C sans
  entrée de message dans le catalogue. Résultat : le `.def` (source « officielle ») est
  incomplet par rapport au binaire réel (`old/errcodes.h`).
- **Détail :**
  - *Paires injectées* : pour chacun des 10 modules (`COMMAND, BU, READER, PREPROC, LEX,
    EXE, EX, MEM, MATCH, SCRIBA`), Jamal génère `<MOD>_ERROR_SUCCESS = 0` (garanti zéro,
    tous les autres codes ≠ 0) et `<MOD>_ERROR_MEMORY_LOW = 1` (valeur identique dans
    tous les modules). Garantie documentée en commentaires de `old/errors.def` (l. 33-35).
  - *Sentinelles* : `MAX_ERROR_CODE = 133` (taille du tableau de messages) et bornes
    hautes `LAST_<MODULE>` (ex. `LAST_COMMAND = 56`).
  - *9 codes implicites COMMAND* (utilisés via `ERROR()`/`return` mais sans message
    catalogue) : `INV_DNAME`(31), `INV_DO_OPTION`(32), `DIR_NO_OPEN`(33), `CURDIR`(35),
    `UNDEF_DIR`(36), `CHDIR`(37), `CREATIME_FAIL`(45), `MODTIME_FAIL`(46),
    `ACCTIM_FAIL`(47). Sources d'usage : `old/commands/file.c` (dossier),
    `old/hookers.c` + `old/filesys.c` (horodatage fichiers). Leurs messages dans
    `doc/spec/errors.md` sont **reconstitués depuis l'usage**, pas extraits du `.def`.
- **Impact portage :** la table d'erreurs v2 doit être régénérée depuis une spec unique
  incluant ces artefacts (sinon numéros incompatibles avec `ERR()` / include BASIC legacy).
- **Traitement :** `tools/gen/extract_errors.py` injecte désormais les paires
  SUCCESS/MEMORY_LOW, les sentinelles et les 9 codes ; `doc/spec/errors.md` (171 lignes)
  porte les colonnes N°/Alias/Type avec marquage *(implicite)* et type `SYN`.
- **Statut portage :** validé en partie — numéros et aliases BASIC des 9 codes vérifiés
  contre `old/include/error.bas` (constantes `sbError*`, l. 31→47 : aucune lacune, les
  trous apparents 34/38-44 correspondent en fait à des erreurs définies dans le `.def`).
  Reste à confirmer la plage complète 0→133 de `error.bas` vs table C, puis figer la
  numérotation v2 dans `doc/spec/`.

<!-- Les entrées suivantes seront ajoutées après audit systématique :
     gestion mémoire (myalloc), fuites connues du thread model, limites de
     longueur de lignes/identifiants, différences de comportement PRINT sous
     Windows/Unix, modules cassés (wx, bdb), etc. -->

## Procédure d'ajout

1. Reproduire le problème avec le binaire `old/` compilé.
2. Ouvrir une issue, référence `E-NNN`.
3. Ajouter l'entrée ici + un test dans `test/regression/E-NNN.bas`.
