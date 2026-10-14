# Référence des mots-clés BasicScript / ScriptBasic 2.0

> Généré à partir de `old/syntax.def` (534 identifiants réservés au total).
> Source normative : `old/syntax.def` + `old/syntax.c` ; la liste exhaustive des identifiants réservés (500 fonctions/commandes) est dans [functions.md](functions.md). Document en cours d'enrichissement — chaque entrée doit être vérifiée contre le comportement réel de l'ancien moteur avant la v1.

## Rôle du fichier

Ce document liste les **mots-clés de contrôle et de déclaration** du langage. Les fonctions intégrées sont dans [functions.md](functions.md), les commandes de modules dans [modules.md](modules.md), les erreurs dans [errors.md](errors.md).

## Mots-clés de structure & contrôle

| Mot-clé | Catégorie | Description | Réf. impl. (`old/`) |
|---------|-----------|-------------|---------------------|
| `BYVAL` | Sous-programme | *(à compléter)* | `syntax.c / lexer.c` |
| `CALL` | Flux | *(à compléter)* | `commands/external.c` |
| `CBYVAL` | Sous-programme | *(à compléter)* | `syntax.c / lexer.c` |
| `CONST` | Données | *(à compléter)* | `syntax.c / lexer.c` |
| `CUNDEF` | Données | *(à compléter)* | `syntax.c / lexer.c` |
| `DO` | Boucle | *(à compléter)* | `syntax.c / lexer.c` |
| `DOUNTIL` | Boucle | *(à compléter)* | `syntax.c / lexer.c` |
| `DOWHILE` | Boucle | *(à compléter)* | `syntax.c / lexer.c` |
| `ELSE` | Condition | *(à compléter)* | `syntax.c / lexer.c` |
| `ELSIF` | Condition | *(à compléter)* | `syntax.c / lexer.c` |
| `END` | Données | *(à compléter)* | `syntax.c / lexer.c` |
| `ENDFUNC` | Sous-programme | *(à compléter)* | `syntax.c / lexer.c` |
| `ENDIF` | Condition | *(à compléter)* | `syntax.c / lexer.c` |
| `ENDMODULE` | Sous-programme | *(à compléter)* | `syntax.c / lexer.c` |
| `ENDSUB` | Sous-programme | *(à compléter)* | `syntax.c / lexer.c` |
| `EXITFUNC` | Flux | *(à compléter)* | `syntax.c / lexer.c` |
| `EXITSUB` | Flux | *(à compléter)* | `syntax.c / lexer.c` |
| `FOR` | Boucle | *(à compléter)* | `syntax.c / lexer.c` |
| `FUNCTION` | Sous-programme | *(à compléter)* | `commands/function.c` |
| `GLOBAL` | Sous-programme | *(à compléter)* | `syntax.c / lexer.c` |
| `GOSUB` | Flux | *(à compléter)* | `syntax.c / lexer.c` |
| `GOTO` | Flux | *(à compléter)* | `commands/goto.c` |
| `IF` | Condition | *(à compléter)* | `commands/if.c` |
| `LET` | Données | *(à compléter)* | `commands/let.c` |
| `LOCAL` | Sous-programme | *(à compléter)* | `syntax.c / lexer.c` |
| `LOOP` | Boucle | *(à compléter)* | `syntax.c / lexer.c` |
| `MODULE` | Sous-programme | *(à compléter)* | `syntax.c / lexer.c` |
| `NEXT` | Boucle | *(à compléter)* | `syntax.c / lexer.c` |
| `REPEAT` | Boucle | *(à compléter)* | `syntax.c / lexer.c` |
| `SUB` | Sous-programme | *(à compléter)* | `syntax.c / lexer.c` |
| `UNDEF` | Données | *(à compléter)* | `syntax.c / lexer.c` |
| `UNTIL` | Boucle | *(à compléter)* | `syntax.c / lexer.c` |
| `WEND` | Boucle | *(à compléter)* | `syntax.c / lexer.c` |
| `WHILE` | Boucle | *(à compléter)* | `commands/while.c` |

## Exemple complet

```basic
DIM a(10)
FOR i = 1 TO 10
    LET a(i) = i * 2
NEXT
IF a(5) > 8 THEN
    PRINT "OK"
ELSE
    PRINT "KO"
ENDIF
```

## À faire pour cet inventaire
- [ ] Rédiger une description précise par mot-clé (réf. `old/texi/`)
- [ ] Noter les variantes avec suffixe `$`, `%` et les alias historiques
