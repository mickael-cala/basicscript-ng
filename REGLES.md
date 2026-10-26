# REGLES.md - Philosophie de Classification des Fichiers

Le test ultime pour classer un fichier : **"Si je supprime ce fichier, puis-je le reconstruire de manière déterministe à partir des autres ?"**

## 📁 Catégories de fichiers

### 1. SOURCES (Versionnés, modifiables)
- **Définition** : Fichiers qui ne peuvent pas être reconstruits. Ils contiennent la logique métier, la grammaire ou la configuration de build.
- **Exemples** : `src/**/*.c`, `src/**/*.h`, `syntax.def`, `scripts/*.py`, `test/**/*.bas`.
- **Règle** : C'est la seule catégorie que vous êtes autorisé à modifier directement.

### 2. PRODUITS (Ignorés par Git, régénérés)
- **Définition** : Fichiers générés automatiquement à partir des sources.
- **Exemples** : `dist/`, `obj/`, `*.exe`, `*.dll`, `*.o`, `docs/MANUEL.md` (généré, mais versionné pour la vérification), `*.log`.
- **Règle** : Ne jamais commiter de produits de compilation. Si un produit est corrompu, supprimez-le et reconstruisez.

### 3. RUSTINES / VENDOR (Versionnés, INTANGIBLES)
- **Définition** : Copies de bibliothèques tierces figées pour garantir la reproductibilité du build, indépendamment de l'environnement hôte.
- **Exemples** : `src/vendor/zlib/`, `src/vendor/curl/`.
- **Règle** : Ne jamais modifier ces fichiers à la main. Pour mettre à jour, utiliser un script de vendorisation dédié et commiter le changement global.

## 🔒 Invariants et Coûts

- Retirer un en-tête de `src/vendor/` fait échouer la compilation de 8 à 47 cibles selon l'architecture.
- Modifier `syntax.def` sans lancer `scripts/docs.py --write` fera échouer la CI à 100%.
- Un nom de produit écrit en dur dans le code source est un bug de conception (utiliser des macros de build ou la détection de cible).