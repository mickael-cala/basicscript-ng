<!-- Généré par tools/gen/extract_syntax.py depuis old/syntax.def — ne pas éditer à la main.
     Document de travail Phase 1 ; voir ROADMAP.md. -->

# Grammaire BasicScript (extraite de l'héritage ScriptBasic 2.0)

- Source : `old/syntax.def` (831 entrées)

## Mots-clés de commande (%COMMANDS)

| Mot-clé | Terminaux | Non-terminaux |
|---|---|---|
| `EMPTY` |  | nl noexec |
| `MODULE` | `module` | name_space nl noexec |
| `ENDMODULE` | `end` `module` | end_name_space nl noexec |
| `LOCAL` | `local` | local_list nl noexec |
| `STOP` | `stop` | nl |
| `END` | `end` | nl |
| `PRAGMA` | `declare` `option` | pragma nl noexec |
| `OPTION` | `option` | absolute_symbol expression |
| `LABEL` | `:` | label_def noexec |
| `CONST` | `const` `=` | cname cval nl noexec |
| `GCONST` | `global` `const` `=` | gcname cval nl noexec |
| `GLOBAL` | `global` | global_list nl noexec |
| `VAR` | `var` | cname nl noexec |
| `SWAP` | `swap` `,` | lval lval nl |
| `FPRINTNL` | `printnl` `#` | expression nl |
| `FPRINT` | `print` `#` `,` | expression expression_list nl |
| `PRINTNL` | `printnl` | nl |
| `PRINTNL` | `print` | nl |
| `PRINT` | `print` | expression_list nl |
| `FUNCTIONARG` | `function` `(` `)` | function local_start local_list arg_num go_forward(FUNCTION) nl |
| `FUNCTION` | `function` | function nl local_start arg_num go_forward(FUNCTION) |
| `EXITFUNC` | `exit` `function` | nl |
| `ENDFUNC` | `end` `function` | local_end come_forward(FUNCTION) nl |
| `SUBARG` | `sub` `(` `)` | function local_start local_list arg_num go_forward(SUB) nl |
| `SUB` | `sub` | function nl local_start arg_num go_forward(SUB) |
| `EXITSUB` | `exit` `sub` | nl |
| `ENDSUB` | `end` `sub` | local_end come_forward(SUB) nl |
| `ICALL` | `icall` | expression_list nl |
| `FLET` | `=` | thisfn expression nl |
| `LETM` | `-=` | lval expression nl |
| `LETP` | `+=` | lval expression nl |
| `LETS` | `*=` | lval expression nl |
| `LETD` | `/=` | lval expression nl |
| `LETI` | `\=` | lval expression nl |
| `LETC` | `&=` | lval expression nl |
| `REF` | `ref` `=` | lval lval nl |
| `NLABEL` |  | label_def noexec |
| `GOTO` | `goto` | label nl |
| `GOTO` | `go` `to` | label nl |
| `GOSUB` | `gosub` | label nl |
| `GOSUB` | `go` `sub` | label nl |
| `RETURNC` | `return` | nl |
| `POP` | `pop` | nl |
| `CUNDEF` | `undef` | lval_list nl |
| `CBYVAL` | `byval` | lval_list nl |
| `ONERRORGOTONULL` | `on` `error` `goto` `null` | nl |
| `ONERRORGOTO` | `on` `error` `goto` | label nl |
| `ONERRORRESUMENEXT` | `on` `error` `resume` `next` | nl |
| `ONERRORRESUMELABEL` | `on` `error` `resume` | label nl |
| `RESUME` | `resume` | nl |
| `RESUMENEXT` | `resume` `next` | nl |
| `RESUMELABEL` | `resume` | label nl |
| `CERROR` | `error` | expression nl |
| `WHILE` | `while` | expression go_forward(WHILE) come_back(WHILE) nl |
| `WEND` | `wend` | go_back(WHILE) come_forward(WHILE) nl |
| `DOWHILE` | `do` `while` | expression go_forward(DO) come_back(DO) nl |
| `DOUNTIL` | `do` `until` | expression go_forward(DO) come_back(DO) nl |
| `LOOPWHILE` | `loop` `while` | expression go_back(LOOP) come_forward(LOOP) nl |
| `LOOPUNTIL` | `loop` `until` | expression go_back(LOOP) come_forward(LOOP) nl |
| `UNTIL` | `until` | expression go_back(REPEAT) come_forward(REPEAT) nl |
| `LOOP` | `loop` | go_back(DO) come_forward(DO) nl |
| `REPEAT` | `repeat` | go_forward(REPEAT) come_back(REPEAT) nl |
| `DO` | `do` | go_forward(LOOP) come_back(LOOP) nl |
| `FOR` | `for` `=` | lval expression come_back(FORTO) |
| `FORTO` | `to` | expression go_back(FORTO) go_forward(FOR) come_back(FOR) |
| `FORSTEP` | `step` | expression nl |
| `NEXTI` | `next` | symbol go_back(FOR) come_forward(FOR) nl |
| `NEXT` | `next` | go_back(FOR) come_forward(FOR) nl |
| `ELSIF` | `elseif` `then` | * expression come_forward(IF) go_forward(IF) nl |
| `ELSIF` | `else` `if` `then` | * expression come_forward(IF) go_forward(IF) nl |
| `ELSIF` | `elif` `then` | * expression come_forward(IF) go_forward(IF) nl |
| `ELSIF` | `elsif` `then` | * expression come_forward(IF) go_forward(IF) nl |
| `ELSE` | `else` | * come_forward(IF) go_forward(ELSE) nl |
| `ENDIF` | `endif` | * come_forward(IF,ELSE) nl |
| `ENDIF` | `end` `if` | * come_forward(IF,ELSE) nl |
| `OPENDIR` | `open` `directory` `pattern` `option` `as` | expression expression expression expression nl |
| `RESETDIR` | `reset` `directory` `#` | expression nl |
| `RESETDIR` | `reset` `directory` | expression nl |
| `CLOSEDIR` | `close` `directory` `#` | expression nl |
| `CLOSEDIR` | `close` `directory` | expression nl |
| `CLOSE` | `close` `#` | expression nl |
| `CLOSE` | `close` | expression nl |
| `SEEK` | `seek` `#` `,` | expression expression nl |
| `SEEK` | `seek` `,` | expression expression nl |
| `REWIND` | `rewind` `#` | expression nl |
| `REWIND` | `rewind` | expression nl |
| `RESET` | `reset` | nl |
| `LINPUTF` | `line` `input` `#` `,` | expression lval nl |
| `LINPUT` | `line` `input` | lval nl |
| `TRUNCATEF` | `truncate` `#` `,` | expression expression nl |
| `RLOCK` | `lock` `region` `#` `from` `to` `for` | expression expression expression absolute_symbol nl |
| `RLOCK` | `lock` `region` `from` `to` `for` | expression expression expression absolute_symbol nl |
| `FLOCK` | `lock` `#` `,` | expression absolute_symbol nl |
| `FLOCK` | `lock` `,` | expression absolute_symbol nl |
| `MKDIR` | `mkdir` | expression nl |
| `DELETEF` | `deltree` | expression nl |
| `DELETE` | `delete` | expression nl |
| `FCOPY` | `filecopy` `,` | expression expression nl |
| `NAME` | `name` `,` | expression expression nl |
| `CHDIR` | `chdir` | expression nl |
| `SETFILE` | `set` `file` `=` | expression absolute_symbol expression nl |
| `BINMO` | `binmode` `output` | nl |
| `BINMI` | `binmode` `input` | nl |
| `TXTMO` | `textmode` `output` | nl |
| `TXTMI` | `textmode` `input` | nl |
| `BINMF` | `binmode` `#` | expression nl |
| `BINMF` | `binmode` | expression nl |
| `TXTMF` | `textmode` `#` | expression nl |
| `TXTMF` | `textmode` | expression nl |
| `SPLITAQ` | `splitaq` `by` `quote` `to` | expression expression expression lval nl |
| `SPLITA` | `splita` `by` `to` | expression expression lval nl |
| `SPLIT` | `split` `by` `to` | expression expression lval_list nl |
| `UNPACK` | `unpack` `by` `to` | expression expression lval_list nl |
| `RANDOMIZA` | `randomize` | nl |
| `RANDOMIZE` | `randomize` | expression nl |
| `SETJOKER` | `set` `joker` `to` | expression expression nl |
| `SETWILD` | `set` `wild` `to` | expression expression nl |
| `SETNOJO` | `set` `no` `joker` | expression nl |
| `SETNOJO` | `set` `no` `wild` | expression nl |
| `SLEEP` | `sleep` | expression nl |
| `PAUSE` | `pause` | expression nl |
| `EXTERNAL` | `declare` `sub` `alias` `lib` | * function string string nl |
| `EXTERNAM` | `declare` `command` `alias` `lib` | * function string string nl |

## Fonctions internes (%FUNCTIONS)

| Fonction | Token | Args min/max |
|---|---|---|
| `SIN` | `sin` | 1–1 |
| `ASIN` | `asin` | 1–1 |
| `COS` | `cos` | 1–1 |
| `ACOS` | `acos` | 1–1 |
| `SGN` | `sgn` | 1–1 |
| `ODD` | `odd` | 1–1 |
| `GCD` | `gcd` | 2–100000 |
| `LCM` | `lcm` | 2–100000 |
| `EVEN` | `even` | 1–1 |
| `SQR` | `sqr` | 1–1 |
| `RND` | `rnd` | 0–0 |
| `ABS` | `abs` | 1–1 |
| `VAL` | `val` | 1–1 |
| `PI` | `pi` | 0–0 |
| `MAXINT` | `maxint` | 0–0 |
| `MININT` | `minint` | 0–0 |
| `TRUE` | `true` | 0–0 |
| `FALSE` | `false` | 0–0 |
| `FIX` | `fix` | 1–1 |
| `INT` | `int` | 1–1 |
| `INT` | `cint` | 1–1 |
| `FRAC` | `frac` | 1–1 |
| `CVD` | `cvd` | 1–1 |
| `CVI` | `cvi` | 1–1 |
| `CVL` | `cvl` | 1–1 |
| `CVS` | `cvs` | 1–1 |
| `MKD` | `mkd` | 1–1 |
| `MKD` | `mkd$` | 1–1 |
| `MKI` | `mki` | 1–1 |
| `MKI` | `mki$` | 1–1 |
| `MKS` | `mks` | 1–1 |
| `MKS` | `mks$` | 1–1 |
| `MKL` | `mkl` | 1–1 |
| `MKL` | `mkl$` | 1–1 |
| `UNDEF` | `undef` | 0–0 |
| `ROUND` | `round` | 1–2 |
| `LOG` | `log` | 1–1 |
| `LOG10` | `log10` | 1–1 |
| `POW` | `pow` | 1–1 |
| `EXP` | `exp` | 1–1 |
| `LEN` | `len` | 1–1 |
| `ASC` | `asc` | 1–1 |
| `UCASE` | `ucase` | 1–1 |
| `LCASE` | `lcase` | 1–1 |
| `LTRIM` | `ltrim` | 1–1 |
| `RTRIM` | `rtrim` | 1–1 |
| `TRIM` | `trim` | 1–1 |
| `MID` | `mid` | 2–3 |
| `LEFT` | `left` | 2–2 |
| `RIGHT` | `right` | 2–2 |
| `SPACE` | `space` | 1–1 |
| `STRING` | `string` | 2–2 |
| `CHR` | `chr` | 1–1 |
| `STRREVERSE` | `strreverse` | 1–1 |
| `STR` | `str` | 1–1 |
| `HEX` | `hex` | 1–1 |
| `OCT` | `oct` | 1–1 |
| `JOKER` | `joker` | 1–1 |
| `CHOMP` | `chomp` | 1–1 |
| `LBOUND` | `lbound` | 1–1 |
| `UBOUND` | `ubound` | 1–1 |
| `ISARRAY` | `isarray` | 1–1 |
| `ISSTRING` | `isstring` | 1–1 |
| `ISLONG` | `isinteger` | 1–1 |
| `ISDOUBLE` | `isreal` | 1–1 |
| `ISNUMERIC` | `isnumeric` | 1–1 |
| `ISDEF` | `isdefined` | 1–1 |
| `ISUNDEF` | `isundef` | 1–1 |
| `ISEMPTY` | `isempty` | 1–1 |
| `TYPE` | `type` | 1–1 |
| `ACOSECANT` | `acosecant` | 1–1 |
| `ACTAN` | `actan` | 1–1 |
| `ASECANT` | `asecant` | 1–1 |
| `ATAN` | `atan` | 1–1 |
| `ATN` | `atn` | 1–1 |
| `COSECANT` | `cosecant` | 1–1 |
| `COTAN` | `cotan` | 1–1 |
| `COTAN2` | `cotan2` | 2–2 |
| `HCOS` | `hcos` | 1–1 |
| `HCOSECANT` | `hcosecant` | 1–1 |
| `HCTAN` | `hctan` | 1–1 |
| `HSECANT` | `hsecant` | 1–1 |
| `HSIN` | `hsin` | 1–1 |
| `HTAN` | `htan` | 1–1 |
| `SECANT` | `secant` | 1–1 |
| `TAN` | `tan` | 1–1 |
| `TAN2` | `tan2` | 2–2 |
| `BIN` | `bin` | 1–1 |
| `UCASE` | `ucase$` | 1–1 |
| `UCASE` | `upper` | 1–1 |
| `UCASE` | `upper$` | 1–1 |
| `LCASE` | `lcase$` | 1–1 |
| `LCASE` | `lower` | 1–1 |
| `LCASE` | `lower$` | 1–1 |
| `LTRIM` | `ltrim$` | 1–1 |
| `RTRIM` | `rtrim$` | 1–1 |
| `TRIM` | `trim$` | 1–1 |
| `MID` | `mid$` | 2–3 |
| `LEFT` | `left$` | 2–2 |
| `RIGHT` | `right$` | 2–2 |
| `SPACE` | `space$` | 1–1 |
| `STRING` | `string$` | 2–2 |
| `CHR` | `chr$` | 1–1 |
| `STRREVERSE` | `strreverse$` | 1–1 |
| `STR` | `str$` | 1–1 |
| `HEX` | `hex$` | 1–1 |
| `OCT` | `oct$` | 1–1 |
| `INSTR` | `instr` | 2–3 |
| `INSTRREV` | `instrrev` | 2–3 |
| `REPLACE` | `replace` | 3–5 |
| `JOIN` | `join` | 2–100000 |
| `MAX` | `max` | 1–100000 |
| `MIN` | `min` | 1–100000 |
| `IMAX` | `imax` | 1–100000 |
| `IMIN` | `imin` | 1–100000 |
| `OPTIONF` | `option` | 1–1 |
| `ERROR` | `error` | 0–0 |
| `ERRORDOLLAR` | `error$` | 0–1 |
| `LOC` | `loc` | 1–1 |
| `LOF` | `lof` | 1–1 |
| `FILELEN` | `filelen` | 1–1 |
| `FREEFILE` | `freefile` | 0–1 |
| `INPUTFUN` | `input` | 1–2 |
| `EOFFUN` | `eof` | 1–1 |
| `EODFUN` | `eod` | 1–1 |
| `FTACCESS` | `fileaccesstime` | 1–1 |
| `FTMODIFY` | `filemodifytime` | 1–1 |
| `FTCREATED` | `filecreatetime` | 1–1 |
| `FOWNER` | `fileowner` | 1–1 |
| `ISDIR` | `isdirectory` | 1–1 |
| `ISREG` | `isfile` | 1–1 |
| `FILEXISTS` | `fileexists` | 1–1 |
| `NEXTFILE` | `nextfile` | 1–1 |
| `ENVIRON` | `environ` | 1–1 |
| `ENVIRON` | `environ$` | 1–1 |
| `CURDIR` | `curdir` | 0–0 |
| `COMMANDF` | `command` | 0–0 |
| `ADDRESSF` | `address` | 1–1 |
| `FORMATDATE` | `formatdate` | 1–2 |
| `FORMATDATE` | `formattime` | 1–2 |
| `FORMAT` | `format` | 1–2000000 |
| `PACK` | `pack` | 1–2000000 |
| `NOW` | `time` | 0–0 |
| `NOW` | `now` | 0–0 |
| `GMTIME` | `gmtime` | 0–0 |
| `YEAR` | `year` | 0–1 |
| `MONTH` | `month` | 0–1 |
| `DAY` | `day` | 0–1 |
| `WDAY` | `weekday` | 0–1 |
| `YDAY` | `yearday` | 0–1 |
| `MINUTE` | `minute` | 0–1 |
| `SEC` | `sec` | 0–1 |
| `HOUR` | `hour` | 0–1 |
| `TIMEVALUE` | `timevalue` | 0–6 |
| `GM2LOCAL` | `gmtimetolocaltime` | 1–1 |
| `LOCAL2GM` | `localtimetogmtime` | 1–1 |
| `ADDYEAR` | `addyear` | 2–2 |
| `ADDMONTH` | `addmonth` | 2–2 |
| `ADDDAY` | `addday` | 2–2 |
| `ADDHOUR` | `addhour` | 2–2 |
| `ADDMINUTE` | `addminute` | 2–2 |
| `ADDSECOND` | `addsecond` | 2–2 |
| `ADDWEEK` | `addweek` | 2–2 |
| `HOSTNAME` | `hostname` | 0–0 |
| `CONF` | `conf` | 1–1 |
| `KILL` | `kill` | 1–1 |
| `ICALLFUN` | `icall` | 1–2000000 |
| `FCRYPT` | `crypt` | 2–2 |
| `CREATEPROCESS` | `system` | 1–1 |
| `CREATEPROCESSEX` | `execute` | 3–3 |
| `WAITPID` | `waitpid` | 2–2 |
| `FORK` | `fork` | 0–0 |

## Opérateurs unaires (%UNARIES)

| Nom | Token | Opcode |
|---|---|---|
| `PLUS` | `+` | `` |
| `MINUS` | `-` | `` |
| `NOT` | `not` | `` |
| `BYVAL` | `byval` | `` |
| `EXTOPF` | `?` | `` |
| `EXTOPK` | `!` | `` |
| `EXTOPL` | `#` | `` |
| `EXTOPO` | ``` | `` |
| `EXTOPQ` | `@` | `` |
| `EXTOPAC` | `+^` | `` |
| `EXTOPAD` | `+<` | `` |
| `EXTOPAE` | `+>` | `` |
| `EXTOPAF` | `+?` | `` |
| `EXTOPAG` | `+=` | `` |
| `EXTOPAH` | `+*` | `` |
| `EXTOPAI` | `+/` | `` |
| `EXTOPAJ` | `+%` | `` |
| `EXTOPAK` | `+!` | `` |
| `EXTOPAL` | `+#` | `` |
| `EXTOPAM` | `+&` | `` |
| `EXTOPAN` | `+\` | `` |
| `EXTOPAO` | `+`` | `` |
| `EXTOPAP` | `+'` | `` |
| `EXTOPAQ` | `+@` | `` |
| `EXTOPBC` | `-^` | `` |
| `EXTOPBD` | `-<` | `` |
| `EXTOPBE` | `->` | `` |
| `EXTOPBF` | `-?` | `` |
| `EXTOPBG` | `-=` | `` |
| `EXTOPBH` | `-*` | `` |
| `EXTOPBI` | `-/` | `` |
| `EXTOPBJ` | `-%` | `` |
| `EXTOPBK` | `-!` | `` |
| `EXTOPBL` | `-#` | `` |
| `EXTOPBM` | `-&` | `` |
| `EXTOPBN` | `-\` | `` |
| `EXTOPBO` | `-`` | `` |
| `EXTOPBP` | `-'` | `` |
| `EXTOPBQ` | `-@` | `` |
| `EXTOPCC` | `^^` | `` |
| `EXTOPCD` | `^<` | `` |
| `EXTOPCE` | `^>` | `` |
| `EXTOPCF` | `^?` | `` |
| `EXTOPCG` | `^=` | `` |
| `EXTOPCH` | `^*` | `` |
| `EXTOPCI` | `^/` | `` |
| `EXTOPCJ` | `^%` | `` |
| `EXTOPCK` | `^!` | `` |
| `EXTOPCL` | `^#` | `` |
| `EXTOPCM` | `^&` | `` |
| `EXTOPCN` | `^\` | `` |
| `EXTOPCO` | `^`` | `` |
| `EXTOPCP` | `^'` | `` |
| `EXTOPCQ` | `^@` | `` |
| `EXTOPDC` | `<^` | `` |
| `EXTOPDD` | `<<` | `` |
| `EXTOPDF` | `<?` | `` |
| `EXTOPDH` | `<*` | `` |
| `EXTOPDI` | `</` | `` |
| `EXTOPDJ` | `<%` | `` |
| `EXTOPDK` | `<!` | `` |
| `EXTOPDL` | `<#` | `` |
| `EXTOPDM` | `<&` | `` |
| `EXTOPDN` | `<\` | `` |
| `EXTOPDO` | `<`` | `` |
| `EXTOPDP` | `<'` | `` |
| `EXTOPDQ` | `<@` | `` |
| `EXTOPEC` | `>^` | `` |
| `EXTOPED` | `><` | `` |
| `EXTOPEE` | `>>` | `` |
| `EXTOPEF` | `>?` | `` |
| `EXTOPEH` | `>*` | `` |
| `EXTOPEI` | `>/` | `` |
| `EXTOPEJ` | `>%` | `` |
| `EXTOPEK` | `>!` | `` |
| `EXTOPEL` | `>#` | `` |
| `EXTOPEM` | `>&` | `` |
| `EXTOPEN` | `>\` | `` |
| `EXTOPEO` | `>`` | `` |
| `EXTOPEP` | `>'` | `` |
| `EXTOPEQ` | `>@` | `` |
| `EXTOPFC` | `?^` | `` |
| `EXTOPFD` | `?<` | `` |
| `EXTOPFE` | `?>` | `` |
| `EXTOPFF` | `??` | `` |
| `EXTOPFG` | `?=` | `` |
| `EXTOPFH` | `?*` | `` |
| `EXTOPFI` | `?/` | `` |
| `EXTOPFJ` | `?%` | `` |
| `EXTOPFK` | `?!` | `` |
| `EXTOPFL` | `?#` | `` |
| `EXTOPFM` | `?&` | `` |
| `EXTOPFN` | `?\` | `` |
| `EXTOPFO` | `?`` | `` |
| `EXTOPFP` | `?'` | `` |
| `EXTOPFQ` | `?@` | `` |
| `EXTOPGC` | `=^` | `` |
| `EXTOPGD` | `=<` | `` |
| `EXTOPGE` | `=>` | `` |
| `EXTOPGF` | `=?` | `` |
| `EXTOPGG` | `==` | `` |
| `EXTOPGH` | `=*` | `` |
| `EXTOPGI` | `=/` | `` |
| `EXTOPGJ` | `=%` | `` |
| `EXTOPGK` | `=!` | `` |
| `EXTOPGL` | `=#` | `` |
| `EXTOPGM` | `=&` | `` |
| `EXTOPGN` | `=\` | `` |
| `EXTOPGO` | `=`` | `` |
| `EXTOPGP` | `='` | `` |
| `EXTOPGQ` | `=@` | `` |
| `EXTOPHC` | `*^` | `` |
| `EXTOPHD` | `*<` | `` |
| `EXTOPHE` | `*>` | `` |
| `EXTOPHF` | `*?` | `` |
| `EXTOPHG` | `*=` | `` |
| `EXTOPHH` | `**` | `` |
| `EXTOPHI` | `*/` | `` |
| `EXTOPHJ` | `*%` | `` |
| `EXTOPHK` | `*!` | `` |
| `EXTOPHL` | `*#` | `` |
| `EXTOPHM` | `*&` | `` |
| `EXTOPHN` | `*\` | `` |
| `EXTOPHO` | `*`` | `` |
| `EXTOPHP` | `*'` | `` |
| `EXTOPHQ` | `*@` | `` |
| `EXTOPIC` | `/^` | `` |
| `EXTOPID` | `/<` | `` |
| `EXTOPIE` | `/>` | `` |
| `EXTOPIF` | `/?` | `` |
| `EXTOPIG` | `/=` | `` |
| `EXTOPIH` | `/*` | `` |
| `EXTOPII` | `//` | `` |
| `EXTOPIJ` | `/%` | `` |
| `EXTOPIK` | `/!` | `` |
| `EXTOPIL` | `/#` | `` |
| `EXTOPIM` | `/&` | `` |
| `EXTOPIN` | `/\` | `` |
| `EXTOPIO` | `/`` | `` |
| `EXTOPIP` | `/'` | `` |
| `EXTOPIQ` | `/@` | `` |
| `EXTOPJC` | `%^` | `` |
| `EXTOPJD` | `%<` | `` |
| `EXTOPJE` | `%>` | `` |
| `EXTOPJF` | `%?` | `` |
| `EXTOPJG` | `%=` | `` |
| `EXTOPJH` | `%*` | `` |
| `EXTOPJI` | `%/` | `` |
| `EXTOPJJ` | `%%` | `` |
| `EXTOPJK` | `%!` | `` |
| `EXTOPJL` | `%#` | `` |
| `EXTOPJM` | `%&` | `` |
| `EXTOPJN` | `%\` | `` |
| `EXTOPJO` | `%`` | `` |
| `EXTOPJP` | `%'` | `` |
| `EXTOPJQ` | `%@` | `` |
| `EXTOPKC` | `!^` | `` |
| `EXTOPKD` | `!<` | `` |
| `EXTOPKE` | `!>` | `` |
| `EXTOPKF` | `!?` | `` |
| `EXTOPKG` | `!=` | `` |
| `EXTOPKH` | `!*` | `` |
| `EXTOPKI` | `!/` | `` |
| `EXTOPKJ` | `!%` | `` |
| `EXTOPKK` | `!!` | `` |
| `EXTOPKL` | `!#` | `` |
| `EXTOPKM` | `!&` | `` |
| `EXTOPKN` | `!\` | `` |
| `EXTOPKO` | `!`` | `` |
| `EXTOPKP` | `!'` | `` |
| `EXTOPKQ` | `!@` | `` |
| `EXTOPLC` | `#^` | `` |
| `EXTOPLD` | `#<` | `` |
| `EXTOPLE` | `#>` | `` |
| `EXTOPLF` | `#?` | `` |
| `EXTOPLG` | `#=` | `` |
| `EXTOPLH` | `#*` | `` |
| `EXTOPLI` | `#/` | `` |
| `EXTOPLJ` | `#%` | `` |
| `EXTOPLK` | `#!` | `` |
| `EXTOPLL` | `##` | `` |
| `EXTOPLM` | `#&` | `` |
| `EXTOPLN` | `#\` | `` |
| `EXTOPLO` | `#`` | `` |
| `EXTOPLP` | `#'` | `` |
| `EXTOPLQ` | `#@` | `` |
| `EXTOPMC` | `&^` | `` |
| `EXTOPMD` | `&<` | `` |
| `EXTOPME` | `&>` | `` |
| `EXTOPMF` | `&?` | `` |
| `EXTOPMG` | `&=` | `` |
| `EXTOPMH` | `&*` | `` |
| `EXTOPMI` | `&/` | `` |
| `EXTOPMJ` | `&%` | `` |
| `EXTOPMK` | `&!` | `` |
| `EXTOPML` | `&#` | `` |
| `EXTOPMM` | `&&` | `` |
| `EXTOPMN` | `&\` | `` |
| `EXTOPMO` | `&`` | `` |
| `EXTOPMP` | `&'` | `` |
| `EXTOPMQ` | `&@` | `` |
| `EXTOPNC` | `\^` | `` |
| `EXTOPND` | `\<` | `` |
| `EXTOPNE` | `\>` | `` |
| `EXTOPNF` | `\?` | `` |
| `EXTOPNG` | `\=` | `` |
| `EXTOPNH` | `\*` | `` |
| `EXTOPNI` | `\/` | `` |
| `EXTOPNJ` | `\%` | `` |
| `EXTOPNK` | `\!` | `` |
| `EXTOPNL` | `\#` | `` |
| `EXTOPNM` | `\&` | `` |
| `EXTOPNN` | `\\` | `` |
| `EXTOPNO` | `\`` | `` |
| `EXTOPNP` | `\'` | `` |
| `EXTOPNQ` | `\@` | `` |
| `EXTOPOC` | ``^` | `` |
| `EXTOPOD` | ``<` | `` |
| `EXTOPOE` | ``>` | `` |
| `EXTOPOF` | ``?` | `` |
| `EXTOPOG` | ``=` | `` |
| `EXTOPOH` | ``*` | `` |
| `EXTOPOI` | ``/` | `` |
| `EXTOPOJ` | ``%` | `` |
| `EXTOPOK` | ``!` | `` |
| `EXTOPOL` | ``#` | `` |
| `EXTOPOM` | ``&` | `` |
| `EXTOPON` | ``\` | `` |
| `EXTOPOO` | ```` | `` |
| `EXTOPOP` | ``'` | `` |
| `EXTOPOQ` | ``@` | `` |
| `EXTOPPC` | `'^` | `` |
| `EXTOPPD` | `'<` | `` |
| `EXTOPPE` | `'>` | `` |
| `EXTOPPF` | `'?` | `` |
| `EXTOPPG` | `'=` | `` |
| `EXTOPPH` | `'*` | `` |
| `EXTOPPI` | `'/` | `` |
| `EXTOPPJ` | `'%` | `` |
| `EXTOPPK` | `'!` | `` |
| `EXTOPPL` | `'#` | `` |
| `EXTOPPM` | `'&` | `` |
| `EXTOPPN` | `'\` | `` |
| `EXTOPPO` | `'`` | `` |
| `EXTOPPP` | `''` | `` |
| `EXTOPPQ` | `'@` | `` |
| `EXTOPQC` | `@^` | `` |
| `EXTOPQD` | `@<` | `` |
| `EXTOPQE` | `@>` | `` |
| `EXTOPQF` | `@?` | `` |
| `EXTOPQG` | `@=` | `` |
| `EXTOPQH` | `@*` | `` |
| `EXTOPQI` | `@/` | `` |
| `EXTOPQJ` | `@%` | `` |
| `EXTOPQK` | `@!` | `` |
| `EXTOPQL` | `@#` | `` |
| `EXTOPQM` | `@&` | `` |
| `EXTOPQN` | `@\` | `` |
| `EXTOPQO` | `@`` | `` |
| `EXTOPQP` | `@'` | `` |
| `EXTOPQQ` | `@@` | `` |

## Opérateurs binaires (%BINARIES)

| Nom | Token | Opcode |
|---|---|---|
| `EXTOPF` | `?` | `` |
| `EXTOPK` | `!` | `` |
| `EXTOPL` | `#` | `` |
| `EXTOPO` | ``` | `` |
| `EXTOPQ` | `@` | `` |
| `EXTOPAC` | `+^` | `` |
| `EXTOPAD` | `+<` | `` |
| `EXTOPAE` | `+>` | `` |
| `EXTOPAF` | `+?` | `` |
| `EXTOPAG` | `+=` | `` |
| `EXTOPAH` | `+*` | `` |
| `EXTOPAI` | `+/` | `` |
| `EXTOPAJ` | `+%` | `` |
| `EXTOPAK` | `+!` | `` |
| `EXTOPAL` | `+#` | `` |
| `EXTOPAM` | `+&` | `` |
| `EXTOPAN` | `+\` | `` |
| `EXTOPAO` | `+`` | `` |
| `EXTOPAP` | `+'` | `` |
| `EXTOPAQ` | `+@` | `` |
| `EXTOPBC` | `-^` | `` |
| `EXTOPBD` | `-<` | `` |
| `EXTOPBE` | `->` | `` |
| `EXTOPBF` | `-?` | `` |
| `EXTOPBG` | `-=` | `` |
| `EXTOPBH` | `-*` | `` |
| `EXTOPBI` | `-/` | `` |
| `EXTOPBJ` | `-%` | `` |
| `EXTOPBK` | `-!` | `` |
| `EXTOPBL` | `-#` | `` |
| `EXTOPBM` | `-&` | `` |
| `EXTOPBN` | `-\` | `` |
| `EXTOPBO` | `-`` | `` |
| `EXTOPBP` | `-'` | `` |
| `EXTOPBQ` | `-@` | `` |
| `EXTOPCC` | `^^` | `` |
| `EXTOPCD` | `^<` | `` |
| `EXTOPCE` | `^>` | `` |
| `EXTOPCF` | `^?` | `` |
| `EXTOPCG` | `^=` | `` |
| `EXTOPCH` | `^*` | `` |
| `EXTOPCI` | `^/` | `` |
| `EXTOPCJ` | `^%` | `` |
| `EXTOPCK` | `^!` | `` |
| `EXTOPCL` | `^#` | `` |
| `EXTOPCM` | `^&` | `` |
| `EXTOPCN` | `^\` | `` |
| `EXTOPCO` | `^`` | `` |
| `EXTOPCP` | `^'` | `` |
| `EXTOPCQ` | `^@` | `` |
| `EXTOPDC` | `<^` | `` |
| `EXTOPDD` | `<<` | `` |
| `EXTOPDF` | `<?` | `` |
| `EXTOPDH` | `<*` | `` |
| `EXTOPDI` | `</` | `` |
| `EXTOPDJ` | `<%` | `` |
| `EXTOPDK` | `<!` | `` |
| `EXTOPDL` | `<#` | `` |
| `EXTOPDM` | `<&` | `` |
| `EXTOPDN` | `<\` | `` |
| `EXTOPDO` | `<`` | `` |
| `EXTOPDP` | `<'` | `` |
| `EXTOPDQ` | `<@` | `` |
| `EXTOPEC` | `>^` | `` |
| `EXTOPED` | `><` | `` |
| `EXTOPEE` | `>>` | `` |
| `EXTOPEF` | `>?` | `` |
| `EXTOPEH` | `>*` | `` |
| `EXTOPEI` | `>/` | `` |
| `EXTOPEJ` | `>%` | `` |
| `EXTOPEK` | `>!` | `` |
| `EXTOPEL` | `>#` | `` |
| `EXTOPEM` | `>&` | `` |
| `EXTOPEN` | `>\` | `` |
| `EXTOPEO` | `>`` | `` |
| `EXTOPEP` | `>'` | `` |
| `EXTOPEQ` | `>@` | `` |
| `EXTOPFC` | `?^` | `` |
| `EXTOPFD` | `?<` | `` |
| `EXTOPFE` | `?>` | `` |
| `EXTOPFF` | `??` | `` |
| `EXTOPFG` | `?=` | `` |
| `EXTOPFH` | `?*` | `` |
| `EXTOPFI` | `?/` | `` |
| `EXTOPFJ` | `?%` | `` |
| `EXTOPFK` | `?!` | `` |
| `EXTOPFL` | `?#` | `` |
| `EXTOPFM` | `?&` | `` |
| `EXTOPFN` | `?\` | `` |
| `EXTOPFO` | `?`` | `` |
| `EXTOPFP` | `?'` | `` |
| `EXTOPFQ` | `?@` | `` |
| `EXTOPGC` | `=^` | `` |
| `EXTOPGD` | `=<` | `` |
| `EXTOPGE` | `=>` | `` |
| `EXTOPGF` | `=?` | `` |
| `EXTOPGG` | `==` | `` |
| `EXTOPGH` | `=*` | `` |
| `EXTOPGI` | `=/` | `` |
| `EXTOPGJ` | `=%` | `` |
| `EXTOPGK` | `=!` | `` |
| `EXTOPGL` | `=#` | `` |
| `EXTOPGM` | `=&` | `` |
| `EXTOPGN` | `=\` | `` |
| `EXTOPGO` | `=`` | `` |
| `EXTOPGP` | `='` | `` |
| `EXTOPGQ` | `=@` | `` |
| `EXTOPHC` | `*^` | `` |
| `EXTOPHD` | `*<` | `` |
| `EXTOPHE` | `*>` | `` |
| `EXTOPHF` | `*?` | `` |
| `EXTOPHG` | `*=` | `` |
| `EXTOPHH` | `**` | `` |
| `EXTOPHI` | `*/` | `` |
| `EXTOPHJ` | `*%` | `` |
| `EXTOPHK` | `*!` | `` |
| `EXTOPHL` | `*#` | `` |
| `EXTOPHM` | `*&` | `` |
| `EXTOPHN` | `*\` | `` |
| `EXTOPHO` | `*`` | `` |
| `EXTOPHP` | `*'` | `` |
| `EXTOPHQ` | `*@` | `` |
| `EXTOPIC` | `/^` | `` |
| `EXTOPID` | `/<` | `` |
| `EXTOPIE` | `/>` | `` |
| `EXTOPIF` | `/?` | `` |
| `EXTOPIG` | `/=` | `` |
| `EXTOPIH` | `/*` | `` |
| `EXTOPII` | `//` | `` |
| `EXTOPIJ` | `/%` | `` |
| `EXTOPIK` | `/!` | `` |
| `EXTOPIL` | `/#` | `` |
| `EXTOPIM` | `/&` | `` |
| `EXTOPIN` | `/\` | `` |
| `EXTOPIO` | `/`` | `` |
| `EXTOPIP` | `/'` | `` |
| `EXTOPIQ` | `/@` | `` |
| `EXTOPJC` | `%^` | `` |
| `EXTOPJD` | `%<` | `` |
| `EXTOPJE` | `%>` | `` |
| `EXTOPJF` | `%?` | `` |
| `EXTOPJG` | `%=` | `` |
| `EXTOPJH` | `%*` | `` |
| `EXTOPJI` | `%/` | `` |
| `EXTOPJJ` | `%%` | `` |
| `EXTOPJK` | `%!` | `` |
| `EXTOPJL` | `%#` | `` |
| `EXTOPJM` | `%&` | `` |
| `EXTOPJN` | `%\` | `` |
| `EXTOPJO` | `%`` | `` |
| `EXTOPJP` | `%'` | `` |
| `EXTOPJQ` | `%@` | `` |
| `EXTOPKC` | `!^` | `` |
| `EXTOPKD` | `!<` | `` |
| `EXTOPKE` | `!>` | `` |
| `EXTOPKF` | `!?` | `` |
| `EXTOPKG` | `!=` | `` |
| `EXTOPKH` | `!*` | `` |
| `EXTOPKI` | `!/` | `` |
| `EXTOPKJ` | `!%` | `` |
| `EXTOPKK` | `!!` | `` |
| `EXTOPKL` | `!#` | `` |
| `EXTOPKM` | `!&` | `` |
| `EXTOPKN` | `!\` | `` |
| `EXTOPKO` | `!`` | `` |
| `EXTOPKP` | `!'` | `` |
| `EXTOPKQ` | `!@` | `` |
| `EXTOPLC` | `#^` | `` |
| `EXTOPLD` | `#<` | `` |
| `EXTOPLE` | `#>` | `` |
| `EXTOPLF` | `#?` | `` |
| `EXTOPLG` | `#=` | `` |
| `EXTOPLH` | `#*` | `` |
| `EXTOPLI` | `#/` | `` |
| `EXTOPLJ` | `#%` | `` |
| `EXTOPLK` | `#!` | `` |
| `EXTOPLL` | `##` | `` |
| `EXTOPLM` | `#&` | `` |
| `EXTOPLN` | `#\` | `` |
| `EXTOPLO` | `#`` | `` |
| `EXTOPLP` | `#'` | `` |
| `EXTOPLQ` | `#@` | `` |
| `EXTOPMC` | `&^` | `` |
| `EXTOPMD` | `&<` | `` |
| `EXTOPME` | `&>` | `` |
| `EXTOPMF` | `&?` | `` |
| `EXTOPMG` | `&=` | `` |
| `EXTOPMH` | `&*` | `` |
| `EXTOPMI` | `&/` | `` |
| `EXTOPMJ` | `&%` | `` |
| `EXTOPMK` | `&!` | `` |
| `EXTOPML` | `&#` | `` |
| `EXTOPMM` | `&&` | `` |
| `EXTOPMN` | `&\` | `` |
| `EXTOPMO` | `&`` | `` |
| `EXTOPMP` | `&'` | `` |
| `EXTOPMQ` | `&@` | `` |
| `EXTOPNC` | `\^` | `` |
| `EXTOPND` | `\<` | `` |
| `EXTOPNE` | `\>` | `` |
| `EXTOPNF` | `\?` | `` |
| `EXTOPNG` | `\=` | `` |
| `EXTOPNH` | `\*` | `` |
| `EXTOPNI` | `\/` | `` |
| `EXTOPNJ` | `\%` | `` |
| `EXTOPNK` | `\!` | `` |
| `EXTOPNL` | `\#` | `` |
| `EXTOPNM` | `\&` | `` |
| `EXTOPNN` | `\\` | `` |
| `EXTOPNO` | `\`` | `` |
| `EXTOPNP` | `\'` | `` |
| `EXTOPNQ` | `\@` | `` |
| `EXTOPOC` | ``^` | `` |
| `EXTOPOD` | ``<` | `` |
| `EXTOPOE` | ``>` | `` |
| `EXTOPOF` | ``?` | `` |
| `EXTOPOG` | ``=` | `` |
| `EXTOPOH` | ``*` | `` |
| `EXTOPOI` | ``/` | `` |
| `EXTOPOJ` | ``%` | `` |
| `EXTOPOK` | ``!` | `` |
| `EXTOPOL` | ``#` | `` |
| `EXTOPOM` | ``&` | `` |
| `EXTOPON` | ``\` | `` |
| `EXTOPOO` | ```` | `` |
| `EXTOPOP` | ``'` | `` |
| `EXTOPOQ` | ``@` | `` |
| `EXTOPPC` | `'^` | `` |
| `EXTOPPD` | `'<` | `` |
| `EXTOPPE` | `'>` | `` |
| `EXTOPPF` | `'?` | `` |
| `EXTOPPG` | `'=` | `` |
| `EXTOPPH` | `'*` | `` |
| `EXTOPPI` | `'/` | `` |
| `EXTOPPJ` | `'%` | `` |
| `EXTOPPK` | `'!` | `` |
| `EXTOPPL` | `'#` | `` |
| `EXTOPPM` | `'&` | `` |
| `EXTOPPN` | `'\` | `` |
| `EXTOPPO` | `'`` | `` |
| `EXTOPPP` | `''` | `` |
| `EXTOPPQ` | `'@` | `` |
| `EXTOPQC` | `@^` | `` |
| `EXTOPQD` | `@<` | `` |
| `EXTOPQE` | `@>` | `` |
| `EXTOPQF` | `@?` | `` |
| `EXTOPQG` | `@=` | `` |
| `EXTOPQH` | `@*` | `` |
| `EXTOPQI` | `@/` | `` |
| `EXTOPQJ` | `@%` | `` |
| `EXTOPQK` | `@!` | `` |
| `EXTOPQL` | `@#` | `` |
| `EXTOPQM` | `@&` | `` |
| `EXTOPQN` | `@\` | `` |
| `EXTOPQO` | `@`` | `` |
| `EXTOPQP` | `@'` | `` |
| `EXTOPQQ` | `@@` | `` |

## Précédences (%PRECEDENCE)

```
POWER: ^
MULT: *
DIV: /
IDIV: \
MOD: %
PLUS: +
MINUS: -
CONCATENATE: &
LIKEOP: like
EQ: =
LT: <
LE: <=
GT: >
GE: >=
NE: <>
AND: and
OR: or
XOR: xor
```
