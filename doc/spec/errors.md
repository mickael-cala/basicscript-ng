<!-- Généré par tools/gen/extract_errors.py depuis old/errors.def + old/errcodes.h — ne pas éditer à la main.
     Catalogue des codes d'erreur hérités ; décisions de conservation dans ROADMAP Phase 1. -->

# Codes d'erreur BasicScript (héritage ScriptBasic 2.0)

**131 entrées du catalogue** (`errors.def`) + **20 paires SUCCESS/MEMORY_LOW** injectées + **9 codes implicites** + sentinelles = 171 lignes.
 `CT` = compilation, `RT` = exécution, `SYN` = constant généré automatiquement par Jamal.
 Numérotation C issue de `old/errcodes.h` (référence pour la compatibilité ERR() BASIC).

## COMMAND

| N° | ID | Alias BASIC | #define C | Type | Message |
|---|---|---|---|---|---|
| 0 | `SUCCESS` | — | `COMMAND_ERROR_SUCCESS` | SYN | (no error — success) |
| 1 | `MEMORY_LOW` | — | `COMMAND_ERROR_MEMORY_LOW` | SYN | Memory allocation failed (out of memory). |
| 2 | `NOARRAY` | — | `COMMAND_ERROR_NOARRAY` |  | en/Function can not return a whole array |
| 3 | `DIV` | — | `COMMAND_ERROR_DIV` |  | en/Division by zero or other calculation error |
| 4 | `UNDEFOP` | — | `COMMAND_ERROR_UNDEFOP` |  | en/Argument to operator is undefined |
| 5 | `BAD_CALL` | — | `COMMAND_ERROR_BAD_CALL` |  | en/The command or sub was called the wrong way |
| 6 | `FEW_ARGS` | — | `COMMAND_ERROR_FEW_ARGS` |  | en/There are not enough arguments of the module function. |
| 7 | `ARGUMENT_TYPE` | — | `COMMAND_ERROR_ARGUMENT_TYPE` |  | en/The argument passed to a module function is not the needed type. |
| 8 | `ARGUMENT_RANGE` | — | `COMMAND_ERROR_ARGUMENT_RANGE` |  | en/The argument passed to a module function is out of the accepted range. |
| 9 | `FILE_READ` | — | `COMMAND_ERROR_FILE_READ` |  | en/The module experiences difficulties reading the file |
| 10 | `FILE_WRITE` | — | `COMMAND_ERROR_FILE_WRITE` |  | en/The module experiences difficulties writing the file. |
| 11 | `FILE` | — | `COMMAND_ERROR_FILE` |  | en/The module experiences handling the file. |
| 12 | `CIRCULAR` | — | `COMMAND_ERROR_CIRCULAR` |  | en/There is a circular reference in memory. |
| 13 | `MODULE_NOT_LOADED` | — | `COMMAND_ERROR_MODULE_NOT_LOADED` |  | en/The module can not be unloaded, because it was not loaded. |
| 14 | `PARTIAL_UNLOAD` | — | `COMMAND_ERROR_PARTIAL_UNLOAD` |  | en/Some modules were active and could not be unloaded. |
| 15 | `MODULE_ACTIVE` | — | `COMMAND_ERROR_MODULE_ACTIVE` |  | en/The module can not be unloaded, because it is currently active. |
| 16 | `MODULE_LOAD` | — | `COMMAND_ERROR_MODULE_LOAD` |  | en/The requested module can not be loaded. |
| 17 | `MODULE_FUNCTION` | — | `COMMAND_ERROR_MODULE_FUNCTION` |  | en/The requested function does not exist in the module. |
| 18 | `MODULE_INITIALIZE` | — | `COMMAND_ERROR_MODULE_INITIALIZE` |  | en/The module did not initialize correctly |
| 19 | `MODULE_VERSION` | — | `COMMAND_ERROR_MODULE_VERSION` |  | en/The module was developed for a different version of ScriptBasic. |
| 20 | `BAD_FILE_NUMBER` | — | `COMMAND_ERROR_BAD_FILE_NUMBER` |  | en/File number is out of range, it should be between 1 and 512 |
| 21 | `FILE_NUMBER_IS_USED` | — | `COMMAND_ERROR_FILE_NUMBER_IS_USED` |  | en/The file number is already used. |
| 22 | `FILE_CANNOT_BE_OPENED` | — | `COMMAND_ERROR_FILE_CANNOT_BE_OPENED` |  | en/The file can not be opened. |
| 23 | `FILE_IS_NOT_OPENED` | — | `COMMAND_ERROR_FILE_IS_NOT_OPENED` |  | en/The file is not opened. |
| 24 | `INVALID_LOCK` | — | `COMMAND_ERROR_INVALID_LOCK` |  | en/The lock type is invalid. |
| 25 | `PRINT_FAIL` | — | `COMMAND_ERROR_PRINT_FAIL` |  | en/The print command failed. The file may be locked by another process. |
| 26 | `MKDIR_FAIL` | — | `COMMAND_ERROR_MKDIR_FAIL` |  | en/Directory can not be created. |
| 27 | `DELETE_FAIL` | — | `COMMAND_ERROR_DELETE_FAIL` |  | en/The directory or file could not be deleted. |
| 28 | `NOTIMP` | — | `COMMAND_ERROR_NOTIMP` |  | en/Command is not implemented and no currently loaded extension module defined behaviour for it |
| 29 | `INVALID_JOKER` | — | `COMMAND_ERROR_INVALID_JOKER` |  | en/The character can not be a joker or wild card character. |
| 30 | `NO_RESUME` | — | `COMMAND_ERROR_NO_RESUME` |  | en/The code tried to execute a resume while not being in error correction code. |
| 31 | `INV_DNAME` | InvalidDirectoryName | `COMMAND_ERROR_INV_DNAME` |  | en/The directory name in open directory is invalid. |
| 31 | `INV_DNAME` *(implicite)* | sbErrorInvalidDirectoryName | `COMMAND_ERROR_INV_DNAME` | RT | The directory name is invalid. |
| 32 | `INV_DO_OPTION` | InvalidOptionDirOpen | `COMMAND_ERROR_INV_DO_OPTION` |  | en/Invalid option for directory open. |
| 32 | `INV_DO_OPTION` *(implicite)* | sbErrorInvalidOptionDirOpen | `COMMAND_ERROR_INV_DO_OPTION` | RT | The directory option argument is undefined. |
| 33 | `DIR_NO_OPEN` | DirectoryNoOpen | `COMMAND_ERROR_DIR_NO_OPEN` |  | en/The directory can not be opened. |
| 33 | `DIR_NO_OPEN` *(implicite)* | sbErrorDirectoryNoOpen | `COMMAND_ERROR_DIR_NO_OPEN` | RT | The directory can not be opened. |
| 34 | `BAD_RECORD_LENGTH` | — | `COMMAND_ERROR_BAD_RECORD_LENGTH` |  | en/The record length is invalid in the open statements (undefined, zero or negative) |
| 35 | `CURDIR` | NoCurrentDirectory | `COMMAND_ERROR_CURDIR` |  | en/The current directory can not be retrieved for some reason. |
| 35 | `CURDIR` *(implicite)* | sbErrorNoCurrentDirectory | `COMMAND_ERROR_CURDIR` | RT | The current directory can not be retrieved for some reason. |
| 36 | `UNDEF_DIR` | ChDirUndef | `COMMAND_ERROR_UNDEF_DIR` |  | en/The directory name in chdir can not be undef. |
| 36 | `UNDEF_DIR` *(implicite)* | sbErrorChDirUndef | `COMMAND_ERROR_UNDEF_DIR` | RT | The directory argument is undefined. |
| 37 | `CHDIR` | ChDir | `COMMAND_ERROR_CHDIR` |  | en/Cannot change the current working directory to the desired directory. |
| 37 | `CHDIR` *(implicite)* | sbErrorChDir | `COMMAND_ERROR_CHDIR` | RT | The current directory can not be changed. |
| 38 | `RETURN_WITHOUT_GOSUB` | — | `COMMAND_ERROR_RETURN_WITHOUT_GOSUB` |  | en/The command RETURN can not be executed, because there is no where to return. |
| 39 | `INVALID_ARGUMENT_FOR_FUNCTION_ADDRESS` | — | `COMMAND_ERROR_INVALID_ARGUMENT_FOR_FUNCTION_ADDRESS` |  | en/The argument for the function address is invalid. |
| 40 | `SETFILE_INVALID_ATTRIBUTE` | — | `COMMAND_ERROR_SETFILE_INVALID_ATTRIBUTE` |  | en/The attribute value or symbol is invalid in the set file command. |
| 41 | `CHOWN_INVALID_USER` | — | `COMMAND_ERROR_CHOWN_INVALID_USER` |  | en/The user does not exist. |
| 42 | `CHOWN_NOT_SUPPORTED` | — | `COMMAND_ERROR_CHOWN_NOT_SUPPORTED` |  | en/The chown command is not supported on Win95 and Win98 |
| 43 | `CHOWN_SET_OWNER` | — | `COMMAND_ERROR_CHOWN_SET_OWNER` |  | en/Can not change owner. |
| 44 | `INVALID_FILE_NAME` | — | `COMMAND_ERROR_INVALID_FILE_NAME` |  | en/The file name is invalid. |
| 45 | `CREATIME_FAIL` | SetCreateTime | `COMMAND_ERROR_CREATIME_FAIL` |  | en/Setting the create time of the file has failed. |
| 45 | `CREATIME_FAIL` *(implicite)* | sbErrorSetCreateTime | `COMMAND_ERROR_CREATIME_FAIL` | RT | Setting the create time of the file has failed. |
| 46 | `MODTIME_FAIL` | SetModifyTime | `COMMAND_ERROR_MODTIME_FAIL` |  | en/Setting the modify time of the file has failed. |
| 46 | `MODTIME_FAIL` *(implicite)* | sbErrorSetModifyTime | `COMMAND_ERROR_MODTIME_FAIL` | RT | Setting the modify time of the file has failed. |
| 47 | `ACCTIM_FAIL` | SetAccessTime | `COMMAND_ERROR_ACCTIM_FAIL` |  | en/Setting the access time of the file has failed |
| 47 | `ACCTIM_FAIL` *(implicite)* | sbErrorSetAccessTime | `COMMAND_ERROR_ACCTIM_FAIL` | RT | Setting the access time of the file has failed |
| 48 | `INVALID_TIME_FORMAT` | — | `COMMAND_ERROR_INVALID_TIME_FORMAT` |  | en/The specified time format is invalid |
| 49 | `INVALID_TIME` | — | `COMMAND_ERROR_INVALID_TIME` |  | en/The time is not valid, cannot be earlier than January 1, 1970. 00:00 |
| 50 | `EXTENSION_SPECIFIC` | — | `COMMAND_ERROR_EXTENSION_SPECIFIC` |  | en/Extension specific error: %s |
| 51 | `SOCKET_FILE` | — | `COMMAND_ERROR_SOCKET_FILE` |  | en/The operation can be done on files only and not on sockets. |
| 52 | `INVALID_CODE` | — | `COMMAND_ERROR_INVALID_CODE` |  | en/The embedding application tried to start the code at an invalid location |
| 53 | `MANDARG` | — | `COMMAND_ERROR_MANDARG` |  | en/Mandatory argument is missing |
| 54 | `TIMEOUT` | — | `COMMAND_ERROR_TIMEOUT` |  | en/Subprocess did not finish within time limits |
| 55 | `STAYS_IN_MEMORY` | — | `COMMAND_ERROR_STAYS_IN_MEMORY` |  | en/The module can not be unloaded |
| 56 | `PREPROCESSOR_ABORT` | — | `COMMAND_ERROR_PREPROCESSOR_ABORT` |  | en/The preprocessor said to abort program compilation or execution. |

## BU

| N° | ID | Alias BASIC | #define C | Type | Message |
|---|---|---|---|---|---|
| 0 | `SUCCESS` | — | `BU_ERROR_SUCCESS` | SYN | (no error — success) |
| 1 | `MEMORY_LOW` | — | `BU_ERROR_MEMORY_LOW` | SYN | Memory allocation failed (out of memory). |
| 57 | `FILE_CORRUPT` | — | `BU_ERROR_FILE_CORRUPT` |  | en/The file is either corrupt or was written with a different version of sb. |
| 58 | `NO_CODE` | — | `BU_ERROR_NO_CODE` |  | en/The compiled program contains no executable code. |
| 59 | `FAIL` | — | `BU_ERROR_FAIL` |  | en/Code file can not be saved. |
| 60 | `ECODE_INPUT` | — | `BU_ERROR_ECODE_INPUT` |  | en/The interpreter file can not be read |

## READER

| N° | ID | Alias BASIC | #define C | Type | Message |
|---|---|---|---|---|---|
| 0 | `SUCCESS` | — | `READER_ERROR_SUCCESS` | SYN | (no error — success) |
| 1 | `MEMORY_LOW` | — | `READER_ERROR_MEMORY_LOW` | SYN | Memory allocation failed (out of memory). |
| 61 | `INCLUDE_SYNTAX` | — | `READER_ERROR_INCLUDE_SYNTAX` |  | en/Bad syntax in include statement. |
| 62 | `INCLUDE_FILE` | — | `READER_ERROR_INCLUDE_FILE` |  | en/Include file is not found |
| 63 | `TOOMANY_INCLUDE` | — | `READER_ERROR_TOOMANY_INCLUDE` |  | en/Too many includes, probably recursive include |
| 64 | `PREPROC_UNDEF` | — | `READER_ERROR_PREPROC_UNDEF` |  | en/Preprocessor \"%s\" is not defined. |
| 65 | `PREPROC_LONG` | — | `READER_ERROR_PREPROC_LONG` |  | en/Preprocessor name is too long. |
| 66 | `PREPROC_NOTAVA` | — | `READER_ERROR_PREPROC_NOTAVA` |  | en/Preprocessor \"%s\" is not available. |
| 67 | `PREPROC_NOTVAL` | — | `READER_ERROR_PREPROC_NOTVAL` |  | en/Preprocessor \"%s\" is invalid. |
| 68 | `FILE_OPEN` | — | `READER_ERROR_FILE_OPEN` |  | en/The file can not be read. |
| 69 | `EMPTY_INPUT` | — | `READER_ERROR_EMPTY_INPUT` |  | en/The file is empty or is not readable. |

## PREPROC

| N° | ID | Alias BASIC | #define C | Type | Message |
|---|---|---|---|---|---|
| 0 | `SUCCESS` | — | `PREPROC_ERROR_SUCCESS` | SYN | (no error — success) |
| 1 | `MEMORY_LOW` | — | `PREPROC_ERROR_MEMORY_LOW` | SYN | Memory allocation failed (out of memory). |
| 70 | `FAIL` | — | `PREPROC_ERROR_FAIL` |  | en/The external preprocessor failed. |
| 71 | `CONFIG_EXE` | — | `PREPROC_ERROR_CONFIG_EXE` |  | en/The preprocessor executable is not configured. |
| 72 | `CONFIG_DIR` | — | `PREPROC_ERROR_CONFIG_DIR` |  | en/The preprocessor temporary directory is not configured. |

## LEX

| N° | ID | Alias BASIC | #define C | Type | Message |
|---|---|---|---|---|---|
| 0 | `SUCCESS` | — | `LEX_ERROR_SUCCESS` | SYN | (no error — success) |
| 1 | `MEMORY_LOW` | — | `LEX_ERROR_MEMORY_LOW` | SYN | Memory allocation failed (out of memory). |
| 73 | `TOO_LONG_SYMBOL` | — | `LEX_ERROR_TOO_LONG_SYMBOL` |  | en/Symbol is too long |
| 74 | `STRING_NOT_TERMINATED` | — | `LEX_ERROR_STRING_NOT_TERMINATED` |  | en/string is not terminated anywhere before end of file |
| 75 | `STRING_NEW_LINE` | — | `LEX_ERROR_STRING_NEW_LINE` |  | en/string is not terminated anywhere before end of line |
| 76 | `BAD_RADIX` | — | `LEX_ERROR_BAD_RADIX` |  | en/the number contains invalid radix. BASE#NNN numbers areavailable for 2<=BASE<=36 only. |
| 77 | `INVALID_NUMBER` | — | `LEX_ERROR_INVALID_NUMBER` |  | en/the BASE#NNN formatted number digit is out of range |

## EXE

| N° | ID | Alias BASIC | #define C | Type | Message |
|---|---|---|---|---|---|
| 0 | `SUCCESS` | — | `EXE_ERROR_SUCCESS` | SYN | (no error — success) |
| 1 | `MEMORY_LOW` | — | `EXE_ERROR_MEMORY_LOW` | SYN | Memory allocation failed (out of memory). |
| 78 | `INVALID_PC` | — | `EXE_ERROR_INVALID_PC` |  | en/Program counter points out of the code. The executed code is corrupt. |
| 79 | `INVALID_PC1` | — | `EXE_ERROR_INVALID_PC1` |  | en/Internal error or the cached code is corrupt. |
| 80 | `INVALID_NODE` | — | `EXE_ERROR_INVALID_NODE` |  | en/Internal error or the cached code is corrupt. |
| 81 | `INVALID_OPCODE` | — | `EXE_ERROR_INVALID_OPCODE` |  | en/invalid op code is in the code table |
| 82 | `NOT_IMPLEMENTED` | — | `EXE_ERROR_NOT_IMPLEMENTED` |  | en/The code exists, but it is not implemented. |
| 83 | `INVALID_EXPRESSION_NODE` | — | `EXE_ERROR_INVALID_EXPRESSION_NODE` |  | en/Internal error or the cached code is corrupt. |
| 84 | `INVALID_EXPRESSION_NODE1` | — | `EXE_ERROR_INVALID_EXPRESSION_NODE1` |  | en/Internal error or the cached code is corrupt. |
| 85 | `INTERNAL_DEFAULTTYPE` | — | `EXE_ERROR_INTERNAL_DEFAULTTYPE` |  | en/Internal error when releasing a variable it has no correct type |
| 86 | `INTERNAL_UNDEF` | — | `EXE_ERROR_INTERNAL_UNDEF` |  | en/Internal error when releasing undefined variable, nonsense |
| 87 | `INTERNAL_MEMREF` | — | `EXE_ERROR_INTERNAL_MEMREF` |  | en/Internal error when releasing variable reference found |
| 88 | `INTERNAL` | — | `EXE_ERROR_INTERNAL` |  | en/any non classified internal error |
| 89 | `NO_LOCAL` | — | `EXE_ERROR_NO_LOCAL` |  | en/local variable referenced in global context or in a local context having no local variables |
| 90 | `INVALID_LVALNODE0` | — | `EXE_ERROR_INVALID_LVALNODE0` |  | en/Internal error or the cached code is corrupt. |
| 91 | `INVALID_LVALNODE1` | — | `EXE_ERROR_INVALID_LVALNODE1` |  | en/Internal error or the cached code is corrupt. |
| 92 | `INVALID_LVALNODE2` | — | `EXE_ERROR_INVALID_LVALNODE2` |  | en/Internal error or the cached code is corrupt. |
| 93 | `INVALID_LVALNODE3` | — | `EXE_ERROR_INVALID_LVALNODE3` |  | en/Internal error or the cached code is corrupt. |
| 94 | `INVALID_LVALNODE4` | — | `EXE_ERROR_INVALID_LVALNODE4` |  | en/Internal error or the cached code is corrupt. |
| 95 | `INVALID_LVALNODE5` | — | `EXE_ERROR_INVALID_LVALNODE5` |  | en/Internal error or the cached code is corrupt. |
| 96 | `INVALID_LVALNODE6` | — | `EXE_ERROR_INVALID_LVALNODE6` |  | en/Internal error or the cached code is corrupt. |
| 97 | `USERFUN_UNDEFINED` | — | `EXE_ERROR_USERFUN_UNDEFINED` |  | en/the user function is undefined |
| 98 | `TOO_DEEP_CALL` | — | `EXE_ERROR_TOO_DEEP_CALL` |  | en/Recursive function calls exceeded configuration limit. |
| 99 | `TOO_LONG_RUN` | — | `EXE_ERROR_TOO_LONG_RUN` |  | en/Program is running too long, probably infinite loop. |

## EX

| N° | ID | Alias BASIC | #define C | Type | Message |
|---|---|---|---|---|---|
| 0 | `SUCCESS` | — | `EX_ERROR_SUCCESS` | SYN | (no error — success) |
| 1 | `MEMORY_LOW` | — | `EX_ERROR_MEMORY_LOW` | SYN | Memory allocation failed (out of memory). |
| 100 | `INTERNAL` | — | `EX_ERROR_INTERNAL` |  | en/Internal error in the syntax analyzer |
| 101 | `LABEL_DOUBLE_DEFINED` | — | `EX_ERROR_LABEL_DOUBLE_DEFINED` |  | en/A line label was defined more than once. |
| 102 | `LABEL_NOT_DEFINED` | — | `EX_ERROR_LABEL_NOT_DEFINED` |  | en/A label \"%s\" was not defined during syntax analysis. |
| 103 | `FUNCTION_DOUBLE_DEFINED` | — | `EX_ERROR_FUNCTION_DOUBLE_DEFINED` |  | en/The user function was already defined. |
| 104 | `FUNCTION_NOT_DEFINED` | — | `EX_ERROR_FUNCTION_NOT_DEFINED` |  | en/The user function \"%s\" is used but is not defined. |
| 105 | `NO_OLD_NAMESPACE` | — | `EX_ERROR_NO_OLD_NAMESPACE` |  | en/There was no module started when an end module statement was found. |
| 106 | `UNFINISHED_MODULE` | — | `EX_ERROR_UNFINISHED_MODULE` |  | en/A module was not closed at end of file. |
| 107 | `TOO_LONG_NAME_SPACE` | — | `EX_ERROR_TOO_LONG_NAME_SPACE` |  | en/the name space grew too long |
| 108 | `TOO_LONG_VARIABLE` | — | `EX_ERROR_TOO_LONG_VARIABLE` |  | en/variable and namespace does not fit into the buffer |
| 109 | `MISSING_PAREN` | — | `EX_ERROR_MISSING_PAREN` |  | en/ closing ) is missing in expression |
| 110 | `MISSING_OPAREN` | — | `EX_ERROR_MISSING_OPAREN` |  | en/ opening ( is missing in sub call |
| 111 | `MISSING_APAREN` | — | `EX_ERROR_MISSING_APAREN` |  | en/ closing ] is missing following array indeces |
| 112 | `MISSING_SAPAREN` | — | `EX_ERROR_MISSING_SAPAREN` |  | en/ closing } is missing following associative array indices |
| 113 | `FUNCTION_NEEDS_ARGUMENTS` | — | `EX_ERROR_FUNCTION_NEEDS_ARGUMENTS` |  | en/ built in function needs arguments |
| 114 | `TOO_FEW_ARGUMENTS` | — | `EX_ERROR_TOO_FEW_ARGUMENTS` |  | en/ Built in function has too few arguments |
| 115 | `TOO_MANY_ARGUMENTS` | — | `EX_ERROR_TOO_MANY_ARGUMENTS` |  | en/ Built in function has too many arguments |
| 116 | `BAD_NESTING` | — | `EX_ERROR_BAD_NESTING` |  | en/erroneous nesting of constructs like IF/ELSIF/ELSE/ENDIF/REPEAT/LOOP/WHILE and so on |
| 117 | `UNFINISHED_NESTING` | — | `EX_ERROR_UNFINISHED_NESTING` |  | en/nested construct remained unclosed when local_end |
| 118 | `SYNTAX` | — | `EX_ERROR_SYNTAX` |  | en/syntax error, no syntax defintion matches the line or syntax defintion does not match the line |
| 119 | `SYNTAX_FATAL` | — | `EX_ERROR_SYNTAX_FATAL` |  | en/syntax error during checking the line and also noting that the syntax error so serious that none of the other syntax defintions can match the current line |
| 120 | `TOO_MANY_WAITING_LABEL` | — | `EX_ERROR_TOO_MANY_WAITING_LABEL` |  | en/there are more then MAX_SAME_LABEL labels referencing the same line |
| 121 | `UNDEF_GLOBAL` | — | `EX_ERROR_UNDEF_GLOBAL` |  | en/Global variable was not declared. |
| 122 | `UNDEF_PRAGMA` | — | `EX_ERROR_UNDEF_PRAGMA` |  | en/Compiler option \"%s\" is not implemented |
| 123 | `GREDEF` | — | `EX_ERROR_GREDEF` |  | en/Global variable \"%s\" is redefined. |
| 124 | `GLODEF` | — | `EX_ERROR_GLODEF` |  | en/Variable \"%s\" can not be used as global and local in a sub. |
| 125 | `CONST_LVAL` | — | `EX_ERROR_CONST_LVAL` |  | en/The variable is declared as constant. Needs 'var' declaration. |

## MEM

| N° | ID | Alias BASIC | #define C | Type | Message |
|---|---|---|---|---|---|
| 0 | `SUCCESS` | — | `MEM_ERROR_SUCCESS` | SYN | (no error — success) |
| 1 | `MEMORY_LOW` | — | `MEM_ERROR_MEMORY_LOW` | SYN | Memory allocation failed (out of memory). |
| 126 | `INTERNAL001` | — | `MEM_ERROR_INTERNAL001` |  | en/DupVar can not duplicate array. This is an internal error. |

## MATCH

| N° | ID | Alias BASIC | #define C | Type | Message |
|---|---|---|---|---|---|
| 0 | `SUCCESS` | — | `MATCH_ERROR_SUCCESS` | SYN | (no error — success) |
| 1 | `MEMORY_LOW` | — | `MATCH_ERROR_MEMORY_LOW` | SYN | Memory allocation failed (out of memory). |
| 127 | `INDEX_OUT_OF_RANGE` | — | `MATCH_ERROR_INDEX_OUT_OF_RANGE` |  | en/The format string contains $n where n is out of range. |
| 128 | `BUFFER_SHORT` | — | `MATCH_ERROR_BUFFER_SHORT` |  | en/The provided buffer is too short. |
| 129 | `SYNTAX_ERROR` | — | `MATCH_ERROR_SYNTAX_ERROR` |  | en/Invalid pattern contains no character after the ~ sign. |
| 130 | `ARRAY_SHORT` | — | `MATCH_ERROR_ARRAY_SHORT` |  | en/The provided string array is too short. |

## SCRIBA

| N° | ID | Alias BASIC | #define C | Type | Message |
|---|---|---|---|---|---|
| 0 | `SUCCESS` | — | `SCRIBA_ERROR_SUCCESS` | SYN | (no error — success) |
| 1 | `MEMORY_LOW` | — | `SCRIBA_ERROR_MEMORY_LOW` | SYN | Memory allocation failed (out of memory). |
| 131 | `FAIL` | — | `SCRIBA_ERROR_FAIL` |  | en/Operation failed |
| 132 | `BUFFER_SHORT` | — | `SCRIBA_ERROR_BUFFER_SHORT` |  | en/The provided buffer is too short. |

## (sentinelle)

| N° | ID | Alias BASIC | #define C | Type | Message |
|---|---|---|---|---|---|
| 56 | `LAST_COMMAND` | — | `LAST_COMMAND` | — | Borne haute implicite du module COMMAND. |
| 60 | `LAST_BU` | — | `LAST_BU` | — | Borne haute implicite du module BU. |
| 69 | `LAST_READER` | — | `LAST_READER` | — | Borne haute implicite du module READER. |
| 72 | `LAST_PREPROC` | — | `LAST_PREPROC` | — | Borne haute implicite du module PREPROC. |
| 77 | `LAST_LEX` | — | `LAST_LEX` | — | Borne haute implicite du module LEX. |
| 99 | `LAST_EXE` | — | `LAST_EXE` | — | Borne haute implicite du module EXE. |
| 125 | `LAST_EX` | — | `LAST_EX` | — | Borne haute implicite du module EX. |
| 126 | `LAST_MEM` | — | `LAST_MEM` | — | Borne haute implicite du module MEM. |
| 130 | `LAST_MATCH` | — | `LAST_MATCH` | — | Borne haute implicite du module MATCH. |
| 132 | `LAST_SCRIBA` | — | `LAST_SCRIBA` | — | Borne haute implicite du module SCRIBA. |
| 133 | `MAX_ERROR_CODE` | — | `MAX_ERROR_CODE` | — | Sentinelle : nombre total de codes (tableau de messages). |

## Notes

- Les paires `SUCCESS`(0)/`MEMORY_LOW`(1) sont générées automatiquement par Jamal pour chaque module (garanti par l'en-tête de `old/errors.def`) ; elles n'apparaissent pas dans le `.def`.
- `MAX_ERROR_CODE` et `LAST_<MODULE>` sont des sentinelles internes (taille de tableau / borne haute).
- Les 9 codes *implicites* du module COMMAND (dossier/heure de fichier) sont utilisés par le code sans entrée message dans `errors.def` ; leurs messages ci-dessus sont reconstitués depuis l'usage (`old/commands/file.c`, `old/hookers.c`, `old/filesys.c`). Voir ERRATA.md E-004.
