# EVOLUTIONS.md - Feuille de route et Traçabilité

Ce document trace les décisions architecturales majeures de la fusion entre `ScriptBasic-master-ng` et `scriba-zig`.

## 🚀 Version 1.0.0 (Fusion Initiale)

### 1. Adoption de Zig et C23
- **Décision** : Remplacer TinyCC/Make par `zig cc` comme compilateur C unique.
- **Raison** : Portabilité extrême (cross-compilation native vers 6 cibles sans toolchain externe) et support natif du standard C23 (`-std=c2x`).
- **Impact** : Suppression des scripts de build complexes, unification via `scripts/build.py`.

### 2. Grammaire Épurée (Héritage de -ng)
- **Supprimé** : 256 opérateurs étendus fantaisistes, fonctions de conversion binaire legacy (`cv*`, `mk*`), `fork`, `pause`.
- **Ajouté** : `EXITFOR`, `EXITWHILE`, `EXITDO`, `UNLOCK`, `UNLOCK REGION`.
- **Normalisé** : Suffixe `$` obligatoire pour toutes les fonctions retournant des chaînes (`UCASESTR`, `JOINSTR`, `FORMATSTR`, etc.).

### 3. Fonctionnalités Killer (Héritage de scriba-zig)
- **`--standalone`** : Capacité d'embarquer le bytecode dans un exécutable unique (en cours d'implémentation dans `build.py`).
- **`--libscriba`** : Génération d'une archive statique pour intégration tierce.
- **Headers plats** : Interdiction des scripts Perl de génération de `.h`. Les en-têtes sont des sources C normales, vérifiées par le compilateur.

### 4. Documentation comme Code
- **Mécanisme** : `scripts/docs.py` parse `syntax.def` et génère/vérifie `docs/MANUEL.md`.
- **Règle** : La CI échoue si la documentation n'est pas synchronisée avec la grammaire.

## 📋 TODO pour la V1.1.0
- [ ] Implémenter la logique complète de `--standalone` (injection de bytecode en section `.rodata`).
- [ ] Vendoriser les sources de `zlib` et les en-têtes de `curl` dans `src/vendor/`.
- [ ] Porter les modules spécifiques : `odbc`, `zlib`, `curl` avec gestion conditionnelle des cibles.
- [ ] Écrire `scripts/test_runner.py` pour l'exécution et la comparaison des tests de contrat.