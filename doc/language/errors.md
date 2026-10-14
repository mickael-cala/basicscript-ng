# Gestion des erreurs & codes d'erreur BasicScript / ScriptBasic 2.0

> Source normative : `old/errors.def` (génère `old/errcodes.c`, `old/include/error.bas` via `generrh.pl`) et `old/errcodes.c`.
> Voir aussi `old/error.html` pour le formatage historique des messages.

## Mécanismes du langage

### ON ERROR (style BASIC classique)
```basic
ON ERROR GOTO handler      ' saute à l'étiquette en cas d'erreur
...
handler:
    PRINT "Erreur n° "; ERR; " ligne "; ERL
    RESUME NEXT            ' ou RESUME, ou fin de programme
```
- `ERR` : code numérique de la dernière erreur (table ci-dessous).
- `ERL` : numéro de ligne où l'erreur s'est produite.
- À l'exécution, une erreur non prise en charge affiche le message (voir `old/error.html`) et termine le script avec un code de sortie non nul.

### TRY / CATCH / THROW (style moderne, ScriptBasic 2.x)
```basic
TRY
    x = 1/0
CATCH
    PRINT "récupéré : "; ERROR$
END TRY
```
*(syntaxe exacte à valider contre `old/syntax.c` — documentée ici provisoirement ; voir ROADMAP Phase 1)*

### Erreurs de compilation vs exécution
`old/errors.def` distingue `%CT` (compile-time, absents de `error.bas`) et `%RT` (run-time, exposés aux scripts sous forme de constantes `sbErrorXxx`).

## Constantes BASIC générées

Le fichier `old/include/error.bas` déclare `Global Const sbError<Name> = N` pour chaque erreur run-time. `sbErrorOK = 0` est garanti ; toutes les autres sont ≠ 0 ; `MODULE_ERROR_MEMORY_LOW` a la même valeur dans tous les modules.


## Module `COMMAND` (55 codes)

| ID | Signification |
|----|---------------|
| `NOARRAY` | *(texte suivant la ligne dans errors.def)* |
| `DIV` | *(texte suivant la ligne dans errors.def)* |
| `UNDEFOP` | *(texte suivant la ligne dans errors.def)* |
| `BAD_CALL` | *(texte suivant la ligne dans errors.def)* |
| `FEW_ARGS` | *(texte suivant la ligne dans errors.def)* |
| `ARGUMENT_TYPE` | *(texte suivant la ligne dans errors.def)* |
| `ARGUMENT_RANGE` | *(texte suivant la ligne dans errors.def)* |
| `FILE_READ` | *(texte suivant la ligne dans errors.def)* |
| `FILE_WRITE` | *(texte suivant la ligne dans errors.def)* |
| `FILE` | *(texte suivant la ligne dans errors.def)* |
| `CIRCULAR` | *(texte suivant la ligne dans errors.def)* |
| `MODULE_NOT_LOADED` | *(texte suivant la ligne dans errors.def)* |
| `PARTIAL_UNLOAD` | *(texte suivant la ligne dans errors.def)* |
| `MODULE_ACTIVE` | *(texte suivant la ligne dans errors.def)* |
| `MODULE_LOAD` | *(texte suivant la ligne dans errors.def)* |
| `MODULE_FUNCTION` | *(texte suivant la ligne dans errors.def)* |
| `MODULE_INITIALIZE` | *(texte suivant la ligne dans errors.def)* |
| `MODULE_VERSION` | *(texte suivant la ligne dans errors.def)* |
| `BAD_FILE_NUMBER` | *(texte suivant la ligne dans errors.def)* |
| `FILE_NUMBER_IS_USED` | *(texte suivant la ligne dans errors.def)* |
| `FILE_CANNOT_BE_OPENED` | *(texte suivant la ligne dans errors.def)* |
| `FILE_IS_NOT_OPENED` | *(texte suivant la ligne dans errors.def)* |
| `INVALID_LOCK` | *(texte suivant la ligne dans errors.def)* |
| `PRINT_FAIL` | *(texte suivant la ligne dans errors.def)* |
| `MKDIR_FAIL` | *(texte suivant la ligne dans errors.def)* |
| `DELETE_FAIL` | *(texte suivant la ligne dans errors.def)* |
| `NOTIMP` | *(texte suivant la ligne dans errors.def)* |
| `INVALID_JOKER` | *(texte suivant la ligne dans errors.def)* |
| `NO_RESUME` | *(texte suivant la ligne dans errors.def)* |
| `INV_DNAME` | InvalidDirectoryName |
| `INV_DO_OPTION` | InvalidOptionDirOpen |
| `DIR_NO_OPEN` | DirectoryNoOpen |
| `BAD_RECORD_LENGTH` | *(texte suivant la ligne dans errors.def)* |
| `CURDIR` | NoCurrentDirectory |
| `UNDEF_DIR` | ChDirUndef |
| `CHDIR` | ChDir |
| `RETURN_WITHOUT_GOSUB` | *(texte suivant la ligne dans errors.def)* |
| `INVALID_ARGUMENT_FOR_FUNCTION_ADDRESS` | *(texte suivant la ligne dans errors.def)* |
| `SETFILE_INVALID_ATTRIBUTE` | *(texte suivant la ligne dans errors.def)* |
| `CHOWN_INVALID_USER` | *(texte suivant la ligne dans errors.def)* |
| `CHOWN_NOT_SUPPORTED` | *(texte suivant la ligne dans errors.def)* |
| `CHOWN_SET_OWNER` | *(texte suivant la ligne dans errors.def)* |
| `INVALID_FILE_NAME` | *(texte suivant la ligne dans errors.def)* |
| `CREATIME_FAIL` | SetCreateTime |
| `MODTIME_FAIL` | SetModifyTime |
| `ACCTIM_FAIL` | SetAccessTime |
| `INVALID_TIME_FORMAT` | *(texte suivant la ligne dans errors.def)* |
| `INVALID_TIME` | *(texte suivant la ligne dans errors.def)* |
| `EXTENSION_SPECIFIC` | *(texte suivant la ligne dans errors.def)* |
| `SOCKET_FILE` | *(texte suivant la ligne dans errors.def)* |
| `INVALID_CODE` | *(texte suivant la ligne dans errors.def)* |
| `MANDARG` | *(texte suivant la ligne dans errors.def)* |
| `TIMEOUT` | *(texte suivant la ligne dans errors.def)* |
| `STAYS_IN_MEMORY` | *(texte suivant la ligne dans errors.def)* |
| `PREPROCESSOR_ABORT` | *(texte suivant la ligne dans errors.def)* |

## Module `BU` (4 codes)

| ID | Signification |
|----|---------------|
| `FILE_CORRUPT` | *(texte suivant la ligne dans errors.def)* |
| `NO_CODE` | *(texte suivant la ligne dans errors.def)* |
| `FAIL` | *(texte suivant la ligne dans errors.def)* |
| `ECODE_INPUT` | *(texte suivant la ligne dans errors.def)* |

## Module `READER` (9 codes)

| ID | Signification |
|----|---------------|
| `INCLUDE_SYNTAX` | *(texte suivant la ligne dans errors.def)* |
| `INCLUDE_FILE` | *(texte suivant la ligne dans errors.def)* |
| `TOOMANY_INCLUDE` | *(texte suivant la ligne dans errors.def)* |
| `PREPROC_UNDEF` | *(texte suivant la ligne dans errors.def)* |
| `PREPROC_LONG` | *(texte suivant la ligne dans errors.def)* |
| `PREPROC_NOTAVA` | *(texte suivant la ligne dans errors.def)* |
| `PREPROC_NOTVAL` | *(texte suivant la ligne dans errors.def)* |
| `FILE_OPEN` | *(texte suivant la ligne dans errors.def)* |
| `EMPTY_INPUT` | *(texte suivant la ligne dans errors.def)* |

## Module `PREPROC` (3 codes)

| ID | Signification |
|----|---------------|
| `FAIL` | *(texte suivant la ligne dans errors.def)* |
| `CONFIG_EXE` | *(texte suivant la ligne dans errors.def)* |
| `CONFIG_DIR` | *(texte suivant la ligne dans errors.def)* |

## Module `LEX` (5 codes)

| ID | Signification |
|----|---------------|
| `TOO_LONG_SYMBOL` | *(texte suivant la ligne dans errors.def)* |
| `STRING_NOT_TERMINATED` | *(texte suivant la ligne dans errors.def)* |
| `STRING_NEW_LINE` | *(texte suivant la ligne dans errors.def)* |
| `BAD_RADIX` | *(texte suivant la ligne dans errors.def)* |
| `INVALID_NUMBER` | *(texte suivant la ligne dans errors.def)* |

## Module `EXE` (22 codes)

| ID | Signification |
|----|---------------|
| `INVALID_PC` | *(texte suivant la ligne dans errors.def)* |
| `INVALID_PC1` | *(texte suivant la ligne dans errors.def)* |
| `INVALID_NODE` | *(texte suivant la ligne dans errors.def)* |
| `INVALID_OPCODE` | *(texte suivant la ligne dans errors.def)* |
| `NOT_IMPLEMENTED` | *(texte suivant la ligne dans errors.def)* |
| `INVALID_EXPRESSION_NODE` | *(texte suivant la ligne dans errors.def)* |
| `INVALID_EXPRESSION_NODE1` | *(texte suivant la ligne dans errors.def)* |
| `INTERNAL_DEFAULTTYPE` | *(texte suivant la ligne dans errors.def)* |
| `INTERNAL_UNDEF` | *(texte suivant la ligne dans errors.def)* |
| `INTERNAL_MEMREF` | *(texte suivant la ligne dans errors.def)* |
| `INTERNAL` | *(texte suivant la ligne dans errors.def)* |
| `NO_LOCAL` | *(texte suivant la ligne dans errors.def)* |
| `INVALID_LVALNODE0` | *(texte suivant la ligne dans errors.def)* |
| `INVALID_LVALNODE1` | *(texte suivant la ligne dans errors.def)* |
| `INVALID_LVALNODE2` | *(texte suivant la ligne dans errors.def)* |
| `INVALID_LVALNODE3` | *(texte suivant la ligne dans errors.def)* |
| `INVALID_LVALNODE4` | *(texte suivant la ligne dans errors.def)* |
| `INVALID_LVALNODE5` | *(texte suivant la ligne dans errors.def)* |
| `INVALID_LVALNODE6` | *(texte suivant la ligne dans errors.def)* |
| `USERFUN_UNDEFINED` | *(texte suivant la ligne dans errors.def)* |
| `TOO_DEEP_CALL` | *(texte suivant la ligne dans errors.def)* |
| `TOO_LONG_RUN` | *(texte suivant la ligne dans errors.def)* |

## Module `EX` (26 codes)

| ID | Signification |
|----|---------------|
| `INTERNAL` | *(texte suivant la ligne dans errors.def)* |
| `LABEL_DOUBLE_DEFINED` | *(texte suivant la ligne dans errors.def)* |
| `LABEL_NOT_DEFINED` | *(texte suivant la ligne dans errors.def)* |
| `FUNCTION_DOUBLE_DEFINED` | *(texte suivant la ligne dans errors.def)* |
| `FUNCTION_NOT_DEFINED` | *(texte suivant la ligne dans errors.def)* |
| `NO_OLD_NAMESPACE` | *(texte suivant la ligne dans errors.def)* |
| `UNFINISHED_MODULE` | *(texte suivant la ligne dans errors.def)* |
| `TOO_LONG_NAME_SPACE` | *(texte suivant la ligne dans errors.def)* |
| `TOO_LONG_VARIABLE` | *(texte suivant la ligne dans errors.def)* |
| `MISSING_PAREN` | *(texte suivant la ligne dans errors.def)* |
| `MISSING_OPAREN` | *(texte suivant la ligne dans errors.def)* |
| `MISSING_APAREN` | *(texte suivant la ligne dans errors.def)* |
| `MISSING_SAPAREN` | *(texte suivant la ligne dans errors.def)* |
| `FUNCTION_NEEDS_ARGUMENTS` | *(texte suivant la ligne dans errors.def)* |
| `TOO_FEW_ARGUMENTS` | *(texte suivant la ligne dans errors.def)* |
| `TOO_MANY_ARGUMENTS` | *(texte suivant la ligne dans errors.def)* |
| `BAD_NESTING` | *(texte suivant la ligne dans errors.def)* |
| `UNFINISHED_NESTING` | *(texte suivant la ligne dans errors.def)* |
| `SYNTAX` | *(texte suivant la ligne dans errors.def)* |
| `SYNTAX_FATAL` | *(texte suivant la ligne dans errors.def)* |
| `TOO_MANY_WAITING_LABEL` | *(texte suivant la ligne dans errors.def)* |
| `UNDEF_GLOBAL` | *(texte suivant la ligne dans errors.def)* |
| `UNDEF_PRAGMA` | *(texte suivant la ligne dans errors.def)* |
| `GREDEF` | *(texte suivant la ligne dans errors.def)* |
| `GLODEF` | *(texte suivant la ligne dans errors.def)* |
| `CONST_LVAL` | *(texte suivant la ligne dans errors.def)* |

## Module `MEM` (1 codes)

| ID | Signification |
|----|---------------|
| `INTERNAL001` | *(texte suivant la ligne dans errors.def)* |

## Module `MATCH` (4 codes)

| ID | Signification |
|----|---------------|
| `INDEX_OUT_OF_RANGE` | *(texte suivant la ligne dans errors.def)* |
| `BUFFER_SHORT` | *(texte suivant la ligne dans errors.def)* |
| `SYNTAX_ERROR` | *(texte suivant la ligne dans errors.def)* |
| `ARRAY_SHORT` | *(texte suivant la ligne dans errors.def)* |

## Module `CONFIG` (0 codes)

| ID | Signification |
|----|---------------|

## Module `SCRIBA` (2 codes)

| ID | Signification |
|----|---------------|
| `FAIL` | *(texte suivant la ligne dans errors.def)* |
| `BUFFER_SHORT` | *(texte suivant la ligne dans errors.def)* |

## À faire
- [ ] Vérifier la table complète contre le binaire `old/` compilé (numéros effectifs)
- [ ] Documenter le comportement par plateforme (Windows vs Unix) des codes système
- [ ] Lier chaque code à un test dans `test/regression/` quand il rejoint l'ERRATA
