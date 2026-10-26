# AGENTS.md - Contrat d'Ingénierie

Ce dépôt est vérifié par des programmes, pas par la lecture humaine. Toute modification doit respecter ces règles sous peine d'échec de la CI.

## 🚨 Règles non négociables

1. **Ne jamais modifier un produit** : Les fichiers dans `dist/`, `obj/`, `bin/` sont générés. Si un binaire est en cause, modifier la source et reconstruire.
2. **Documentation synchronisée** : Toute modification de `syntax.def` impose une régénération de la documentation (`python scripts/docs.py --write`). La CI échouera si `docs.py --check` détecte une divergence.
3. **Headers plats** : Les fichiers `.h` sont des sources normales, maintenus à la main. Aucun script de génération magique de headers n'est autorisé.
4. **Inventaire à jour** : Tout nouveau fichier source impose une mise à jour de l'inventaire du projet.

## ⚠️ Pièges historiques documentés

- **`dyc` (Dynamic C)** : Exclusivement 32 bits pour des raisons d'ABI. Toute tentative de compilation 64 bits de ce module est interdite.
- **`waitpid` / `execute`** : Exigent des variables lvalue pour le PID et le status, pas des constantes.
- **Modules vendorisés** : Le contenu de `src/vendor/` (zlib, curl headers) est INTANGIBLE. Ne jamais modifier ces fichiers directement ; mettre à jour la version vendorisée via un script dédié.
- **Opérateurs étendus** : Les 256 opérateurs fantaisistes (`+^`, `!<`, etc.) ont été supprimés. Seul le tiret de fichier `#` reste valide.

## ✅ Définition d'un commit propre

Un commit doit être atomique et répondre à une seule préoccupation :
- `feat: ajout du support ARM64 pour le module hash`
- `fix: correction de la fuite mémoire dans isdefined imbriqué`
- `docs: mise à jour de MANUEL.md après ajout de UNLOCK`