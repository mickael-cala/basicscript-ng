# Extensions & modules disponibles (référence `old/extensions/`)

> Chaque module ajoute des commandes au langage (identifiants listés dans [functions.md](functions.md)). Le portage vers `extensions/` suit la ROADMAP Phase 4 ; la colonne **Décision** reflète l'état actuel des intentions (à confirmer par RFC).

Implémentations historiques : `old/extensions/<nom>/` ; tables de commandes générées par `old/moduletablemaker.pl` (`linkedmodules.def`).

| Module | Rôle | Décision portage |
|--------|------|------------------|
| `bdb` | Accès Berkeley DB | ❌ abandon probable |
| `cgi` | Interface CGI (sortie HTTP, variables d’environnement web) | 🔶 P4 |
| `cio` | Console I/O étendue (curseur, couleurs, saisie caractère par caractère — BINMI/BINMO/BINMF) | 🔶 P4 |
| `curl` | Transferts URL via libcurl | ✅ P4 |
| `curses` | Interface texte ncurses | ❌ arbitrer |
| `dbg` | Débogueur/pas-à-pas | ✅ P4 |
| `dyc` | Chaînes dynamiques avancées (dynstring) | ✅ P3 (intégré?) |
| `gd` | Création d’images (PNG/JPEG/GIF via libgd) | ❌ arbitrer |
| `hash` | Fonctions de hachage (MD5, SHA…) | ✅ P4 |
| `mt` | Multithreading (THREADCREATE, LOCK…) | 🔶 P4 (RFC) |
| `mysql` | Connecteur MySQL | ❌ abandon probable |
| `nt` | Services Windows NT spécifiques | ❌ legacy |
| `odbc` | Connecteur ODBC générique | ❌ abandon probable |
| `psql` | Connecteur PostgreSQL | ❌ abandon probable |
| `re` | Expressions régulières PCRE | ✅ P4 |
| `t` | Module de test/exemple | — |
| `trial` | Fonctions d’évaluation/démonstration | — |
| `ux` | Utilitaires Unix (processus, signaux…) | 🔶 P4 |
| `xml` | Analyseur XML | 🔶 P4 |
| `zlib` | Compression zlib/gzip | ✅ P4 |

## Écrire une extension (API cible — provisoire)

1. Créer `extensions/<nom>/<nom>.zig` (ou `.c` en phase C23) exportant la table de commandes.
2. Déclarer les nouveaux identifiants dans le registre syntaxique (`src/parser` une fois porté).
3. Fournir un script de démo dans `examples/<nom>.bas` et des tests dans `test/language/ext_<nom>_*.bas`.
4. Documenter chaque commande dans `doc/language/functions.md` (section du module).

## À faire
- [ ] Extraire la liste exacte des commandes par module depuis les `.def` de `old/extensions/`
- [ ] Trancher les ❓/🔶 par RFC avant la Phase 4
