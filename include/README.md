# En-têtes C — passe de migration

Contenu **généré** par la toolchain legacy (`old/headerer.pl`, `old/syntaxer.pl`, `old/generrh.pl`)
à partir des sources `old/*.c` lors de la passe initiale du portage. Rôle :

- servir de **référence d'API** pour réécrire le noyau en C23 ;
- alimenter les tests de fumée contre le moteur de référence ;
- **ne pas éditer à la main** — régénérer via `tools/gen/regen.sh`.

## Répartition

| Emplacement | Contenu | Destin à la v1.0 |
|---|---|---|
| `include/basicscript/` | 41 en-têtes du noyau (dont `errcodes.h` généré depuis `errors.def`, `syntax.h` depuis `syntax.def`) | API publique réécrite module par module |
| `src/private/` | 4 en-têtes internes (extensions cgi/dbg/rpool, variations) | resteront privés ou disparaîtront |

Après la Phase 2, ces fichiers seront remplacés par des en-têtes C23 écrits à la main et
les outils temporaires de `tools/gen/` seront supprimés (voir ROADMAP Phase 1).
