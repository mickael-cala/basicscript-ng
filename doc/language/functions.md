# Référence des fonctions & commandes intégrées BasicScript / ScriptBasic 2.0

> Généré depuis `old/syntax.def` : **500** identifiants hors mots-clés de contrôle, regroupés par domaine. Les implémentations de référence sont dans `old/commands/`, `old/basext.c` et `old/extensions/<module>/`.
> ⚠️ Document de travail : les descriptions marquées *(à compléter)* doivent être rédigées d'après `old/texi/` et vérifiées contre le moteur réel avant d'être considérées normatives.

Voir aussi : [keywords.md](keywords.md) (structure du langage), [modules.md](modules.md) (extensions), [errors.md](errors.md).


## Math

| Nom | Description | Suffixe `$` usuel |
|-----|-------------|-------------------|
| `ABS` | ABS(x) — valeur absolue | |
| `ACOS` | *(à compléter)* | |
| `ACOSECANT` | *(à compléter)* | |
| `ACTAN` | *(à compléter)* | |
| `ASIN` | *(à compléter)* | |
| `ATAN` | *(à compléter)* | |
| `ATN` | ATN(x) — arc tangente | |
| `COS` | COS(x) | |
| `COSECANT` | *(à compléter)* | |
| `COTAN` | *(à compléter)* | |
| `DIV` | x DIV y — division entière | |
| `EVEN` | *(à compléter)* | |
| `EXP` | EXP(x) — exponentielle | |
| `FIX` | FIX(x) — troncature | |
| `INT` | INT(x) — partie entière | |
| `LOG` | LOG(x) — ln | |
| `MAX` | MAX(a,b) | |
| `MIN` | MIN(a,b) | |
| `MOD` | x MOD y — modulo | |
| `ODD` | *(à compléter)* | |
| `PI` | PI — constante π | |
| `RANDOMIZE` | RANDOMIZE — reseed | |
| `RND` | RND — aléatoire | |
| `ROUND` | *(à compléter)* | |
| `SECANT` | *(à compléter)* | |
| `SGN` | SGN(x) — signe | |
| `SIN` | SIN(x) | |
| `SQR` | SQR(x) — racine carrée | |
| `TAN` | TAN(x) | |


## Date/Heure

| Nom | Description | Suffixe `$` usuel |
|-----|-------------|-------------------|
| `ADDDAY` | *(à compléter)* | |
| `ADDHOUR` | *(à compléter)* | |
| `ADDMINUTE` | *(à compléter)* | |
| `ADDMONTH` | *(à compléter)* | |
| `ADDSECOND` | *(à compléter)* | |
| `ADDWEEK` | *(à compléter)* | |
| `ADDYEAR` | *(à compléter)* | |
| `DAY` | DAY(d) — jour du mois | |
| `HOUR` | HOUR(t) | |
| `MINUTE` | MINUTE(t) | |
| `MONTH` | MONTH(d) | |
| `NOW` | NOW — date/heure courantes | |
| `TIMEVALUE` | *(à compléter)* | |
| `YEAR` | YEAR(d) | |


## Bas niveau / FFI

| Nom | Description | Suffixe `$` usuel |
|-----|-------------|-------------------|
| `ADDRESSF` | *(à compléter)* | |
| `COMMANDF` | *(à compléter)* | |


## Autres / modules

| Nom | Description | Suffixe `$` usuel |
|-----|-------------|-------------------|
| `AND` | *(à compléter)* | |
| `ASECANT` | *(à compléter)* | |
| `CERROR` | *(à compléter)* | |
| `COTAN2` | *(à compléter)* | |
| `CURDIR` | *(à compléter)* | |
| `DELETE` | *(à compléter)* | |
| `DELETEF` | *(à compléter)* | |
| `EMPTY` | *(à compléter)* | |
| `EODFUN` | *(à compléter)* | |
| `EOFFUN` | *(à compléter)* | |
| `EQ` | *(à compléter)* | |
| `ERROR` | *(à compléter)* | |
| `ERRORDOLLAR` | *(à compléter)* | |
| `EXTERNAL` | *(à compléter)* | |
| `EXTERNAM` | *(à compléter)* | |
| `EXTOPAC` | *(à compléter)* | |
| `EXTOPAD` | *(à compléter)* | |
| `EXTOPAE` | *(à compléter)* | |
| `EXTOPAF` | *(à compléter)* | |
| `EXTOPAG` | *(à compléter)* | |
| `EXTOPAH` | *(à compléter)* | |
| `EXTOPAI` | *(à compléter)* | |
| `EXTOPAJ` | *(à compléter)* | |
| `EXTOPAK` | *(à compléter)* | |
| `EXTOPAL` | *(à compléter)* | |
| `EXTOPAM` | *(à compléter)* | |
| `EXTOPAN` | *(à compléter)* | |
| `EXTOPAO` | *(à compléter)* | |
| `EXTOPAP` | *(à compléter)* | |
| `EXTOPAQ` | *(à compléter)* | |
| `EXTOPBC` | *(à compléter)* | |
| `EXTOPBD` | *(à compléter)* | |
| `EXTOPBE` | *(à compléter)* | |
| `EXTOPBF` | *(à compléter)* | |
| `EXTOPBG` | *(à compléter)* | |
| `EXTOPBH` | *(à compléter)* | |
| `EXTOPBI` | *(à compléter)* | |
| `EXTOPBJ` | *(à compléter)* | |
| `EXTOPBK` | *(à compléter)* | |
| `EXTOPBL` | *(à compléter)* | |
| `EXTOPBM` | *(à compléter)* | |
| `EXTOPBN` | *(à compléter)* | |
| `EXTOPBO` | *(à compléter)* | |
| `EXTOPBP` | *(à compléter)* | |
| `EXTOPBQ` | *(à compléter)* | |
| `EXTOPCC` | *(à compléter)* | |
| `EXTOPCD` | *(à compléter)* | |
| `EXTOPCE` | *(à compléter)* | |
| `EXTOPCF` | *(à compléter)* | |
| `EXTOPCG` | *(à compléter)* | |
| `EXTOPCH` | *(à compléter)* | |
| `EXTOPCI` | *(à compléter)* | |
| `EXTOPCJ` | *(à compléter)* | |
| `EXTOPCK` | *(à compléter)* | |
| `EXTOPCL` | *(à compléter)* | |
| `EXTOPCM` | *(à compléter)* | |
| `EXTOPCN` | *(à compléter)* | |
| `EXTOPCO` | *(à compléter)* | |
| `EXTOPCP` | *(à compléter)* | |
| `EXTOPCQ` | *(à compléter)* | |
| `EXTOPDC` | *(à compléter)* | |
| `EXTOPDD` | *(à compléter)* | |
| `EXTOPDF` | *(à compléter)* | |
| `EXTOPDH` | *(à compléter)* | |
| `EXTOPDI` | *(à compléter)* | |
| `EXTOPDJ` | *(à compléter)* | |
| `EXTOPDK` | *(à compléter)* | |
| `EXTOPDL` | *(à compléter)* | |
| `EXTOPDM` | *(à compléter)* | |
| `EXTOPDN` | *(à compléter)* | |
| `EXTOPDO` | *(à compléter)* | |
| `EXTOPDP` | *(à compléter)* | |
| `EXTOPDQ` | *(à compléter)* | |
| `EXTOPEC` | *(à compléter)* | |
| `EXTOPED` | *(à compléter)* | |
| `EXTOPEE` | *(à compléter)* | |
| `EXTOPEF` | *(à compléter)* | |
| `EXTOPEH` | *(à compléter)* | |
| `EXTOPEI` | *(à compléter)* | |
| `EXTOPEJ` | *(à compléter)* | |
| `EXTOPEK` | *(à compléter)* | |
| `EXTOPEL` | *(à compléter)* | |
| `EXTOPEM` | *(à compléter)* | |
| `EXTOPEN` | *(à compléter)* | |
| `EXTOPEO` | *(à compléter)* | |
| `EXTOPEP` | *(à compléter)* | |
| `EXTOPEQ` | *(à compléter)* | |
| `EXTOPF` | *(à compléter)* | |
| `EXTOPFC` | *(à compléter)* | |
| `EXTOPFD` | *(à compléter)* | |
| `EXTOPFE` | *(à compléter)* | |
| `EXTOPFF` | *(à compléter)* | |
| `EXTOPFG` | *(à compléter)* | |
| `EXTOPFH` | *(à compléter)* | |
| `EXTOPFI` | *(à compléter)* | |
| `EXTOPFJ` | *(à compléter)* | |
| `EXTOPFK` | *(à compléter)* | |
| `EXTOPFL` | *(à compléter)* | |
| `EXTOPFM` | *(à compléter)* | |
| `EXTOPFN` | *(à compléter)* | |
| `EXTOPFO` | *(à compléter)* | |
| `EXTOPFP` | *(à compléter)* | |
| `EXTOPFQ` | *(à compléter)* | |
| `EXTOPGC` | *(à compléter)* | |
| `EXTOPGD` | *(à compléter)* | |
| `EXTOPGE` | *(à compléter)* | |
| `EXTOPGF` | *(à compléter)* | |
| `EXTOPGG` | *(à compléter)* | |
| `EXTOPGH` | *(à compléter)* | |
| `EXTOPGI` | *(à compléter)* | |
| `EXTOPGJ` | *(à compléter)* | |
| `EXTOPGK` | *(à compléter)* | |
| `EXTOPGL` | *(à compléter)* | |
| `EXTOPGM` | *(à compléter)* | |
| `EXTOPGN` | *(à compléter)* | |
| `EXTOPGO` | *(à compléter)* | |
| `EXTOPGP` | *(à compléter)* | |
| `EXTOPGQ` | *(à compléter)* | |
| `EXTOPHC` | *(à compléter)* | |
| `EXTOPHD` | *(à compléter)* | |
| `EXTOPHE` | *(à compléter)* | |
| `EXTOPHF` | *(à compléter)* | |
| `EXTOPHG` | *(à compléter)* | |
| `EXTOPHH` | *(à compléter)* | |
| `EXTOPHI` | *(à compléter)* | |
| `EXTOPHJ` | *(à compléter)* | |
| `EXTOPHK` | *(à compléter)* | |
| `EXTOPHL` | *(à compléter)* | |
| `EXTOPHM` | *(à compléter)* | |
| `EXTOPHN` | *(à compléter)* | |
| `EXTOPHO` | *(à compléter)* | |
| `EXTOPHP` | *(à compléter)* | |
| `EXTOPHQ` | *(à compléter)* | |
| `EXTOPIC` | *(à compléter)* | |
| `EXTOPID` | *(à compléter)* | |
| `EXTOPIE` | *(à compléter)* | |
| `EXTOPIF` | *(à compléter)* | |
| `EXTOPIG` | *(à compléter)* | |
| `EXTOPIH` | *(à compléter)* | |
| `EXTOPII` | *(à compléter)* | |
| `EXTOPIJ` | *(à compléter)* | |
| `EXTOPIK` | *(à compléter)* | |
| `EXTOPIL` | *(à compléter)* | |
| `EXTOPIM` | *(à compléter)* | |
| `EXTOPIN` | *(à compléter)* | |
| `EXTOPIO` | *(à compléter)* | |
| `EXTOPIP` | *(à compléter)* | |
| `EXTOPIQ` | *(à compléter)* | |
| `EXTOPJC` | *(à compléter)* | |
| `EXTOPJD` | *(à compléter)* | |
| `EXTOPJE` | *(à compléter)* | |
| `EXTOPJF` | *(à compléter)* | |
| `EXTOPJG` | *(à compléter)* | |
| `EXTOPJH` | *(à compléter)* | |
| `EXTOPJI` | *(à compléter)* | |
| `EXTOPJJ` | *(à compléter)* | |
| `EXTOPJK` | *(à compléter)* | |
| `EXTOPJL` | *(à compléter)* | |
| `EXTOPJM` | *(à compléter)* | |
| `EXTOPJN` | *(à compléter)* | |
| `EXTOPJO` | *(à compléter)* | |
| `EXTOPJP` | *(à compléter)* | |
| `EXTOPJQ` | *(à compléter)* | |
| `EXTOPK` | *(à compléter)* | |
| `EXTOPKC` | *(à compléter)* | |
| `EXTOPKD` | *(à compléter)* | |
| `EXTOPKE` | *(à compléter)* | |
| `EXTOPKF` | *(à compléter)* | |
| `EXTOPKG` | *(à compléter)* | |
| `EXTOPKH` | *(à compléter)* | |
| `EXTOPKI` | *(à compléter)* | |
| `EXTOPKJ` | *(à compléter)* | |
| `EXTOPKK` | *(à compléter)* | |
| `EXTOPKL` | *(à compléter)* | |
| `EXTOPKM` | *(à compléter)* | |
| `EXTOPKN` | *(à compléter)* | |
| `EXTOPKO` | *(à compléter)* | |
| `EXTOPKP` | *(à compléter)* | |
| `EXTOPKQ` | *(à compléter)* | |
| `EXTOPL` | *(à compléter)* | |
| `EXTOPLC` | *(à compléter)* | |
| `EXTOPLD` | *(à compléter)* | |
| `EXTOPLE` | *(à compléter)* | |
| `EXTOPLF` | *(à compléter)* | |
| `EXTOPLG` | *(à compléter)* | |
| `EXTOPLH` | *(à compléter)* | |
| `EXTOPLI` | *(à compléter)* | |
| `EXTOPLJ` | *(à compléter)* | |
| `EXTOPLK` | *(à compléter)* | |
| `EXTOPLL` | *(à compléter)* | |
| `EXTOPLM` | *(à compléter)* | |
| `EXTOPLN` | *(à compléter)* | |
| `EXTOPLO` | *(à compléter)* | |
| `EXTOPLP` | *(à compléter)* | |
| `EXTOPLQ` | *(à compléter)* | |
| `EXTOPMC` | *(à compléter)* | |
| `EXTOPMD` | *(à compléter)* | |
| `EXTOPME` | *(à compléter)* | |
| `EXTOPMF` | *(à compléter)* | |
| `EXTOPMG` | *(à compléter)* | |
| `EXTOPMH` | *(à compléter)* | |
| `EXTOPMI` | *(à compléter)* | |
| `EXTOPMJ` | *(à compléter)* | |
| `EXTOPMK` | *(à compléter)* | |
| `EXTOPML` | *(à compléter)* | |
| `EXTOPMM` | *(à compléter)* | |
| `EXTOPMN` | *(à compléter)* | |
| `EXTOPMO` | *(à compléter)* | |
| `EXTOPMP` | *(à compléter)* | |
| `EXTOPMQ` | *(à compléter)* | |
| `EXTOPNC` | *(à compléter)* | |
| `EXTOPND` | *(à compléter)* | |
| `EXTOPNE` | *(à compléter)* | |
| `EXTOPNF` | *(à compléter)* | |
| `EXTOPNG` | *(à compléter)* | |
| `EXTOPNH` | *(à compléter)* | |
| `EXTOPNI` | *(à compléter)* | |
| `EXTOPNJ` | *(à compléter)* | |
| `EXTOPNK` | *(à compléter)* | |
| `EXTOPNL` | *(à compléter)* | |
| `EXTOPNM` | *(à compléter)* | |
| `EXTOPNN` | *(à compléter)* | |
| `EXTOPNO` | *(à compléter)* | |
| `EXTOPNP` | *(à compléter)* | |
| `EXTOPNQ` | *(à compléter)* | |
| `EXTOPO` | *(à compléter)* | |
| `EXTOPOC` | *(à compléter)* | |
| `EXTOPOD` | *(à compléter)* | |
| `EXTOPOE` | *(à compléter)* | |
| `EXTOPOF` | *(à compléter)* | |
| `EXTOPOG` | *(à compléter)* | |
| `EXTOPOH` | *(à compléter)* | |
| `EXTOPOI` | *(à compléter)* | |
| `EXTOPOJ` | *(à compléter)* | |
| `EXTOPOK` | *(à compléter)* | |
| `EXTOPOL` | *(à compléter)* | |
| `EXTOPOM` | *(à compléter)* | |
| `EXTOPON` | *(à compléter)* | |
| `EXTOPOO` | *(à compléter)* | |
| `EXTOPOP` | *(à compléter)* | |
| `EXTOPOQ` | *(à compléter)* | |
| `EXTOPPC` | *(à compléter)* | |
| `EXTOPPD` | *(à compléter)* | |
| `EXTOPPE` | *(à compléter)* | |
| `EXTOPPF` | *(à compléter)* | |
| `EXTOPPG` | *(à compléter)* | |
| `EXTOPPH` | *(à compléter)* | |
| `EXTOPPI` | *(à compléter)* | |
| `EXTOPPJ` | *(à compléter)* | |
| `EXTOPPK` | *(à compléter)* | |
| `EXTOPPL` | *(à compléter)* | |
| `EXTOPPM` | *(à compléter)* | |
| `EXTOPPN` | *(à compléter)* | |
| `EXTOPPO` | *(à compléter)* | |
| `EXTOPPP` | *(à compléter)* | |
| `EXTOPPQ` | *(à compléter)* | |
| `EXTOPQ` | *(à compléter)* | |
| `EXTOPQC` | *(à compléter)* | |
| `EXTOPQD` | *(à compléter)* | |
| `EXTOPQE` | *(à compléter)* | |
| `EXTOPQF` | *(à compléter)* | |
| `EXTOPQG` | *(à compléter)* | |
| `EXTOPQH` | *(à compléter)* | |
| `EXTOPQI` | *(à compléter)* | |
| `EXTOPQJ` | *(à compléter)* | |
| `EXTOPQK` | *(à compléter)* | |
| `EXTOPQL` | *(à compléter)* | |
| `EXTOPQM` | *(à compléter)* | |
| `EXTOPQN` | *(à compléter)* | |
| `EXTOPQO` | *(à compléter)* | |
| `EXTOPQP` | *(à compléter)* | |
| `EXTOPQQ` | *(à compléter)* | |
| `FALSE` | *(à compléter)* | |
| `FCOPY` | *(à compléter)* | |
| `FCRYPT` | *(à compléter)* | |
| `FILEXISTS` | *(à compléter)* | |
| `FLET` | *(à compléter)* | |
| `FLOCK` | *(à compléter)* | |
| `FORK` | *(à compléter)* | |
| `FORSTEP` | *(à compléter)* | |
| `FORTO` | *(à compléter)* | |
| `FOWNER` | *(à compléter)* | |
| `FPRINT` | *(à compléter)* | |
| `FPRINTNL` | *(à compléter)* | |
| `FRAC` | *(à compléter)* | |
| `FTACCESS` | *(à compléter)* | |
| `FTCREATED` | *(à compléter)* | |
| `FTMODIFY` | *(à compléter)* | |
| `FUNCTIONARG` | *(à compléter)* | |
| `GCD` | *(à compléter)* | |
| `GCONST` | *(à compléter)* | |
| `GE` | *(à compléter)* | |
| `GM2LOCAL` | *(à compléter)* | |
| `GMTIME` | *(à compléter)* | |
| `GT` | *(à compléter)* | |
| `HCOS` | *(à compléter)* | |
| `HCOSECANT` | *(à compléter)* | |
| `HCTAN` | *(à compléter)* | |
| `HOSTNAME` | *(à compléter)* | |
| `HSECANT` | *(à compléter)* | |
| `HSIN` | *(à compléter)* | |
| `HTAN` | *(à compléter)* | |
| `ICALL` | *(à compléter)* | |
| `ICALLFUN` | *(à compléter)* | |
| `IDIV` | *(à compléter)* | |
| `IMAX` | *(à compléter)* | |
| `IMIN` | *(à compléter)* | |
| `ISARRAY` | *(à compléter)* | |
| `ISDEF` | *(à compléter)* | |
| `ISDIR` | *(à compléter)* | |
| `ISDOUBLE` | *(à compléter)* | |
| `ISEMPTY` | *(à compléter)* | |
| `ISLONG` | *(à compléter)* | |
| `ISNUMERIC` | *(à compléter)* | |
| `ISREG` | *(à compléter)* | |
| `ISSTRING` | *(à compléter)* | |
| `ISUNDEF` | *(à compléter)* | |
| `JOKER` | *(à compléter)* | |
| `LBOUND` | *(à compléter)* | |
| `LCM` | *(à compléter)* | |
| `LE` | *(à compléter)* | |
| `LEN` | LEN(s) — longueur | |
| `LETC` | *(à compléter)* | |
| `LETD` | *(à compléter)* | |
| `LETI` | *(à compléter)* | |
| `LETM` | *(à compléter)* | |
| `LETP` | *(à compléter)* | |
| `LETS` | *(à compléter)* | |
| `LIKEOP` | *(à compléter)* | |
| `LINPUT` | *(à compléter)* | |
| `LINPUTF` | *(à compléter)* | |
| `LLET` | *(à compléter)* | |
| `LOC` | *(à compléter)* | |
| `LOCAL2GM` | *(à compléter)* | |
| `LOG10` | *(à compléter)* | |
| `LOOPUNTIL` | *(à compléter)* | |
| `LOOPWHILE` | *(à compléter)* | |
| `LT` | *(à compléter)* | |
| `MAXINT` | *(à compléter)* | |
| `MININT` | *(à compléter)* | |
| `MINUS` | *(à compléter)* | |
| `MULT` | *(à compléter)* | |
| `NE` | *(à compléter)* | |
| `NEXTFILE` | *(à compléter)* | |
| `NEXTI` | *(à compléter)* | |
| `NLABEL` | *(à compléter)* | |
| `NOT` | *(à compléter)* | |
| `OR` | *(à compléter)* | |
| `PACK` | *(à compléter)* | |
| `PAUSE` | *(à compléter)* | |
| `PLUS` | *(à compléter)* | |
| `POP` | *(à compléter)* | |
| `POW` | *(à compléter)* | |
| `POWER` | *(à compléter)* | |
| `PRAGMA` | *(à compléter)* | |
| `RANDOMIZA` | *(à compléter)* | |
| `REF` | *(à compléter)* | |
| `RESETDIR` | *(à compléter)* | |
| `RESUMELABEL` | *(à compléter)* | |
| `RESUMENEXT` | *(à compléter)* | |
| `RETURNC` | *(à compléter)* | |
| `REWIND` | *(à compléter)* | |
| `RLOCK` | *(à compléter)* | |
| `SEC` | *(à compléter)* | |
| `SETFILE` | *(à compléter)* | |
| `SETJOKER` | *(à compléter)* | |
| `SETNOJO` | *(à compléter)* | |
| `SETWILD` | *(à compléter)* | |
| `SLEEP` | *(à compléter)* | |
| `SLIF` | *(à compléter)* | |
| `STOP` | *(à compléter)* | |
| `SUBARG` | *(à compléter)* | |
| `SWAP` | *(à compléter)* | |
| `TAN2` | *(à compléter)* | |
| `TRUE` | *(à compléter)* | |
| `TRUNCATEF` | *(à compléter)* | |
| `TXTMF` | *(à compléter)* | |
| `TXTMI` | *(à compléter)* | |
| `TXTMO` | *(à compléter)* | |
| `TYPE` | *(à compléter)* | |
| `UBOUND` | *(à compléter)* | |
| `UNPACK` | *(à compléter)* | |
| `VAR` | *(à compléter)* | |
| `WAITPID` | *(à compléter)* | |
| `WDAY` | *(à compléter)* | |
| `XOR` | *(à compléter)* | |
| `YDAY` | *(à compléter)* | |


## Chaînes

| Nom | Description | Suffixe `$` usuel |
|-----|-------------|-------------------|
| `ASC` | ASC(s) — code du 1er caractère | |
| `BIN` | BIN(n) — binaire | |
| `CHOMP` | *(à compléter)* | |
| `CHR`$ | CHR$(n) — caractère ASCII | |
| `CONCATENATE` | *(à compléter)* | |
| `CVD` | *(à compléter)* | |
| `CVI` | *(à compléter)* | |
| `CVL` | *(à compléter)* | |
| `CVS` | *(à compléter)* | |
| `FORMAT` | FORMAT — mise en forme | |
| `FORMATDATE` | *(à compléter)* | |
| `HEX`$ | HEX$(n) — hexadécimal | |
| `INSTR` | INSTR(s,sub) — position de recherche | |
| `INSTRREV` | *(à compléter)* | |
| `JOIN` | JOIN — collage de tableau | |
| `LCASE`$ | LCASE$(s) — minuscules | |
| `LEFT`$ | LEFT$(s,n) — n caractères à gauche | |
| `LTRIM`$ | LTRIM$(s) — espace gauche | |
| `MID`$ | MID$(s,p[,n]) — sous-chaîne | |
| `MKD` | *(à compléter)* | |
| `MKI` | *(à compléter)* | |
| `MKL` | *(à compléter)* | |
| `MKS` | *(à compléter)* | |
| `OCT`$ | OCT$(n) — octal | |
| `REPLACE` | REPLACE — substitution dans une chaîne | |
| `RIGHT`$ | RIGHT$(s,n) — n caractères à droite | |
| `RTRIM`$ | RTRIM$(s) — espace droite | |
| `SPACE`$ | SPACE$(n) — n espaces | |
| `SPLIT` | SPLIT — découpage en tableau | |
| `SPLITA` | *(à compléter)* | |
| `SPLITAQ` | *(à compléter)* | |
| `STR`$ | STR$(n) — conversion texte | |
| `STRING` | *(à compléter)* | |
| `STRREVERSE` | *(à compléter)* | |
| `TRIM`$ | TRIM$(s) — espaces latérales | |
| `UCASE`$ | UCASE$(s) — majuscules | |
| `VAL` | VAL(s) — conversion numérique | |


## Console I/O (cio)

| Nom | Description | Suffixe `$` usuel |
|-----|-------------|-------------------|
| `BINMF` | *(à compléter)* | |
| `BINMI` | *(à compléter)* | |
| `BINMO` | *(à compléter)* | |


## Fichiers

| Nom | Description | Suffixe `$` usuel |
|-----|-------------|-------------------|
| `CHDIR` | *(à compléter)* | |
| `CLOSE` | *(à compléter)* | |
| `CLOSEDIR` | *(à compléter)* | |
| `FILELEN` | *(à compléter)* | |
| `FREEFILE` | *(à compléter)* | |
| `KILL` | *(à compléter)* | |
| `LOF` | *(à compléter)* | |
| `MKDIR` | *(à compléter)* | |
| `NAME` | *(à compléter)* | |
| `OPEN` | *(à compléter)* | |
| `OPENDIR` | *(à compléter)* | |
| `RESET` | *(à compléter)* | |
| `SEEK` | *(à compléter)* | |


## Configuration

| Nom | Description | Suffixe `$` usuel |
|-----|-------------|-------------------|
| `CONF` | *(à compléter)* | |
| `OPTION` | *(à compléter)* | |
| `OPTIONF` | *(à compléter)* | |


## Processus/Threads

| Nom | Description | Suffixe `$` usuel |
|-----|-------------|-------------------|
| `CREATEPROCESS` | *(à compléter)* | |
| `CREATEPROCESSEX` | *(à compléter)* | |


## Environnement

| Nom | Description | Suffixe `$` usuel |
|-----|-------------|-------------------|
| `ENVIRON`$ | ENVIRON$(var) — variable d’environnement | |
| `RESUME` | RESUME — reprise après ON ERROR | |


## Console/Écran

| Nom | Description | Suffixe `$` usuel |
|-----|-------------|-------------------|
| `INPUTFUN` | *(à compléter)* | |
| `PRINT` | *(à compléter)* | |
| `PRINTNL` | *(à compléter)* | |


## Interface graphique

| Nom | Description | Suffixe `$` usuel |
|-----|-------------|-------------------|
| `LABEL` | *(à compléter)* | |


## Callbacks

| Nom | Description | Suffixe `$` usuel |
|-----|-------------|-------------------|
| `ONERRORGOTO` | *(à compléter)* | |
| `ONERRORGOTONULL` | *(à compléter)* | |
| `ONERRORRESUMELABEL` | *(à compléter)* | |
| `ONERRORRESUMENEXT` | *(à compléter)* | |

