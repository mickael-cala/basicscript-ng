<!-- DEBUT TABLE ERREURS ENGENDREE - scripts/docs.py -->

_Table engendree par `scripts/docs.py` depuis `src/errcodes.h` et `src/errcodes.c`. Ne pas editer a la main : `python scripts/docs.py --write`._

| code | symboles | message |
|---|---|---|
| 0 | `BU_ERROR_SUCCESS` (alias : `COMMAND_ERROR_SUCCESS`, `CONFIG_ERROR_SUCCESS`, `EXE_ERROR_SUCCESS`, `EX_ERROR_SUCCESS`, `LEX_ERROR_SUCCESS`, `MATCH_ERROR_SUCCESS`, `MEM_ERROR_SUCCESS`, `PREPROC_ERROR_SUCCESS`, `READER_ERROR_SUCCESS`, `SCRIBA_ERROR_SUCCESS`) | SUCCESS |
| 1 | `BU_ERROR_MEMORY_LOW` (alias : `COMMAND_ERROR_MEMORY_LOW`, `CONFIG_ERROR_MEMORY_LOW`, `EXE_ERROR_MEMORY_LOW`, `EX_ERROR_MEMORY_LOW`, `LEX_ERROR_MEMORY_LOW`, `MATCH_ERROR_MEMORY_LOW`, `MEM_ERROR_MEMORY_LOW`, `PREPROC_ERROR_MEMORY_LOW`, `READER_ERROR_MEMORY_LOW`, `SCRIBA_ERROR_MEMORY_LOW`) | Not enough memory |
| 2 | `COMMAND_ERROR_NOARRAY` | Function can not return a whole array |
| 3 | `COMMAND_ERROR_DIV` | Division by zero or other calculation error |
| 4 | `COMMAND_ERROR_UNDEFOP` | Argument to operator is undefined |
| 5 | `COMMAND_ERROR_BAD_CALL` | The command or sub was called the wrong way |
| 6 | `COMMAND_ERROR_FEW_ARGS` | There are not enough arguments of the module function. |
| 7 | `COMMAND_ERROR_ARGUMENT_TYPE` | The argument passed to a module function is not the needed type. |
| 8 | `COMMAND_ERROR_ARGUMENT_RANGE` | The argument passed to a module function is out of the accepted range. |
| 9 | `COMMAND_ERROR_FILE_READ` | The module experiences difficulties reading the file |
| 10 | `COMMAND_ERROR_FILE_WRITE` | The module experiences difficulties writing the file. |
| 11 | `COMMAND_ERROR_FILE` | The module experiences handling the file. |
| 12 | `COMMAND_ERROR_CIRCULAR` | There is a circular reference in memory. |
| 13 | `COMMAND_ERROR_MODULE_NOT_LOADED` | The module can not be unloaded, because it was not loaded. |
| 14 | `COMMAND_ERROR_PARTIAL_UNLOAD` | Some modules were active and could not be unloaded. |
| 15 | `COMMAND_ERROR_MODULE_ACTIVE` | The module can not be unloaded, because it is currently active. |
| 16 | `COMMAND_ERROR_MODULE_LOAD` | The requested module can not be loaded. |
| 17 | `COMMAND_ERROR_MODULE_FUNCTION` | The requested function does not exist in the module. |
| 18 | `COMMAND_ERROR_MODULE_INITIALIZE` | The module did not initialize correctly |
| 19 | `COMMAND_ERROR_MODULE_VERSION` | The module was developed for a different version of ScriptBasic. |
| 20 | `COMMAND_ERROR_BAD_FILE_NUMBER` | File number is out of range, it should be between 1 and 512 |
| 21 | `COMMAND_ERROR_FILE_NUMBER_IS_USED` | The file number is already used. |
| 22 | `COMMAND_ERROR_FILE_CANNOT_BE_OPENED` | The file can not be opened. |
| 23 | `COMMAND_ERROR_FILE_IS_NOT_OPENED` | The file is not opened. |
| 24 | `COMMAND_ERROR_INVALID_LOCK` | The lock type is invalid. |
| 25 | `COMMAND_ERROR_PRINT_FAIL` | The print command failed. The file may be locked by another process. |
| 26 | `COMMAND_ERROR_MKDIR_FAIL` | Directory can not be created. |
| 27 | `COMMAND_ERROR_DELETE_FAIL` | The directory or file could not be deleted. |
| 28 | `COMMAND_ERROR_NOTIMP` | Command is not implemented and no currently loaded extension module defined behaviour for it |
| 29 | `COMMAND_ERROR_INVALID_JOKER` | The character can not be a joker or wild card character. |
| 30 | `COMMAND_ERROR_NO_RESUME` | The code tried to execute a resume while not being in error correction code. |
| 31 | `COMMAND_ERROR_INV_DNAME` | The directory name in open directory is invalid. |
| 32 | `COMMAND_ERROR_INV_DO_OPTION` | Invalid option for directory open. |
| 33 | `COMMAND_ERROR_DIR_NO_OPEN` | The directory can not be opened. |
| 34 | `COMMAND_ERROR_BAD_RECORD_LENGTH` | The record length is invalid in the open statements (undefined, zero or negative) |
| 35 | `COMMAND_ERROR_CURDIR` | The current directory can not be retrieved for some reason. |
| 36 | `COMMAND_ERROR_UNDEF_DIR` | The directory name in chdir can not be undef. |
| 37 | `COMMAND_ERROR_CHDIR` | Cannot change the current working directory to the desired directory. |
| 38 | `COMMAND_ERROR_RETURN_WITHOUT_GOSUB` | The command RETURN can not be executed, because there is no where to return. |
| 39 | `COMMAND_ERROR_INVALID_ARGUMENT_FOR_FUNCTION_ADDRESS` | The argument for the function address is invalid. |
| 40 | `COMMAND_ERROR_SETFILE_INVALID_ATTRIBUTE` | The attribute value or symbol is invalid in the set file command. |
| 41 | `COMMAND_ERROR_CHOWN_INVALID_USER` | The user does not exist. |
| 42 | `COMMAND_ERROR_CHOWN_NOT_SUPPORTED` | The chown command is not supported on Win95 and Win98 |
| 43 | `COMMAND_ERROR_CHOWN_SET_OWNER` | Can not change owner. |
| 44 | `COMMAND_ERROR_INVALID_FILE_NAME` | The file name is invalid. |
| 45 | `COMMAND_ERROR_CREATIME_FAIL` | Setting the create time of the file has failed. |
| 46 | `COMMAND_ERROR_MODTIME_FAIL` | Setting the modify time of the file has failed. |
| 47 | `COMMAND_ERROR_ACCTIM_FAIL` | Setting the access time of the file has failed |
| 48 | `COMMAND_ERROR_INVALID_TIME_FORMAT` | The specified time format is invalid |
| 49 | `COMMAND_ERROR_INVALID_TIME` | The time is not valid, cannot be earlier than January 1, 1970. 00:00 |
| 50 | `COMMAND_ERROR_EXTENSION_SPECIFIC` | Extension specific error: %s |
| 51 | `COMMAND_ERROR_SOCKET_FILE` | The operation can be done on files only and not on sockets. |
| 52 | `COMMAND_ERROR_INVALID_CODE` | The embedding application tried to start the code at an invalid location |
| 53 | `COMMAND_ERROR_MANDARG` | Mandatory argument is missing |
| 54 | `COMMAND_ERROR_TIMEOUT` | Subprocess did not finish within time limits |
| 55 | `COMMAND_ERROR_STAYS_IN_MEMORY` | The module can not be unloaded |
| 56 | `COMMAND_ERROR_PREPROCESSOR_ABORT` | The preprocessor said to abort program compilation or execution. |
| 57 | `BU_ERROR_FILE_CORRUPT` | The file is either corrupt or was written with a different version of sb. |
| 58 | `BU_ERROR_NO_CODE` | The compiled program contains no executable code. |
| 59 | `BU_ERROR_FAIL` | Code file can not be saved. |
| 60 | `BU_ERROR_ECODE_INPUT` | The interpreter file can not be read |
| 61 | `READER_ERROR_INCLUDE_SYNTAX` | Bad syntax in include statement. |
| 62 | `READER_ERROR_INCLUDE_FILE` | Include file is not found |
| 63 | `READER_ERROR_TOOMANY_INCLUDE` | Too many includes, probably recursive include |
| 64 | `READER_ERROR_PREPROC_UNDEF` | Preprocessor name is too long. |
| 65 | `READER_ERROR_PREPROC_LONG` | The file can not be read. |
| 66 | `READER_ERROR_PREPROC_NOTAVA` | The file is empty or is not readable. |
| 67 | `READER_ERROR_PREPROC_NOTVAL` | The external preprocessor failed. |
| 68 | `READER_ERROR_FILE_OPEN` | The preprocessor executable is not configured. |
| 69 | `READER_ERROR_EMPTY_INPUT` | The preprocessor temporary directory is not configured. |
| 70 | `PREPROC_ERROR_FAIL` | Symbol is too long |
| 71 | `PREPROC_ERROR_CONFIG_EXE` | string is not terminated anywhere before end of file |
| 72 | `PREPROC_ERROR_CONFIG_DIR` | string is not terminated anywhere before end of line |
| 73 | `LEX_ERROR_TOO_LONG_SYMBOL` | the number contains invalid radix. BASE#NNN numbers areavailable for 2<=BASE<=36 only. |
| 74 | `LEX_ERROR_STRING_NOT_TERMINATED` | the BASE#NNN formatted number digit is out of range |
| 75 | `LEX_ERROR_STRING_NEW_LINE` | Program counter points out of the code. The executed code is corrupt. |
| 76 | `LEX_ERROR_BAD_RADIX` | Internal error or the cached code is corrupt. |
| 77 | `LEX_ERROR_INVALID_NUMBER` | Internal error or the cached code is corrupt. |
| 78 | `EXE_ERROR_INVALID_PC` | invalid op code is in the code table |
| 79 | `EXE_ERROR_INVALID_PC1` | The code exists, but it is not implemented. |
| 80 | `EXE_ERROR_INVALID_NODE` | Internal error or the cached code is corrupt. |
| 81 | `EXE_ERROR_INVALID_OPCODE` | Internal error or the cached code is corrupt. |
| 82 | `EXE_ERROR_NOT_IMPLEMENTED` | Internal error when releasing a variable it has no correct type |
| 83 | `EXE_ERROR_INVALID_EXPRESSION_NODE` | Internal error when releasing undefined variable, nonsense |
| 84 | `EXE_ERROR_INVALID_EXPRESSION_NODE1` | Internal error when releasing variable reference found |
| 85 | `EXE_ERROR_INTERNAL_DEFAULTTYPE` | any non classified internal error |
| 86 | `EXE_ERROR_INTERNAL_UNDEF` | local variable referenced in global context or in a local context having no local variables |
| 87 | `EXE_ERROR_INTERNAL_MEMREF` | Internal error or the cached code is corrupt. |
| 88 | `EXE_ERROR_INTERNAL` | Internal error or the cached code is corrupt. |
| 89 | `EXE_ERROR_NO_LOCAL` | Internal error or the cached code is corrupt. |
| 90 | `EXE_ERROR_INVALID_LVALNODE0` | Internal error or the cached code is corrupt. |
| 91 | `EXE_ERROR_INVALID_LVALNODE1` | Internal error or the cached code is corrupt. |
| 92 | `EXE_ERROR_INVALID_LVALNODE2` | Internal error or the cached code is corrupt. |
| 93 | `EXE_ERROR_INVALID_LVALNODE3` | Internal error or the cached code is corrupt. |
| 94 | `EXE_ERROR_INVALID_LVALNODE4` | the user function is undefined |
| 95 | `EXE_ERROR_INVALID_LVALNODE5` | Recursive function calls exceeded configuration limit. |
| 96 | `EXE_ERROR_INVALID_LVALNODE6` | Program is running too long, probably infinite loop. |
| 97 | `EXE_ERROR_USERFUN_UNDEFINED` | Internal error in the syntax analyzer |
| 98 | `EXE_ERROR_TOO_DEEP_CALL` | A line label was defined more than once. |
| 99 | `EXE_ERROR_TOO_LONG_RUN` | The user function was already defined. |
| 100 | `EX_ERROR_INTERNAL` | There was no module started when an end module statement was found. |
| 101 | `EX_ERROR_LABEL_DOUBLE_DEFINED` | A module was not closed at end of file. |
| 102 | `EX_ERROR_LABEL_NOT_DEFINED` | the name space grew too long |
| 103 | `EX_ERROR_FUNCTION_DOUBLE_DEFINED` | variable and namespace does not fit into the buffer |
| 104 | `EX_ERROR_FUNCTION_NOT_DEFINED` |  closing ) is missing in expression |
| 105 | `EX_ERROR_NO_OLD_NAMESPACE` |  opening ( is missing in sub call |
| 106 | `EX_ERROR_UNFINISHED_MODULE` |  closing ] is missing following array indeces |
| 107 | `EX_ERROR_TOO_LONG_NAME_SPACE` |  closing } is missing following associative array indices |
| 108 | `EX_ERROR_TOO_LONG_VARIABLE` |  built in function needs arguments |
| 109 | `EX_ERROR_MISSING_PAREN` |  Built in function has too few arguments |
| 110 | `EX_ERROR_MISSING_OPAREN` |  Built in function has too many arguments |
| 111 | `EX_ERROR_MISSING_APAREN` | erroneous nesting of constructs like IF/ELSIF/ELSE/ENDIF/REPEAT/LOOP/WHILE and so on |
| 112 | `EX_ERROR_MISSING_SAPAREN` | nested construct remained unclosed when local_end |
| 113 | `EX_ERROR_FUNCTION_NEEDS_ARGUMENTS` | syntax error, no syntax defintion matches the line or syntax defintion does not match the line |
| 114 | `EX_ERROR_TOO_FEW_ARGUMENTS` | syntax error during checking the line and also noting that the syntax error so serious that none of the other syntax defintions can match the current line |
| 115 | `EX_ERROR_TOO_MANY_ARGUMENTS` | there are more then MAX_SAME_LABEL labels referencing the same line |
| 116 | `EX_ERROR_BAD_NESTING` | Global variable was not declared. |
| 117 | `EX_ERROR_UNFINISHED_NESTING` | The variable is declared as constant. Needs 'var' declaration. |
| 118 | `EX_ERROR_SYNTAX` | DupVar can not duplicate array. This is an internal error. |
| 119 | `EX_ERROR_SYNTAX_FATAL` | The format string contains $n where n is out of range. |
| 120 | `EX_ERROR_TOO_MANY_WAITING_LABEL` | The provided buffer is too short. |
| 121 | `EX_ERROR_UNDEF_GLOBAL` | Invalid pattern contains no character after the ~ sign. |
| 122 | `EX_ERROR_UNDEF_PRAGMA` | The provided string array is too short. |
| 123 | `EX_ERROR_GREDEF` | Operation failed |
| 124 | `EX_ERROR_GLODEF` | The provided buffer is too short. |

**125 codes references.** Au-dela de MAX_ERROR_CODE (133) commence l'espace des erreurs specifiques aux modules (`0x00080000` et plus), dont le message est fourni par le module lui-meme (E46).

<!-- FIN TABLES ENGENDREES -->
