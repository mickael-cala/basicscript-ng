# GRAMMAIRE — mots-clés, fonctions et numéros

Relevé de la grammaire vivante, produit le 2026-10-25 en relisant
[syntax.c](../src/syntax.c) et [syntax.h](../src/syntax.h). Il répond à
trois questions : **quel nom porte quel numéro**, **quels numéros ne servent
à rien**, et **comment attribuer les suivants**. `python tests/run.py
--grammaire` vérifie déjà les tables en paires `{ "nom", CMD_X }` : tout
nom y a un corps, tout `CMD_` a un `#define`, aucun `NOTIMP` n'est armé.
Les noms de `COMMANDSYMBOLS[]`, eux, sont de simples chaînes indexées par
opcode : le contrôle ne les voit pas, et c'est là que vivent les 255 noms
morts (§ 6.2). Étendre le contrôle à cette table est l'étape 2 de § 7.4 ;
ce document est la vue d'ensemble, pas le contrôle automatique.

| en un coup d'œil | nombre |
|---|---|
| codes de mots-clés utilisés | 77 sur 100 |
| codes de mots-clés libres | 23 |
| emplacements d'opcodes | 544 (350–893) |
| opcodes avec un corps | 278 |
| opcodes sans corps | 266, dont 11 mots du préprocesseur |
| noms morts | 255 |
| commandes (opcodes / lignes) | 115 / 133 |
| fonctions internes (opcodes / noms) | 144 / 165 |
| opérateurs (binaires / unaires) | 21 / 5 |

## 1. Trois espaces de numéros

| espace | plage | qui l'utilise | consommé |
|---|---|---|---|
| codes de mots-clés | 250–349 | `ASYMBOLS` (texte → code), les motifs de `COMMANDS[]` | 77 / 100 |
| opcodes | 350–893 | `CommandFunction[]`, `COMMANDSYMBOLS[]`, `COMMANDS[]`, `INTERNALFUNCTIONS`, `BINARIES`/`UNARIES` | 544 / 544 |
| index de table | `opcode - 350` | accès direct dans `execute.c`, `builder.c` | 544 places |

Un **mot-clé** est un mot réservé que l'analyseur lexical transforme en code
(250–349) ; ce code sert de jetons dans les motifs de `COMMANDS[]`. Une
**commande** est un opcode (350+) avec un corps C dans `CommandFunction[]`.
Une **fonction interne** est un opcode appelable dans une expression, décrit
par `INTERNALFUNCTIONS[]` (arité minimale et maximale). Un **opérateur** est
un opcode présent dans `BINARIES[]` ou `UNARIES[]`. Les trois se recoupent :
`time` est une fonction interne, `print` une commande, `+` un opérateur.

## 2. Les mots-clés (77 codes)

### 2.1 Mots qui ouvrent une ligne

`rôle` distingue un mot de commande (au moins un de ses opcodes a un
corps), un mot du préprocesseur (aucun corps) et un mot mixte, comme
`end`, qui ouvre aussi bien `end` que `end if` ou `end module`.

| code | mot | opcode(s) | rôle |
|---|---|---|---|
| 256 | `module` | 875 | préprocesseur |
| 257 | `end` | 523, 657, 730, 774, 874 | mixte |
| 258 | `local` | 883 | préprocesseur |
| 259 | `stop` | 684 | commande |
| 260 | `declare` | 727, 761, 876 | mixte |
| 262 | `const` | 873 | préprocesseur |
| 263 | `global` | 878, 881 | préprocesseur |
| 264 | `var` | 880 | préprocesseur |
| 265 | `swap` | 668 | commande |
| 266 | `printnl` | 626, 844 | commande |
| 267 | `print` | 626, 763, 842 | commande |
| 268 | `function` | 682, 700 | commande |
| 269 | `exit` | 454, 578, 892 | commande |
| 270 | `sub` | 756, 814 | commande |
| 271 | `call` | 787 | commande |
| 272 | `dim` | 893 | commande |
| 273 | `let` | 580 | commande |
| 280 | `ref` | 634 | commande |
| 281 | `goto` | 609 | commande |
| 282 | `go` | 507, 609 | commande |
| 283 | `to` | 716 | commande |
| 284 | `gosub` | 507 | commande |
| 285 | `return` | 399 | commande |
| 286 | `pop` | 653 | commande |
| 289 | `on` | 361, 504, 704, 734 | commande |
| 292 | `resume` | 597, 710, 725 | commande |
| 293 | `next` | 758, 831 | commande |
| 294 | `while` | 865 | commande |
| 295 | `wend` | 776 | commande |
| 296 | `do` | 360, 422, 628 | commande |
| 297 | `until` | 854 | commande |
| 298 | `loop` | 413, 519, 614 | commande |
| 299 | `repeat` | 605 | commande |
| 300 | `for` | 439 | commande |
| 301 | `step` | 629 | commande |
| 302 | `if` | 471, 571 | commande |
| 304 | `elseif` | 414, 891 | commande |
| 305 | `else` | 433, 890 | commande |
| 308 | `endif` | 730 | commande |
| 309 | `open` | 363, 577 | commande |
| 313 | `reset` | 651, 674 | commande |
| 314 | `close` | 379, 804 | commande |
| 316 | `seek` | 458 | commande |
| 317 | `rewind` | 658 | commande |
| 318 | `line` | 427, 678 | commande |
| 320 | `truncate` | 728 | commande |
| 321 | `lock` | 417, 736 | commande |
| 324 | `mkdir` | 693 | commande |
| 325 | `deltree` | 487 | commande |
| 326 | `delete` | 377 | commande |
| 327 | `filecopy` | 789 | commande |
| 328 | `name` | 562 | commande |
| 329 | `chdir` | 696 | commande |
| 330 | `set` | 404, 472, 656, 850 | commande |
| 332 | `binmode` | 408, 448, 663 | commande |
| 334 | `textmode` | 354, 541, 808 | commande |
| 335 | `splitaq` | 751 | commande |
| 338 | `splita` | 407 | commande |
| 339 | `split` | 378 | commande |
| 340 | `unpack` | 574 | commande |
| 341 | `randomize` | 703, 719 | commande |
| 345 | `sleep` | 635 | commande |

### 2.2 Mots auxiliaires (dans la ligne, pas en tête)

| code | mot | apparaît dans les opcodes |
|---|---|---|
| 256 | `module` | 874 |
| 262 | `const` | 878 |
| 268 | `function` | 454, 774 |
| 270 | `sub` | 507, 578, 657, 727 |
| 281 | `goto` | 504, 734 |
| 283 | `to` | 378, 407, 417, 574, 609, 656, 751, 850 |
| 291 | `null` | 734 |
| 292 | `resume` | 361, 704 |
| 293 | `next` | 704, 725 |
| 294 | `while` | 614, 628, 892 |
| 296 | `do` | 892 |
| 297 | `until` | 360, 413 |
| 299 | `repeat` | 892 |
| 300 | `for` | 363, 417, 892 |
| 302 | `if` | 730 |
| 303 | `then` | 414, 471, 571, 891 |
| 310 | `directory` | 577, 651, 804 |
| 311 | `pattern` | 577 |
| 312 | `as` | 363, 577 |
| 322 | `region` | 417 |
| 323 | `from` | 417 |
| 331 | `file` | 404 |
| 333 | `output` | 448, 541 |
| 336 | `by` | 378, 407, 574, 751 |
| 337 | `quote` | 751 |
| 343 | `wild` | 472, 656 |
| 344 | `no` | 472 |
| 347 | `alias` | 727, 761 |
| 348 | `lib` | 727, 761 |

### 2.3 Mots du préprocesseur (11, aucun corps, jamais exécutés)

| mot | opcode |
|---|---|
| `const` | 873 |
| `endmodule` | 874 |
| `module` | 875 |
| `pragma` | 876 |
| `nlabel` | 877 |
| `gconst` | 878 |
| `label` | 879 |
| `var` | 880 |
| `global` | 881 |
| `empty` | 882 |
| `local` | 883 |

`--grammaire` connaît cette liste (`PREPROCESSEUR_SANS_CORPS`) et refuse
tout autre nom dont l'emplacement de `CommandFunction[]` est vide.

## 3. Les fonctions internes (144 opcodes, 165 noms)

L'arité est celle de `INTERNALFUNCTIONS[]` : minimum, maximum. Un nom en
plusieurs exemplaires est un alias du même opcode (par exemple `upper$` et
`ucase`).

| nom(s) | opcode | arité |
|---|---|---|
| `abs` | 398 | 1–1 |
| `acos` | 564 | 1–1 |
| `acosecant` | 694 | 1–1 |
| `actan` | 869 | 1–1 |
| `addday` | 520 | 2–2 |
| `addhour` | 845 | 2–2 |
| `addminute` | 608 | 2–2 |
| `addmonth` | 561 | 2–2 |
| `address` | 509 | 1–1 |
| `addsecond` | 590 | 2–2 |
| `addweek` | 821 | 2–2 |
| `addyear` | 594 | 2–2 |
| `ahcos` | 885 | 1–1 |
| `ahcosecant` | 888 | 1–1 |
| `ahctan` | 889 | 1–1 |
| `ahsecant` | 887 | 1–1 |
| `ahsin` | 884 | 1–1 |
| `ahtan` | 886 | 1–1 |
| `asc` | 861 | 1–1 |
| `asecant` | 852 | 1–1 |
| `asin` | 699 | 1–1 |
| `atan` | 588 | 1–1 |
| `atn` | 840 | 1–1 |
| `bin` | 553 | 1–1 |
| `chomp` | 828 | 1–1 |
| `chr`, `chr$` | 644 | 1–1 |
| `cint`, `int` | 598 | 1–1 |
| `command` | 550 | 0–0 |
| `conf` | 478 | 1–1 |
| `cos` | 738 | 1–1 |
| `cosecant` | 368 | 1–1 |
| `cotan` | 739 | 1–1 |
| `cotan2` | 823 | 2–2 |
| `crypt` | 837 | 2–2 |
| `curdir` | 732 | 0–0 |
| `day` | 633 | 0–1 |
| `deg2rad` | 356 | 1–1 |
| `environ`, `environ$` | 647 | 1–1 |
| `eod` | 532 | 1–1 |
| `eof` | 465 | 1–1 |
| `error` | 388 | 0–0 |
| `error$` | 455 | 0–1 |
| `even` | 839 | 1–1 |
| `execute` | 686 | 3–3 |
| `exp` | 492 | 1–1 |
| `false` | 646 | 0–0 |
| `fileaccesstime` | 675 | 1–1 |
| `filecreatetime` | 451 | 1–1 |
| `fileexists` | 497 | 1–1 |
| `filelen` | 525 | 1–1 |
| `filemodifytime` | 375 | 1–1 |
| `fileowner` | 645 | 1–1 |
| `fix` | 843 | 1–1 |
| `format` | 429 | 1–2000000 |
| `formatdate`, `formattime` | 373 | 1–2 |
| `frac` | 582 | 1–1 |
| `freefile` | 767 | 0–1 |
| `gcd` | 424 | 2–100000 |
| `gmtime` | 812 | 0–0 |
| `gmtimetolocaltime` | 835 | 1–1 |
| `hcos` | 395 | 1–1 |
| `hcosecant` | 384 | 1–1 |
| `hctan` | 468 | 1–1 |
| `hex`, `hex$` | 352 | 1–1 |
| `hostname` | 636 | 0–0 |
| `hour` | 795 | 0–1 |
| `hsecant` | 820 | 1–1 |
| `hsin` | 766 | 1–1 |
| `htan` | 613 | 1–1 |
| `icall` | 474 | 1–2000000 |
| `imax` | 579 | 1–100000 |
| `imin` | 421 | 1–100000 |
| `input` | 800 | 1–2 |
| `instr` | 490 | 2–3 |
| `instrrev` | 791 | 2–3 |
| `isarray` | 396 | 1–1 |
| `isdefined` | 822 | 1–1 |
| `isdirectory` | 426 | 1–1 |
| `isempty` | 779 | 1–1 |
| `isfile` | 480 | 1–1 |
| `isinteger` | 358 | 1–1 |
| `isnumeric` | 583 | 1–1 |
| `isreal` | 367 | 1–1 |
| `isstring` | 679 | 1–1 |
| `isundef` | 467 | 1–1 |
| `join` | 351 | 2–100000 |
| `joker` | 498 | 1–1 |
| `kill` | 447 | 1–1 |
| `lbound` | 486 | 1–1 |
| `lcase`, `lower`, `lower$` | 649 | 1–1 |
| `lcm` | 723 | 2–100000 |
| `left`, `left$` | 438 | 2–2 |
| `len` | 502 | 1–1 |
| `loc` | 803 | 1–1 |
| `localtimetogmtime` | 672 | 1–1 |
| `lof` | 807 | 1–1 |
| `log` | 863 | 1–1 |
| `log10` | 717 | 1–1 |
| `ltrim`, `ltrim$` | 780 | 1–1 |
| `max` | 402 | 1–100000 |
| `maxint` | 549 | 0–0 |
| `mid`, `mid$` | 374 | 2–3 |
| `min` | 603 | 1–100000 |
| `minint` | 389 | 0–0 |
| `minute` | 372 | 0–1 |
| `month` | 364 | 0–1 |
| `nextfile` | 726 | 1–1 |
| `now`, `time` | 752 | 0–0 |
| `oct`, `oct$` | 494 | 1–1 |
| `odd` | 650 | 1–1 |
| `option` | 707 | 1–1 |
| `pack` | 687 | 1–2000000 |
| `pi` | 485 | 0–0 |
| `pow` | 747 | 1–1 |
| `rad2deg` | 355 | 1–1 |
| `replace` | 606 | 3–5 |
| `right`, `right$` | 548 | 2–2 |
| `rnd` | 681 | 0–0 |
| `round` | 637 | 1–2 |
| `rtrim`, `rtrim$` | 483 | 1–1 |
| `sec` | 500 | 0–1 |
| `secant` | 762 | 1–1 |
| `sgn` | 777 | 1–1 |
| `sin` | 565 | 1–1 |
| `space`, `space$` | 401 | 1–1 |
| `sqrt` | 359 | 1–1 |
| `str`, `str$` | 834 | 1–1 |
| `string`, `string$` | 443 | 2–2 |
| `strreverse`, `strreverse$` | 811 | 1–1 |
| `system` | 369 | 1–1 |
| `tan` | 742 | 1–1 |
| `tan2` | 406 | 2–2 |
| `timevalue` | 462 | 0–6 |
| `trim`, `trim$` | 846 | 1–1 |
| `true` | 499 | 0–0 |
| `type` | 671 | 1–1 |
| `ubound` | 630 | 1–1 |
| `ucase`, `upper`, `upper$` | 419 | 1–1 |
| `undef` | 741 | 0–0 |
| `val` | 365 | 1–1 |
| `waitpid` | 418 | 2–2 |
| `weekday` | 860 | 0–1 |
| `year` | 528 | 0–1 |
| `yearday` | 830 | 0–1 |

## 4. Les opérateurs

### 4.1 Binaires (21)

| symbole | opcode |
|---|---|
| `<<` | 740 |
| `>>` | 664 |
| `??` | 475 |
| `^` | 587 |
| `*` | 737 |
| `/` | 464 |
| `\` | 806 |
| `%` | 415 |
| `+` | 643 |
| `-` | 627 |
| `&` | 403 |
| `like` | 505 |
| `=` | 445 |
| `<` | 735 |
| `<=` | 639 |
| `>` | 479 |
| `>=` | 798 |
| `<>` | 517 |
| `and` | 794 |
| `or` | 527 |
| `xor` | 456 |

### 4.2 Unaires (5)

| symbole | opcode |
|---|---|
| `+` | 643 |
| `-` | 627 |
| `not` | 567 |
| `byval` | 690 |
| `#` | 596 |

### 4.3 Jetons d'affectation composée

Six jetons (`+=`, `-=`, `*=`, `/=`, `\=`, `&=`) ont un corps mais aucune
ligne de grammaire propre : `ex_IsCommandLET` les convertit en `LETP`,
`LETM`, `LETS`, `LETD`, `LETI`, `LETC`. Leurs corps sont donc du code mort
conservé : `COMMAND_EXTOPMG` (411), `COMMAND_EXTOPHG` (441), `COMMAND_EXTOPBG` (476), `COMMAND_EXTOPNG` (516), `COMMAND_EXTOPIG` (691), `COMMAND_EXTOPAG` (720).

## 5. Les commandes (lignes de grammaire)

115 opcodes distincts portent les 133 lignes
de `COMMANDS[]` : une commande peut avoir plusieurs formes (`EXIT`, `EXIT
FOR`, `EXIT WHILE`…).

| opcode | nom | lignes | corps |
|---|---|---|---|
| 354 | `TXTMI` | 1 | `COMMAND_TXTMI` |
| 360 | `DOUNTIL` | 1 | `COMMAND_DOUNTIL` |
| 361 | `ONERRORRESUMELABEL` | 1 | `COMMAND_ONERRORRESUMELABEL` |
| 363 | `OPEN` | 1 | `COMMAND_OPEN` |
| 377 | `DELETE` | 1 | `COMMAND_DELETE` |
| 378 | `SPLIT` | 1 | `COMMAND_SPLIT` |
| 379 | `CLOSE` | 2 | `COMMAND_CLOSE` |
| 386 | `LETC` | 1 | `COMMAND_LETC` |
| 392 | `CERROR` | 1 | `COMMAND_CERROR` |
| 399 | `RETURNC` | 1 | `COMMAND_RETURNC` |
| 404 | `SETFILE` | 1 | `COMMAND_SETFILE` |
| 407 | `SPLITA` | 1 | `COMMAND_SPLITA` |
| 408 | `BINMF` | 2 | `COMMAND_BINMF` |
| 413 | `LOOPUNTIL` | 1 | `COMMAND_LOOPUNTIL` |
| 414 | `ELSIF` | 1 | `COMMAND_ELSIF` |
| 417 | `RLOCK` | 2 | `COMMAND_RLOCK` |
| 422 | `DO` | 1 | `COMMAND_DO` |
| 427 | `LINPUT` | 1 | `COMMAND_LINPUT` |
| 431 | `OPTION` | 1 | `COMMAND_OPTION` |
| 433 | `ELSE` | 1 | `COMMAND_ELSE` |
| 439 | `FOR` | 1 | `COMMAND_FOR` |
| 446 | `ICALL` | 1 | `COMMAND_ICALL` |
| 448 | `BINMO` | 1 | `COMMAND_BINMO` |
| 454 | `EXITFUNC` | 1 | `COMMAND_EXITFUNC` |
| 458 | `SEEK` | 2 | `COMMAND_SEEK` |
| 471 | `SLIF` | 1 | `COMMAND_SLIF` |
| 472 | `SETNOJO` | 2 | `COMMAND_SETNOJO` |
| 473 | `LET` | 1 | `COMMAND_LET` |
| 487 | `DELETEF` | 1 | `COMMAND_DELETEF` |
| 488 | `LETM` | 1 | `COMMAND_LETM` |
| 504 | `ONERRORGOTO` | 1 | `COMMAND_ONERRORGOTO` |
| 506 | `LETP` | 1 | `COMMAND_LETP` |
| 507 | `GOSUB` | 2 | `COMMAND_GOSUB` |
| 519 | `LOOP` | 1 | `COMMAND_LOOP` |
| 523 | `END` | 1 | `COMMAND_END` |
| 541 | `TXTMO` | 1 | `COMMAND_TXTMO` |
| 562 | `NAME` | 1 | `COMMAND_NAME` |
| 571 | `IF` | 1 | `COMMAND_IF` |
| 574 | `UNPACK` | 1 | `COMMAND_UNPACK` |
| 576 | `LETI` | 1 | `COMMAND_LETI` |
| 577 | `OPENDIR` | 1 | `COMMAND_OPENDIR` |
| 578 | `EXITSUB` | 1 | `COMMAND_EXITSUB` |
| 580 | `LLET` | 1 | `COMMAND_LLET` |
| 597 | `RESUMELABEL` | 1 | `COMMAND_RESUMELABEL` |
| 605 | `REPEAT` | 1 | `COMMAND_REPEAT` |
| 609 | `GOTO` | 2 | `COMMAND_GOTO` |
| 614 | `LOOPWHILE` | 1 | `COMMAND_LOOPWHILE` |
| 624 | `CUNDEF` | 1 | `COMMAND_CUNDEF` |
| 626 | `PRINTNL` | 2 | `COMMAND_PRINTNL` |
| 628 | `DOWHILE` | 1 | `COMMAND_DOWHILE` |
| 629 | `FORSTEP` | 1 | `COMMAND_FORSTEP` |
| 634 | `REF` | 1 | `COMMAND_REF` |
| 635 | `SLEEP` | 1 | `COMMAND_SLEEP` |
| 651 | `RESETDIR` | 2 | `COMMAND_RESETDIR` |
| 653 | `POP` | 1 | `COMMAND_POP` |
| 656 | `SETWILD` | 1 | `COMMAND_SETWILD` |
| 657 | `ENDSUB` | 1 | `COMMAND_ENDSUB` |
| 658 | `REWIND` | 2 | `COMMAND_REWIND` |
| 663 | `BINMI` | 1 | `COMMAND_BINMI` |
| 668 | `SWAP` | 1 | `COMMAND_SWAP` |
| 674 | `RESET` | 1 | `COMMAND_RESET` |
| 678 | `LINPUTF` | 1 | `COMMAND_LINPUTF` |
| 682 | `FUNCTIONARG` | 1 | `COMMAND_FUNCTIONARG` |
| 684 | `STOP` | 1 | `COMMAND_STOP` |
| 693 | `MKDIR` | 1 | `COMMAND_MKDIR` |
| 696 | `CHDIR` | 1 | `COMMAND_CHDIR` |
| 700 | `FUNCTION` | 1 | `COMMAND_FUNCTION` |
| 703 | `RANDOMIZE` | 1 | `COMMAND_RANDOMIZE` |
| 704 | `ONERRORRESUMENEXT` | 1 | `COMMAND_ONERRORRESUMENEXT` |
| 708 | `FLET` | 1 | `COMMAND_FLET` |
| 710 | `RESUME` | 1 | `COMMAND_RESUME` |
| 716 | `FORTO` | 1 | `COMMAND_FORTO` |
| 719 | `RANDOMIZA` | 1 | `COMMAND_RANDOMIZA` |
| 725 | `RESUMENEXT` | 1 | `COMMAND_RESUMENEXT` |
| 727 | `EXTERNAL` | 1 | `COMMAND_EXTERNAL` |
| 728 | `TRUNCATEF` | 1 | `COMMAND_TRUNCATEF` |
| 730 | `ENDIF` | 2 | `COMMAND_ENDIF` |
| 734 | `ONERRORGOTONULL` | 1 | `COMMAND_ONERRORGOTONULL` |
| 736 | `FLOCK` | 2 | `COMMAND_FLOCK` |
| 751 | `SPLITAQ` | 1 | `COMMAND_SPLITAQ` |
| 756 | `SUB` | 1 | `COMMAND_SUB` |
| 758 | `NEXTI` | 1 | `COMMAND_NEXTI` |
| 761 | `EXTERNAM` | 1 | `COMMAND_EXTERNAM` |
| 763 | `FPRINT` | 1 | `COMMAND_FPRINT` |
| 768 | `CBYVAL` | 1 | `COMMAND_CBYVAL` |
| 774 | `ENDFUNC` | 1 | `COMMAND_ENDFUNC` |
| 776 | `WEND` | 1 | `COMMAND_WEND` |
| 787 | `CALL` | 1 | `COMMAND_CALL` |
| 789 | `FCOPY` | 1 | `COMMAND_FCOPY` |
| 804 | `CLOSEDIR` | 2 | `COMMAND_CLOSEDIR` |
| 808 | `TXTMF` | 2 | `COMMAND_TXTMF` |
| 814 | `SUBARG` | 1 | `COMMAND_SUBARG` |
| 831 | `NEXT` | 1 | `COMMAND_NEXT` |
| 832 | `LETD` | 1 | `COMMAND_LETD` |
| 842 | `PRINT` | 1 | `COMMAND_PRINT` |
| 844 | `FPRINTNL` | 1 | `COMMAND_FPRINTNL` |
| 850 | `SETJOKER` | 1 | `COMMAND_SETJOKER` |
| 851 | `LETS` | 1 | `COMMAND_LETS` |
| 854 | `UNTIL` | 1 | `COMMAND_UNTIL` |
| 865 | `WHILE` | 1 | `COMMAND_WHILE` |
| 873 | `CONST` | 1 | `NULL` |
| 874 | `ENDMODULE` | 1 | `NULL` |
| 875 | `MODULE` | 1 | `NULL` |
| 876 | `PRAGMA` | 1 | `NULL` |
| 877 | `NLABEL` | 1 | `NULL` |
| 878 | `GCONST` | 1 | `NULL` |
| 879 | `LABEL` | 1 | `NULL` |
| 880 | `VAR` | 1 | `NULL` |
| 881 | `GLOBAL` | 1 | `NULL` |
| 882 | `EMPTY` | 1 | `NULL` |
| 883 | `LOCAL` | 1 | `NULL` |
| 890 | `ELSE_SL` | 1 | `COMMAND_ELSE_SL` |
| 891 | `ELSIF_SL` | 1 | `COMMAND_ELSIF_SL` |
| 892 | `EXIT` | 5 | `COMMAND_EXIT` |
| 893 | `DIM` | 1 | `COMMAND_DIM` |

## 6. Les trous

### 6.1 Codes de mots-clés libres (23)

Aucun nom ne les utilise ; ils sont disponibles pour de nouveaux mots-clés,
sans toucher aux codes existants :

`250`, `251`, `252`, `253`, `254`, `255`, `261`, `274`, `275`, `276`, `277`, `278`, `279`, `287`, `288`, `290`, `306`, `307`, `315`, `319`, `342`, `346`, `349`.

### 6.2 Noms morts (255 opcodes sans corps)

Sur 544 emplacements, **266 sont vides**, dont les 11 mots du préprocesseur. Les autres sont
des **noms morts** : 255 entrées de `COMMANDSYMBOLS[]` qu'aucun
motif et aucune fonction n'atteint. Elles viennent de deux sources :

- **245 jetons `EXTOPxx`** — les opérateurs étendus retirés le
  2026-10-23. Leurs noms subsistent, leurs corps sont vides, aucune ligne de
  grammaire ne les produit. `execute_GetCommandByName("EXTOPJP")` rend
  encore 350, et l'exécuter donnerait l'erreur 28 ;
- **10 noms de familles retirées** : `MKL` (423), `FORK` (493), `CVD` (592), `MKI` (593), `CVI` (623), `MKS` (718), `CVS` (772), `PAUSE` (783), `CVL` (786), `MKD` (825).

Ces noms ne coûtent pas d'opcode (les numéros sont déjà consommés) mais ils
trompent : un nom existe, donc `--grammaire` le croit vivant s'il apparaît
dans une table `{ "nom", CMD_X }`, et un module peut le récupérer par nom.

### 6.3 Autres écarts

- **8 `CMD_` définis dans `syntax.h` ne sont cités nulle part
  dans `syntax.c`** : `CMD_MKL`, `CMD_CVD`, `CMD_MKI`, `CMD_CVI`, `CMD_MKS`, `CMD_CVS`, `CMD_CVL`, `CMD_MKD`.
- `COMMANDSYMBOLS[]` porte encore des noms qui ne correspondent plus au
  corps : `SQR` (359) pointe sur `COMMAND_SQRT`, `MKD`, `MKS`, `MKI`, `MKL`,
  `CVD`, `CVI`, `CVS`, `CVL`, `PAUSE`, `FORK` n'ont plus de corps.
- Les six corps d'affectation composée (§ 4.3) ne sont jamais atteints.

### 6.4 Trous du langage, hors numérotation

Deux commandes de l'audit restent absentes : `select case` et `unlock`
(le mot n'existe nulle part dans `syntax.c` ; `lock` est là, son symétrique
non). `dim` est entrée le 2026-10-25 (opcode 893). `exit`/`exit for`/
`exit while`/`exit do`, `elseif` et le `if` sur une ligne sont en place.
`sqrt` remplace `sqr` côté langage ; seul le nom interne
`COMMANDSYMBOLS[359]` dit encore `SQR` (§6.3).

Une gêne d'analyse subsiste, hors numérotation : une déclaration à
parenthèses vides (`sub s()`) est refusée, il faut écrire `sub s`.

## 7. Stratégie de renumérotation

### 7.1 Ce qui casse si les opcodes bougent

- **Les binaires `.sb`** : `scriba -o` écrit `szCommandArray`, une image des
  `cNODE` avec leur `OpCode` ; `scriba -b` la recharge telle quelle. Un
  numéro déplacé change le sens d'un fichier existant sans le dire.
- **Le C produit par `-C`** : il contient la même image en octets ; le
  recompiler est obligatoire.
- **Les modules externes** : `besSETCOMMAND(CMD_X, f)` et `besGETCOMMAND`
  figent `CMD_*` à la compilation. Un module construit contre d'anciens
  en-têtes appellerait une autre fonction.
- **Les codes de mots-clés (250–349)**, eux, ne sortent pas du compilateur :
  aucun fichier sauvegardé ne les contient. Les déplacer est sûr, à condition
  de mettre à jour `ASYMBOLS` et les motifs ensemble.

### 7.2 Trois options

**N1 — Gel des numéros, noms morts marqués, trous consommés
(recommandée).** Les numéros 350–893 sont déclarés ABI et ne bougent plus ;
`END_CMD`/`NUM_CMD`/`END_EXEC` avancent d'un cran par commande ajoutée. Les
255 noms morts de `COMMANDSYMBOLS` deviennent la sentinelle `"-"` — jamais
`NULL`, que `strcmp` dereferencerait. En défense supplémentaire,
`execute_GetCommandByName` ignore un nom dont l'emplacement de
`CommandFunction[]` est vide : même un module qui demanderait `EXTOPJP`
reçoit 0 — un module qui voudrait ressusciter une de ces commandes doit
passer par le `CMD_*` compilé, pas par son nom. `COMMANDSYMBOLS[359]` est
renommé `SQRT`, son corps réel. Les 23
codes de mots-clés libres se consomment du plus petit au plus grand.
`--grammaire` est étendu : un nom de `COMMANDSYMBOLS` doit avoir un corps,
être un des onze mots du préprocesseur, ou valoir `"-"` ; un `"-"` doit
avoir un corps vide. Coût : 255 lignes mécaniques et quelques lignes dans
`execute.c` ; aucun numéro ne bouge. Gain : plus de nom trompeur, plus de
résolution d'un opcode mort, règle d'attribution écrite.

**N2 — Marquage des noms seulement.** On marque les 255 entrées mortes et
on renomme `SQR`, sans toucher à `execute_GetCommandByName`, à la règle des
codes libres ni à `--grammaire`. Aucun gain de plage : les 255 emplacements
restent entre 350 et 893. C'est N1 sans le verrou de résolution ni la règle
d'attribution.

**N3 — Renumérotation complète par familles.** On réattribue tout : par
exemple 250–299 préprocesseur, 350–499 commandes, 500–649 fonctions,
650–749 opérateurs, 750–899 réservé. Il faut réécrire les quatre tables
parallèles, `START_CMD`/`END_CMD`/`NUM_CMD`/`END_EXEC`, `syntax.h`, et
introduire une version de bytecode refusant les anciens `.sb`. Les modules
du dépôt se recompilent ; ceux d'un tiers, non. Le bénéfice est
cosmétique — la numérotation actuelle n'a pas de défaut fonctionnel — pour
un risque de rupture réel.

### 7.3 Règle d'attribution proposée (N1)

1. Un **nouveau mot-clé** prend le plus petit code libre de 250–349. Le
   bloc 250–255 reste réservé au préprocesseur — aucun de ses onze mots ne
   l'occupe aujourd'hui, et ils n'ont pas de corps, donc ils ne doivent pas
   empiéter sur les commandes.
2. Une **nouvelle commande** prend `END_CMD + 1` ; les trois constantes
   (`END_CMD`, `NUM_CMD`, `END_EXEC`) avancent ensemble, et l'entrée est
   ajoutée en queue de `CommandFunction[]` et `COMMANDSYMBOLS[]`.
3. Une **nouvelle fonction interne** réutilise l'opcode d'une fonction
   existante si c'est un alias, sinon prend `END_CMD + 1` et une ligne dans
   `INTERNALFUNCTIONS`.
4. Un **nom mort** ne se réattribue jamais : l'opcode reste consommé, le nom
   devient la sentinelle. C'est la seule façon de garder les `.sb` lisibles.
5. `--grammaire` est étendu pour refuser un nom de `COMMANDSYMBOLS` dont le
   corps est vide et qui n'est pas un mot du préprocesseur — la classe des
   255 noms morts actuels.

### 7.4 Ce que je ferais, dans l'ordre

1. Étendre `--grammaire` au contrôle des trois cas (corps, préprocesseur,
   `"-"`). Il échoue sur l'état actuel — 255 noms morts — et sert de
   garde-fou pour la suite.
2. Remplacer les 255 noms morts par `"-"` et `SQR` par `SQRT` (même
   nombres, aucun `.sb` ne change de sens). Vérifier que la campagne
   par défaut, `--modules` et `--upstream` restent vertes.
3. Faire ignorer par `execute_GetCommandByName` tout opcode sans corps.
4. Écrire la règle 250–349 dans ce document et dans `syntax.h`.
5. Garder N3 pour un éventuel format `.sb` versionné, jamais pour un simple
   rangement.

