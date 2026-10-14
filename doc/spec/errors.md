<!-- Généré par tools/gen/extract_errors.py depuis old/errors.def — ne pas éditer à la main.
     Catalogue des codes d'erreur hérités ; décisions de conservation dans ROADMAP Phase 1. -->

# Codes d'erreur BasicScript (héritage ScriptBasic 2.0)

**122 erreurs** réparties en 10 modules. `CT` = compilation, `RT` = exécution.

## COMMAND

| ID | #define C | Type | Message |
|---|---|---|---|
| `NOARRAY` | `COMMAND_ERROR_NOARRAY` |  | en/Function can not return a whole array |
| `DIV` | `COMMAND_ERROR_DIV` |  | en/Division by zero or other calculation error |
| `UNDEFOP` | `COMMAND_ERROR_UNDEFOP` |  | en/Argument to operator is undefined |
| `BAD_CALL` | `COMMAND_ERROR_BAD_CALL` |  | en/The command or sub was called the wrong way |
| `FEW_ARGS` | `COMMAND_ERROR_FEW_ARGS` |  | en/There are not enough arguments of the module function. |
| `ARGUMENT_TYPE` | `COMMAND_ERROR_ARGUMENT_TYPE` |  | en/The argument passed to a module function is not the needed type. |
| `ARGUMENT_RANGE` | `COMMAND_ERROR_ARGUMENT_RANGE` |  | en/The argument passed to a module function is out of the accepted range. |
| `FILE_READ` | `COMMAND_ERROR_FILE_READ` |  | en/The module experiences difficulties reading the file |
| `FILE_WRITE` | `COMMAND_ERROR_FILE_WRITE` |  | en/The module experiences difficulties writing the file. |
| `FILE` | `COMMAND_ERROR_FILE` |  | en/The module experiences handling the file. |
| `CIRCULAR` | `COMMAND_ERROR_CIRCULAR` |  | en/There is a circular reference in memory. |
| `MODULE_NOT_LOADED` | `COMMAND_ERROR_MODULE_NOT_LOADED` |  | en/The module can not be unloaded, because it was not loaded. |
| `PARTIAL_UNLOAD` | `COMMAND_ERROR_PARTIAL_UNLOAD` |  | en/Some modules were active and could not be unloaded. |
| `MODULE_ACTIVE` | `COMMAND_ERROR_MODULE_ACTIVE` |  | en/The module can not be unloaded, because it is currently active. |
| `MODULE_LOAD` | `COMMAND_ERROR_MODULE_LOAD` |  | en/The requested module can not be loaded. |
| `MODULE_FUNCTION` | `COMMAND_ERROR_MODULE_FUNCTION` |  | en/The requested function does not exist in the module. |
| `MODULE_INITIALIZE` | `COMMAND_ERROR_MODULE_INITIALIZE` |  | en/The module did not initialize correctly |
| `MODULE_VERSION` | `COMMAND_ERROR_MODULE_VERSION` |  | en/The module was developed for a different version of ScriptBasic. |
| `BAD_FILE_NUMBER` | `COMMAND_ERROR_BAD_FILE_NUMBER` |  | en/File number is out of range, it should be between 1 and 512 |
| `FILE_NUMBER_IS_USED` | `COMMAND_ERROR_FILE_NUMBER_IS_USED` |  | en/The file number is already used. |
| `FILE_CANNOT_BE_OPENED` | `COMMAND_ERROR_FILE_CANNOT_BE_OPENED` |  | en/The file can not be opened. |
| `FILE_IS_NOT_OPENED` | `COMMAND_ERROR_FILE_IS_NOT_OPENED` |  | en/The file is not opened. |
| `INVALID_LOCK` | `COMMAND_ERROR_INVALID_LOCK` |  | en/The lock type is invalid. |
| `PRINT_FAIL` | `COMMAND_ERROR_PRINT_FAIL` |  | en/The print command failed. The file may be locked by another process. |
| `MKDIR_FAIL` | `COMMAND_ERROR_MKDIR_FAIL` |  | en/Directory can not be created. |
| `DELETE_FAIL` | `COMMAND_ERROR_DELETE_FAIL` |  | en/The directory or file could not be deleted. |
| `NOTIMP` | `COMMAND_ERROR_NOTIMP` |  | en/Command is not implemented and no currently loaded extension module defined behaviour for it |
| `INVALID_JOKER` | `COMMAND_ERROR_INVALID_JOKER` |  | en/The character can not be a joker or wild card character. |
| `NO_RESUME` | `COMMAND_ERROR_NO_RESUME` |  | en/The code tried to execute a resume while not being in error correction code. |
| `BAD_RECORD_LENGTH` | `COMMAND_ERROR_BAD_RECORD_LENGTH` |  | en/The record length is invalid in the open statements (undefined, zero or negative) |
| `RETURN_WITHOUT_GOSUB` | `COMMAND_ERROR_RETURN_WITHOUT_GOSUB` |  | en/The command RETURN can not be executed, because there is no where to return. |
| `INVALID_ARGUMENT_FOR_FUNCTION_ADDRESS` | `COMMAND_ERROR_INVALID_ARGUMENT_FOR_FUNCTION_ADDRESS` |  | en/The argument for the function address is invalid. |
| `SETFILE_INVALID_ATTRIBUTE` | `COMMAND_ERROR_SETFILE_INVALID_ATTRIBUTE` |  | en/The attribute value or symbol is invalid in the set file command. |
| `CHOWN_INVALID_USER` | `COMMAND_ERROR_CHOWN_INVALID_USER` |  | en/The user does not exist. |
| `CHOWN_NOT_SUPPORTED` | `COMMAND_ERROR_CHOWN_NOT_SUPPORTED` |  | en/The chown command is not supported on Win95 and Win98 |
| `CHOWN_SET_OWNER` | `COMMAND_ERROR_CHOWN_SET_OWNER` |  | en/Can not change owner. |
| `INVALID_FILE_NAME` | `COMMAND_ERROR_INVALID_FILE_NAME` |  | en/The file name is invalid. |
| `INVALID_TIME_FORMAT` | `COMMAND_ERROR_INVALID_TIME_FORMAT` |  | en/The specified time format is invalid |
| `INVALID_TIME` | `COMMAND_ERROR_INVALID_TIME` |  | en/The time is not valid, cannot be earlier than January 1, 1970. 00:00 |
| `EXTENSION_SPECIFIC` | `COMMAND_ERROR_EXTENSION_SPECIFIC` |  | en/Extension specific error: %s |
| `SOCKET_FILE` | `COMMAND_ERROR_SOCKET_FILE` |  | en/The operation can be done on files only and not on sockets. |
| `INVALID_CODE` | `COMMAND_ERROR_INVALID_CODE` |  | en/The embedding application tried to start the code at an invalid location |
| `MANDARG` | `COMMAND_ERROR_MANDARG` |  | en/Mandatory argument is missing |
| `TIMEOUT` | `COMMAND_ERROR_TIMEOUT` |  | en/Subprocess did not finish within time limits |
| `STAYS_IN_MEMORY` | `COMMAND_ERROR_STAYS_IN_MEMORY` |  | en/The module can not be unloaded |
| `PREPROCESSOR_ABORT` | `COMMAND_ERROR_PREPROCESSOR_ABORT` |  | en/The preprocessor said to abort program compilation or execution. |

## BU

| ID | #define C | Type | Message |
|---|---|---|---|
| `FILE_CORRUPT` | `BU_ERROR_FILE_CORRUPT` |  | en/The file is either corrupt or was written with a different version of sb. |
| `NO_CODE` | `BU_ERROR_NO_CODE` |  | en/The compiled program contains no executable code. |
| `FAIL` | `BU_ERROR_FAIL` |  | en/Code file can not be saved. |
| `ECODE_INPUT` | `BU_ERROR_ECODE_INPUT` |  | en/The interpreter file can not be read |

## READER

| ID | #define C | Type | Message |
|---|---|---|---|
| `INCLUDE_SYNTAX` | `READER_ERROR_INCLUDE_SYNTAX` |  | en/Bad syntax in include statement. |
| `INCLUDE_FILE` | `READER_ERROR_INCLUDE_FILE` |  | en/Include file is not found |
| `TOOMANY_INCLUDE` | `READER_ERROR_TOOMANY_INCLUDE` |  | en/Too many includes, probably recursive include |
| `PREPROC_UNDEF` | `READER_ERROR_PREPROC_UNDEF` |  | en/Preprocessor \"%s\" is not defined. |
| `PREPROC_LONG` | `READER_ERROR_PREPROC_LONG` |  | en/Preprocessor name is too long. |
| `PREPROC_NOTAVA` | `READER_ERROR_PREPROC_NOTAVA` |  | en/Preprocessor \"%s\" is not available. |
| `PREPROC_NOTVAL` | `READER_ERROR_PREPROC_NOTVAL` |  | en/Preprocessor \"%s\" is invalid. |
| `FILE_OPEN` | `READER_ERROR_FILE_OPEN` |  | en/The file can not be read. |
| `EMPTY_INPUT` | `READER_ERROR_EMPTY_INPUT` |  | en/The file is empty or is not readable. |

## PREPROC

| ID | #define C | Type | Message |
|---|---|---|---|
| `FAIL` | `PREPROC_ERROR_FAIL` |  | en/The external preprocessor failed. |
| `CONFIG_EXE` | `PREPROC_ERROR_CONFIG_EXE` |  | en/The preprocessor executable is not configured. |
| `CONFIG_DIR` | `PREPROC_ERROR_CONFIG_DIR` |  | en/The preprocessor temporary directory is not configured. |

## LEX

| ID | #define C | Type | Message |
|---|---|---|---|
| `TOO_LONG_SYMBOL` | `LEX_ERROR_TOO_LONG_SYMBOL` |  | en/Symbol is too long |
| `STRING_NOT_TERMINATED` | `LEX_ERROR_STRING_NOT_TERMINATED` |  | en/string is not terminated anywhere before end of file |
| `STRING_NEW_LINE` | `LEX_ERROR_STRING_NEW_LINE` |  | en/string is not terminated anywhere before end of line |
| `BAD_RADIX` | `LEX_ERROR_BAD_RADIX` |  | en/the number contains invalid radix. BASE#NNN numbers areavailable for 2<=BASE<=36 only. |
| `INVALID_NUMBER` | `LEX_ERROR_INVALID_NUMBER` |  | en/the BASE#NNN formatted number digit is out of range |

## EXE

| ID | #define C | Type | Message |
|---|---|---|---|
| `INVALID_PC` | `EXE_ERROR_INVALID_PC` |  | en/Program counter points out of the code. The executed code is corrupt. |
| `INVALID_PC1` | `EXE_ERROR_INVALID_PC1` |  | en/Internal error or the cached code is corrupt. |
| `INVALID_NODE` | `EXE_ERROR_INVALID_NODE` |  | en/Internal error or the cached code is corrupt. |
| `INVALID_OPCODE` | `EXE_ERROR_INVALID_OPCODE` |  | en/invalid op code is in the code table |
| `NOT_IMPLEMENTED` | `EXE_ERROR_NOT_IMPLEMENTED` |  | en/The code exists, but it is not implemented. |
| `INVALID_EXPRESSION_NODE` | `EXE_ERROR_INVALID_EXPRESSION_NODE` |  | en/Internal error or the cached code is corrupt. |
| `INVALID_EXPRESSION_NODE1` | `EXE_ERROR_INVALID_EXPRESSION_NODE1` |  | en/Internal error or the cached code is corrupt. |
| `INTERNAL_DEFAULTTYPE` | `EXE_ERROR_INTERNAL_DEFAULTTYPE` |  | en/Internal error when releasing a variable it has no correct type |
| `INTERNAL_UNDEF` | `EXE_ERROR_INTERNAL_UNDEF` |  | en/Internal error when releasing undefined variable, nonsense |
| `INTERNAL_MEMREF` | `EXE_ERROR_INTERNAL_MEMREF` |  | en/Internal error when releasing variable reference found |
| `INTERNAL` | `EXE_ERROR_INTERNAL` |  | en/any non classified internal error |
| `NO_LOCAL` | `EXE_ERROR_NO_LOCAL` |  | en/local variable referenced in global context or in a local context having no local variables |
| `INVALID_LVALNODE0` | `EXE_ERROR_INVALID_LVALNODE0` |  | en/Internal error or the cached code is corrupt. |
| `INVALID_LVALNODE1` | `EXE_ERROR_INVALID_LVALNODE1` |  | en/Internal error or the cached code is corrupt. |
| `INVALID_LVALNODE2` | `EXE_ERROR_INVALID_LVALNODE2` |  | en/Internal error or the cached code is corrupt. |
| `INVALID_LVALNODE3` | `EXE_ERROR_INVALID_LVALNODE3` |  | en/Internal error or the cached code is corrupt. |
| `INVALID_LVALNODE4` | `EXE_ERROR_INVALID_LVALNODE4` |  | en/Internal error or the cached code is corrupt. |
| `INVALID_LVALNODE5` | `EXE_ERROR_INVALID_LVALNODE5` |  | en/Internal error or the cached code is corrupt. |
| `INVALID_LVALNODE6` | `EXE_ERROR_INVALID_LVALNODE6` |  | en/Internal error or the cached code is corrupt. |
| `USERFUN_UNDEFINED` | `EXE_ERROR_USERFUN_UNDEFINED` |  | en/the user function is undefined |
| `TOO_DEEP_CALL` | `EXE_ERROR_TOO_DEEP_CALL` |  | en/Recursive function calls exceeded configuration limit. |
| `TOO_LONG_RUN` | `EXE_ERROR_TOO_LONG_RUN` |  | en/Program is running too long, probably infinite loop. |

## EX

| ID | #define C | Type | Message |
|---|---|---|---|
| `INTERNAL` | `EX_ERROR_INTERNAL` |  | en/Internal error in the syntax analyzer |
| `LABEL_DOUBLE_DEFINED` | `EX_ERROR_LABEL_DOUBLE_DEFINED` |  | en/A line label was defined more than once. |
| `LABEL_NOT_DEFINED` | `EX_ERROR_LABEL_NOT_DEFINED` |  | en/A label \"%s\" was not defined during syntax analysis. |
| `FUNCTION_DOUBLE_DEFINED` | `EX_ERROR_FUNCTION_DOUBLE_DEFINED` |  | en/The user function was already defined. |
| `FUNCTION_NOT_DEFINED` | `EX_ERROR_FUNCTION_NOT_DEFINED` |  | en/The user function \"%s\" is used but is not defined. |
| `NO_OLD_NAMESPACE` | `EX_ERROR_NO_OLD_NAMESPACE` |  | en/There was no module started when an end module statement was found. |
| `UNFINISHED_MODULE` | `EX_ERROR_UNFINISHED_MODULE` |  | en/A module was not closed at end of file. |
| `TOO_LONG_NAME_SPACE` | `EX_ERROR_TOO_LONG_NAME_SPACE` |  | en/the name space grew too long |
| `TOO_LONG_VARIABLE` | `EX_ERROR_TOO_LONG_VARIABLE` |  | en/variable and namespace does not fit into the buffer |
| `MISSING_PAREN` | `EX_ERROR_MISSING_PAREN` |  | en/ closing ) is missing in expression |
| `MISSING_OPAREN` | `EX_ERROR_MISSING_OPAREN` |  | en/ opening ( is missing in sub call |
| `MISSING_APAREN` | `EX_ERROR_MISSING_APAREN` |  | en/ closing ] is missing following array indeces |
| `MISSING_SAPAREN` | `EX_ERROR_MISSING_SAPAREN` |  | en/ closing } is missing following associative array indices |
| `FUNCTION_NEEDS_ARGUMENTS` | `EX_ERROR_FUNCTION_NEEDS_ARGUMENTS` |  | en/ built in function needs arguments |
| `TOO_FEW_ARGUMENTS` | `EX_ERROR_TOO_FEW_ARGUMENTS` |  | en/ Built in function has too few arguments |
| `TOO_MANY_ARGUMENTS` | `EX_ERROR_TOO_MANY_ARGUMENTS` |  | en/ Built in function has too many arguments |
| `BAD_NESTING` | `EX_ERROR_BAD_NESTING` |  | en/erroneous nesting of constructs like IF/ELSIF/ELSE/ENDIF/REPEAT/LOOP/WHILE and so on |
| `UNFINISHED_NESTING` | `EX_ERROR_UNFINISHED_NESTING` |  | en/nested construct remained unclosed when local_end |
| `SYNTAX` | `EX_ERROR_SYNTAX` |  | en/syntax error, no syntax defintion matches the line or syntax defintion does not match the line |
| `SYNTAX_FATAL` | `EX_ERROR_SYNTAX_FATAL` |  | en/syntax error during checking the line and also noting that the syntax error so serious that none of the other syntax defintions can match the current line |
| `TOO_MANY_WAITING_LABEL` | `EX_ERROR_TOO_MANY_WAITING_LABEL` |  | en/there are more then MAX_SAME_LABEL labels referencing the same line |
| `UNDEF_GLOBAL` | `EX_ERROR_UNDEF_GLOBAL` |  | en/Global variable was not declared. |
| `UNDEF_PRAGMA` | `EX_ERROR_UNDEF_PRAGMA` |  | en/Compiler option \"%s\" is not implemented |
| `GREDEF` | `EX_ERROR_GREDEF` |  | en/Global variable \"%s\" is redefined. |
| `GLODEF` | `EX_ERROR_GLODEF` |  | en/Variable \"%s\" can not be used as global and local in a sub. |
| `CONST_LVAL` | `EX_ERROR_CONST_LVAL` |  | en/The variable is declared as constant. Needs 'var' declaration. |

## MEM

| ID | #define C | Type | Message |
|---|---|---|---|
| `INTERNAL001` | `MEM_ERROR_INTERNAL001` |  | en/DupVar can not duplicate array. This is an internal error. |

## MATCH

| ID | #define C | Type | Message |
|---|---|---|---|
| `INDEX_OUT_OF_RANGE` | `MATCH_ERROR_INDEX_OUT_OF_RANGE` |  | en/The format string contains $n where n is out of range. |
| `BUFFER_SHORT` | `MATCH_ERROR_BUFFER_SHORT` |  | en/The provided buffer is too short. |
| `SYNTAX_ERROR` | `MATCH_ERROR_SYNTAX_ERROR` |  | en/Invalid pattern contains no character after the ~ sign. |
| `ARRAY_SHORT` | `MATCH_ERROR_ARRAY_SHORT` |  | en/The provided string array is too short. |

## SCRIBA

| ID | #define C | Type | Message |
|---|---|---|---|
| `FAIL` | `SCRIBA_ERROR_FAIL` |  | en/Operation failed |
| `BUFFER_SHORT` | `SCRIBA_ERROR_BUFFER_SHORT` |  | en/The provided buffer is too short. |
