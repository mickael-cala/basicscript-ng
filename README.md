# ScriptBasic-ng (Next-Gen)

Une refonte moderne, portable et robuste de l'interpréteur ScriptBasic, combinant la rigueur de l'ingénierie logicielle et la puissance de la toolchain Zig (C23).

## 🚀 Fonctionnalités Killer
- **Portabilité extrême** : Compilation native Windows (x86/x64), Linux (x64/ARM64) et macOS (x64/ARM64) via `zig cc`.
- **Exécutables autonomes** : Option `--standalone` pour embarquer le bytecode dans un exécutable unique, sans dépendance.
- **Intégration facile** : Option `--libscriba` pour intégrer le moteur dans d'autres projets C/C++/Rust/Go.
- **Documentation comme Code** : `MANUEL.md` est généré et vérifié automatiquement depuis `syntax.def`.
- **Zéro dépendance magique** : Fichiers `.h` plats et maintenus manuellement, standard C23 strict.

## 🛠️ Démarrage rapide

### Prérequis
- Python 3.8+
- Zig 0.11+ (dans le PATH ou variable d'environnement `ZIG`)

### Build
```bash
# Compiler pour la cible hôte par défaut
python scripts/build.py

# Compiler pour une cible spécifique (ex: Windows 64-bit)
python scripts/build.py --target windows-x64

# Générer un exécutable autonome
python scripts/build.py --target windows-x64 --standalone mon_script.bas
```

### Tests
```bash
# Lancer la campagne de tests de contrat
python scripts/test_runner.py
```

### Documentation
```bash
# Générer la documentation depuis syntax.def
python scripts/docs.py --write

# Vérifier que la documentation est à jour (échoue si divergence)
python scripts/docs.py --check
```

## 📚 Documentation
- [AGENTS.md](AGENTS.md) : Contrat d'ingénierie et règles non négociables.
- [REGLES.md](REGLES.md) : Philosophie de classification des fichiers.
- [docs/EVOLUTIONS.md](docs/EVOLUTIONS.md) : Feuille de route et historique des décisions.