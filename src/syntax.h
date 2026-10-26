/*
syntax.h
*/
#ifndef __SYNTAX_H__
#define __SYNTAX_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif
#define MAX_BINARY_OPERATOR_PRECEDENCE 10
#define START_CMD 350
#define END_CMD 893
#define NUM_CMD 544

#include "memory.h"
#include "builder.h"
#include "execute.h"

#define CMD_JOIN 351
void COMMAND_JOIN(pExecuteObject);
#define CMD_HEX 352
void COMMAND_HEX(pExecuteObject);
#define CMD_TXTMI 354
void COMMAND_TXTMI(pExecuteObject);
#define CMD_ISLONG 358
void COMMAND_ISLONG(pExecuteObject);
#define CMD_SQRT 359
void COMMAND_SQRT(pExecuteObject);
#define CMD_DOUNTIL 360
void COMMAND_DOUNTIL(pExecuteObject);
#define CMD_ONERRORRESUMELABEL 361
void COMMAND_ONERRORRESUMELABEL(pExecuteObject);
#define CMD_OPEN 363
void COMMAND_OPEN(pExecuteObject);
#define CMD_MONTH 364
void COMMAND_MONTH(pExecuteObject);
#define CMD_VAL 365
void COMMAND_VAL(pExecuteObject);
#define CMD_ISDOUBLE 367
void COMMAND_ISDOUBLE(pExecuteObject);
#define CMD_COSECANT 368
void COMMAND_COSECANT(pExecuteObject);
#define CMD_CREATEPROCESS 369
void COMMAND_CREATEPROCESS(pExecuteObject);
#define CMD_MINUTE 372
void COMMAND_MINUTE(pExecuteObject);
#define CMD_FORMATDATE 373
void COMMAND_FORMATDATE(pExecuteObject);
#define CMD_MID 374
void COMMAND_MID(pExecuteObject);
#define CMD_FTMODIFY 375
void COMMAND_FTMODIFY(pExecuteObject);
#define CMD_DELETE 377
void COMMAND_DELETE(pExecuteObject);
#define CMD_SPLIT 378
void COMMAND_SPLIT(pExecuteObject);
#define CMD_CONST 873
#define CMD_CLOSE 379
void COMMAND_CLOSE(pExecuteObject);
#define CMD_ENDMODULE 874
#define CMD_HCOSECANT 384
void COMMAND_HCOSECANT(pExecuteObject);
#define CMD_LETC 386
void COMMAND_LETC(pExecuteObject);
#define CMD_ERROR 388
void COMMAND_ERROR(pExecuteObject);
#define CMD_MININT 389
void COMMAND_MININT(pExecuteObject);
#define CMD_CERROR 392
void COMMAND_CERROR(pExecuteObject);
#define CMD_HCOS 395
void COMMAND_HCOS(pExecuteObject);
#define CMD_ISARRAY 396
void COMMAND_ISARRAY(pExecuteObject);
#define CMD_ABS 398
void COMMAND_ABS(pExecuteObject);
#define CMD_RETURNC 399
void COMMAND_RETURNC(pExecuteObject);
#define CMD_SPACE 401
void COMMAND_SPACE(pExecuteObject);
#define CMD_MAX 402
void COMMAND_MAX(pExecuteObject);
#define CMD_CONCATENATE 403
void COMMAND_CONCATENATE(pExecuteObject);
#define CMD_SETFILE 404
void COMMAND_SETFILE(pExecuteObject);
#define CMD_TAN2 406
void COMMAND_TAN2(pExecuteObject);
#define CMD_SPLITA 407
void COMMAND_SPLITA(pExecuteObject);
#define CMD_BINMF 408
void COMMAND_BINMF(pExecuteObject);
#define CMD_EXTOPMG 411
void COMMAND_EXTOPMG(pExecuteObject);
#define CMD_LOOPUNTIL 413
void COMMAND_LOOPUNTIL(pExecuteObject);
#define CMD_ELSIF 414
void COMMAND_ELSIF(pExecuteObject);
/* ELSE and ELSEIF of the single-line IF. They are internal opcodes: the
   analyser emits them when the keyword is followed by a command on the same
   line, so that the single-line IF can tell its own ELSE/ELSEIF from the
   ELSE/ELSEIF of an enclosing block IF (those are plain jump targets). */
#define CMD_ELSE_SL 890
void COMMAND_ELSE_SL(pExecuteObject);
#define CMD_ELSIF_SL 891
void COMMAND_ELSIF_SL(pExecuteObject);
/* EXIT [ FOR | WHILE | DO | REPEAT ] : saute au label de sortie de la boucle
   la plus interne. Le label est fourni par l'analyseur (ex_IsCommandEXIT) et
   le corps fait exactement ce que fait GOTO. */
#define CMD_EXIT 892
void COMMAND_EXIT(pExecuteObject);
/* DIM variable[indexes] : declare un tableau de taille connue, tous les
   elements undef. Les indices s'ecrivent entre crochets, comme partout dans
   le langage ; une dimension par crochet, et chaque dimension est allouee. */
#define CMD_DIM 893
void COMMAND_DIM(pExecuteObject);
#define CMD_MOD 415
void COMMAND_MOD(pExecuteObject);
#define CMD_RLOCK 417
void COMMAND_RLOCK(pExecuteObject);
#define CMD_WAITPID 418
void COMMAND_WAITPID(pExecuteObject);
#define CMD_UCASE 419
void COMMAND_UCASE(pExecuteObject);
#define CMD_IMIN 421
void COMMAND_IMIN(pExecuteObject);
#define CMD_DO 422
void COMMAND_DO(pExecuteObject);
#define CMD_MKL 423
#define CMD_GCD 424
void COMMAND_GCD(pExecuteObject);
#define CMD_ISDIR 426
void COMMAND_ISDIR(pExecuteObject);
#define CMD_LINPUT 427
void COMMAND_LINPUT(pExecuteObject);
#define CMD_FORMAT 429
void COMMAND_FORMAT(pExecuteObject);
#define CMD_OPTION 431
void COMMAND_OPTION(pExecuteObject);
#define CMD_ELSE 433
void COMMAND_ELSE(pExecuteObject);
#define CMD_LEFT 438
void COMMAND_LEFT(pExecuteObject);
#define CMD_FOR 439
void COMMAND_FOR(pExecuteObject);
#define CMD_EXTOPHG 441
void COMMAND_EXTOPHG(pExecuteObject);
#define CMD_STRING 443
void COMMAND_STRING(pExecuteObject);
#define CMD_EQ 445
void COMMAND_EQ(pExecuteObject);
#define CMD_ICALL 446
void COMMAND_ICALL(pExecuteObject);
#define CMD_KILL 447
void COMMAND_KILL(pExecuteObject);
#define CMD_BINMO 448
void COMMAND_BINMO(pExecuteObject);
#define CMD_FTCREATED 451
void COMMAND_FTCREATED(pExecuteObject);
#define CMD_EXITFUNC 454
void COMMAND_EXITFUNC(pExecuteObject);
#define CMD_ERRORDOLLAR 455
void COMMAND_ERRORDOLLAR(pExecuteObject);
#define CMD_XOR 456
void COMMAND_XOR(pExecuteObject);
#define CMD_SEEK 458
void COMMAND_SEEK(pExecuteObject);
#define CMD_TIMEVALUE 462
void COMMAND_TIMEVALUE(pExecuteObject);
#define CMD_DIV 464
void COMMAND_DIV(pExecuteObject);
#define CMD_EOFFUN 465
void COMMAND_EOFFUN(pExecuteObject);
#define CMD_ISUNDEF 467
void COMMAND_ISUNDEF(pExecuteObject);
#define CMD_HCTAN 468
void COMMAND_HCTAN(pExecuteObject);
#define CMD_SLIF 471
void COMMAND_SLIF(pExecuteObject);
#define CMD_SETNOJO 472
void COMMAND_SETNOJO(pExecuteObject);
#define CMD_LET 473
void COMMAND_LET(pExecuteObject);
#define CMD_ICALLFUN 474
void COMMAND_ICALLFUN(pExecuteObject);
#define CMD_EXTOPFF 475
void COMMAND_EXTOPFF(pExecuteObject);
#define CMD_EXTOPBG 476
void COMMAND_EXTOPBG(pExecuteObject);
#define CMD_CONF 478
void COMMAND_CONF(pExecuteObject);
#define CMD_GT 479
void COMMAND_GT(pExecuteObject);
#define CMD_ISREG 480
void COMMAND_ISREG(pExecuteObject);
#define CMD_MODULE 875
#define CMD_RTRIM 483
void COMMAND_RTRIM(pExecuteObject);
#define CMD_PI 485
void COMMAND_PI(pExecuteObject);
#define CMD_LBOUND 486
void COMMAND_LBOUND(pExecuteObject);
#define CMD_DELETEF 487
void COMMAND_DELETEF(pExecuteObject);
#define CMD_LETM 488
void COMMAND_LETM(pExecuteObject);
#define CMD_INSTR 490
void COMMAND_INSTR(pExecuteObject);
#define CMD_PRAGMA 876
#define CMD_EXP 492
void COMMAND_EXP(pExecuteObject);
#define CMD_OCT 494
void COMMAND_OCT(pExecuteObject);
#define CMD_FILEXISTS 497
void COMMAND_FILEXISTS(pExecuteObject);
#define CMD_JOKER 498
void COMMAND_JOKER(pExecuteObject);
#define CMD_TRUE 499
void COMMAND_TRUE(pExecuteObject);
#define CMD_SEC 500
void COMMAND_SEC(pExecuteObject);
#define CMD_LEN 502
void COMMAND_LEN(pExecuteObject);
#define CMD_ONERRORGOTO 504
void COMMAND_ONERRORGOTO(pExecuteObject);
#define CMD_LIKEOP 505
void COMMAND_LIKEOP(pExecuteObject);
#define CMD_LETP 506
void COMMAND_LETP(pExecuteObject);
#define CMD_GOSUB 507
void COMMAND_GOSUB(pExecuteObject);
#define CMD_ADDRESSF 509
void COMMAND_ADDRESSF(pExecuteObject);
#define CMD_EXTOPNG 516
void COMMAND_EXTOPNG(pExecuteObject);
#define CMD_NE 517
void COMMAND_NE(pExecuteObject);
#define CMD_LOOP 519
void COMMAND_LOOP(pExecuteObject);
#define CMD_ADDDAY 520
void COMMAND_ADDDAY(pExecuteObject);
#define CMD_END 523
void COMMAND_END(pExecuteObject);
#define CMD_NLABEL 877
#define CMD_FILELEN 525
void COMMAND_FILELEN(pExecuteObject);
#define CMD_OR 527
void COMMAND_OR(pExecuteObject);
#define CMD_YEAR 528
void COMMAND_YEAR(pExecuteObject);
#define CMD_EODFUN 532
void COMMAND_EODFUN(pExecuteObject);
#define CMD_GCONST 878
#define CMD_TXTMO 541
void COMMAND_TXTMO(pExecuteObject);
#define CMD_RIGHT 548
void COMMAND_RIGHT(pExecuteObject);
#define CMD_MAXINT 549
void COMMAND_MAXINT(pExecuteObject);
#define CMD_COMMANDF 550
void COMMAND_COMMANDF(pExecuteObject);
#define CMD_BIN 553
void COMMAND_BIN(pExecuteObject);
#define CMD_ADDMONTH 561
void COMMAND_ADDMONTH(pExecuteObject);
#define CMD_NAME 562
void COMMAND_NAME(pExecuteObject);
#define CMD_ACOS 564
void COMMAND_ACOS(pExecuteObject);
#define CMD_SIN 565
void COMMAND_SIN(pExecuteObject);
#define CMD_NOT 567
void COMMAND_NOT(pExecuteObject);
#define CMD_IF 571
void COMMAND_IF(pExecuteObject);
#define CMD_UNPACK 574
void COMMAND_UNPACK(pExecuteObject);
#define CMD_LETI 576
void COMMAND_LETI(pExecuteObject);
#define CMD_OPENDIR 577
void COMMAND_OPENDIR(pExecuteObject);
#define CMD_EXITSUB 578
void COMMAND_EXITSUB(pExecuteObject);
#define CMD_IMAX 579
void COMMAND_IMAX(pExecuteObject);
#define CMD_LLET 580
void COMMAND_LLET(pExecuteObject);
#define CMD_FRAC 582
void COMMAND_FRAC(pExecuteObject);
#define CMD_ISNUMERIC 583
void COMMAND_ISNUMERIC(pExecuteObject);
#define CMD_POWER 587
void COMMAND_POWER(pExecuteObject);
#define CMD_ATAN 588
void COMMAND_ATAN(pExecuteObject);
#define CMD_ADDSECOND 590
void COMMAND_ADDSECOND(pExecuteObject);
#define CMD_CVD 592
#define CMD_MKI 593
#define CMD_ADDYEAR 594
void COMMAND_ADDYEAR(pExecuteObject);
#define CMD_EXTOPL 596
void COMMAND_EXTOPL(pExecuteObject);
#define CMD_RESUMELABEL 597
void COMMAND_RESUMELABEL(pExecuteObject);
#define CMD_INT 598
void COMMAND_INT(pExecuteObject);
#define CMD_MIN 603
void COMMAND_MIN(pExecuteObject);
#define CMD_REPEAT 605
void COMMAND_REPEAT(pExecuteObject);
#define CMD_REPLACE 606
void COMMAND_REPLACE(pExecuteObject);
#define CMD_ADDMINUTE 608
void COMMAND_ADDMINUTE(pExecuteObject);
#define CMD_GOTO 609
void COMMAND_GOTO(pExecuteObject);
#define CMD_HTAN 613
void COMMAND_HTAN(pExecuteObject);
#define CMD_LOOPWHILE 614
void COMMAND_LOOPWHILE(pExecuteObject);
#define CMD_CVI 623
#define CMD_CUNDEF 624
void COMMAND_CUNDEF(pExecuteObject);
#define CMD_PRINTNL 626
void COMMAND_PRINTNL(pExecuteObject);
#define CMD_MINUS 627
void COMMAND_MINUS(pExecuteObject);
#define CMD_DOWHILE 628
void COMMAND_DOWHILE(pExecuteObject);
#define CMD_FORSTEP 629
void COMMAND_FORSTEP(pExecuteObject);
#define CMD_UBOUND 630
void COMMAND_UBOUND(pExecuteObject);
#define CMD_DAY 633
void COMMAND_DAY(pExecuteObject);
#define CMD_REF 634
void COMMAND_REF(pExecuteObject);
#define CMD_SLEEP 635
void COMMAND_SLEEP(pExecuteObject);
#define CMD_HOSTNAME 636
void COMMAND_HOSTNAME(pExecuteObject);
#define CMD_ROUND 637
void COMMAND_ROUND(pExecuteObject);
#define CMD_LE 639
void COMMAND_LE(pExecuteObject);
#define CMD_PLUS 643
void COMMAND_PLUS(pExecuteObject);
#define CMD_CHR 644
void COMMAND_CHR(pExecuteObject);
#define CMD_FOWNER 645
void COMMAND_FOWNER(pExecuteObject);
#define CMD_FALSE 646
void COMMAND_FALSE(pExecuteObject);
#define CMD_ENVIRON 647
void COMMAND_ENVIRON(pExecuteObject);
#define CMD_LCASE 649
void COMMAND_LCASE(pExecuteObject);
#define CMD_ODD 650
void COMMAND_ODD(pExecuteObject);
#define CMD_RESETDIR 651
void COMMAND_RESETDIR(pExecuteObject);
#define CMD_POP 653
void COMMAND_POP(pExecuteObject);
#define CMD_SETWILD 656
void COMMAND_SETWILD(pExecuteObject);
#define CMD_ENDSUB 657
void COMMAND_ENDSUB(pExecuteObject);
#define CMD_REWIND 658
void COMMAND_REWIND(pExecuteObject);
#define CMD_LABEL 879
#define CMD_BINMI 663
void COMMAND_BINMI(pExecuteObject);
#define CMD_EXTOPEE 664
void COMMAND_EXTOPEE(pExecuteObject);
#define CMD_SWAP 668
void COMMAND_SWAP(pExecuteObject);
#define CMD_TYPE 671
void COMMAND_TYPE(pExecuteObject);
#define CMD_LOCAL2GM 672
void COMMAND_LOCAL2GM(pExecuteObject);
#define CMD_RESET 674
void COMMAND_RESET(pExecuteObject);
#define CMD_FTACCESS 675
void COMMAND_FTACCESS(pExecuteObject);
#define CMD_LINPUTF 678
void COMMAND_LINPUTF(pExecuteObject);
#define CMD_ISSTRING 679
void COMMAND_ISSTRING(pExecuteObject);
#define CMD_RND 681
void COMMAND_RND(pExecuteObject);
#define CMD_FUNCTIONARG 682
void COMMAND_FUNCTIONARG(pExecuteObject);
#define CMD_STOP 684
void COMMAND_STOP(pExecuteObject);
#define CMD_CREATEPROCESSEX 686
void COMMAND_CREATEPROCESSEX(pExecuteObject);
#define CMD_PACK 687
void COMMAND_PACK(pExecuteObject);
#define CMD_BYVAL 690
void COMMAND_BYVAL(pExecuteObject);
#define CMD_EXTOPIG 691
void COMMAND_EXTOPIG(pExecuteObject);
#define CMD_MKDIR 693
void COMMAND_MKDIR(pExecuteObject);
#define CMD_ACOSECANT 694
void COMMAND_ACOSECANT(pExecuteObject);
#define CMD_CHDIR 696
void COMMAND_CHDIR(pExecuteObject);
#define CMD_ASIN 699
void COMMAND_ASIN(pExecuteObject);
#define CMD_FUNCTION 700
void COMMAND_FUNCTION(pExecuteObject);
#define CMD_RANDOMIZE 703
void COMMAND_RANDOMIZE(pExecuteObject);
#define CMD_ONERRORRESUMENEXT 704
void COMMAND_ONERRORRESUMENEXT(pExecuteObject);
#define CMD_OPTIONF 707
void COMMAND_OPTIONF(pExecuteObject);
#define CMD_FLET 708
void COMMAND_FLET(pExecuteObject);
#define CMD_RESUME 710
void COMMAND_RESUME(pExecuteObject);
#define CMD_FORTO 716
void COMMAND_FORTO(pExecuteObject);
#define CMD_LOG10 717
void COMMAND_LOG10(pExecuteObject);
#define CMD_MKS 718
#define CMD_RANDOMIZA 719
void COMMAND_RANDOMIZA(pExecuteObject);
#define CMD_EXTOPAG 720
void COMMAND_EXTOPAG(pExecuteObject);
#define CMD_LCM 723
void COMMAND_LCM(pExecuteObject);
#define CMD_VAR 880
#define CMD_RESUMENEXT 725
void COMMAND_RESUMENEXT(pExecuteObject);
#define CMD_NEXTFILE 726
void COMMAND_NEXTFILE(pExecuteObject);
#define CMD_GLOBAL 881
#define CMD_EXTERNAL 727
void COMMAND_EXTERNAL(pExecuteObject);
#define CMD_TRUNCATEF 728
void COMMAND_TRUNCATEF(pExecuteObject);
#define CMD_ENDIF 730
void COMMAND_ENDIF(pExecuteObject);
#define CMD_CURDIR 732
void COMMAND_CURDIR(pExecuteObject);
#define CMD_ONERRORGOTONULL 734
void COMMAND_ONERRORGOTONULL(pExecuteObject);
#define CMD_LT 735
void COMMAND_LT(pExecuteObject);
#define CMD_FLOCK 736
void COMMAND_FLOCK(pExecuteObject);
#define CMD_MULT 737
void COMMAND_MULT(pExecuteObject);
#define CMD_COS 738
void COMMAND_COS(pExecuteObject);
#define CMD_COTAN 739
void COMMAND_COTAN(pExecuteObject);
#define CMD_EXTOPDD 740
void COMMAND_EXTOPDD(pExecuteObject);
#define CMD_UNDEF 741
void COMMAND_UNDEF(pExecuteObject);
#define CMD_TAN 742
void COMMAND_TAN(pExecuteObject);
#define CMD_POW 747
void COMMAND_POW(pExecuteObject);
#define CMD_SPLITAQ 751
void COMMAND_SPLITAQ(pExecuteObject);
#define CMD_NOW 752
void COMMAND_NOW(pExecuteObject);
#define CMD_SUB 756
void COMMAND_SUB(pExecuteObject);
#define CMD_NEXTI 758
void COMMAND_NEXTI(pExecuteObject);
#define CMD_EXTERNAM 761
void COMMAND_EXTERNAM(pExecuteObject);
#define CMD_SECANT 762
void COMMAND_SECANT(pExecuteObject);
#define CMD_FPRINT 763
void COMMAND_FPRINT(pExecuteObject);
#define CMD_HSIN 766
void COMMAND_HSIN(pExecuteObject);
#define CMD_FREEFILE 767
void COMMAND_FREEFILE(pExecuteObject);
#define CMD_CBYVAL 768
void COMMAND_CBYVAL(pExecuteObject);
#define CMD_CVS 772
#define CMD_ENDFUNC 774
void COMMAND_ENDFUNC(pExecuteObject);
#define CMD_WEND 776
void COMMAND_WEND(pExecuteObject);
#define CMD_SGN 777
void COMMAND_SGN(pExecuteObject);
#define CMD_ISEMPTY 779
void COMMAND_ISEMPTY(pExecuteObject);
#define CMD_LTRIM 780
void COMMAND_LTRIM(pExecuteObject);
#define CMD_CVL 786
#define CMD_CALL 787
void COMMAND_CALL(pExecuteObject);
#define CMD_FCOPY 789
void COMMAND_FCOPY(pExecuteObject);
#define CMD_INSTRREV 791
void COMMAND_INSTRREV(pExecuteObject);
#define CMD_AND 794
void COMMAND_AND(pExecuteObject);
#define CMD_HOUR 795
void COMMAND_HOUR(pExecuteObject);
#define CMD_GE 798
void COMMAND_GE(pExecuteObject);
#define CMD_EMPTY 882
#define CMD_INPUTFUN 800
void COMMAND_INPUTFUN(pExecuteObject);
#define CMD_LOC 803
void COMMAND_LOC(pExecuteObject);
#define CMD_CLOSEDIR 804
void COMMAND_CLOSEDIR(pExecuteObject);
#define CMD_IDIV 806
void COMMAND_IDIV(pExecuteObject);
#define CMD_LOF 807
void COMMAND_LOF(pExecuteObject);
#define CMD_TXTMF 808
void COMMAND_TXTMF(pExecuteObject);
#define CMD_STRREVERSE 811
void COMMAND_STRREVERSE(pExecuteObject);
#define CMD_GMTIME 812
void COMMAND_GMTIME(pExecuteObject);
#define CMD_SUBARG 814
void COMMAND_SUBARG(pExecuteObject);
#define CMD_HSECANT 820
void COMMAND_HSECANT(pExecuteObject);
#define CMD_ADDWEEK 821
void COMMAND_ADDWEEK(pExecuteObject);
#define CMD_ISDEF 822
void COMMAND_ISDEF(pExecuteObject);
#define CMD_COTAN2 823
void COMMAND_COTAN2(pExecuteObject);
#define CMD_MKD 825
#define CMD_CHOMP 828
void COMMAND_CHOMP(pExecuteObject);
#define CMD_YDAY 830
void COMMAND_YDAY(pExecuteObject);
#define CMD_NEXT 831
void COMMAND_NEXT(pExecuteObject);
#define CMD_LETD 832
void COMMAND_LETD(pExecuteObject);
#define CMD_STR 834
void COMMAND_STR(pExecuteObject);
#define CMD_GM2LOCAL 835
void COMMAND_GM2LOCAL(pExecuteObject);
#define CMD_FCRYPT 837
void COMMAND_FCRYPT(pExecuteObject);
#define CMD_EVEN 839
void COMMAND_EVEN(pExecuteObject);
#define CMD_ATN 840
void COMMAND_ATN(pExecuteObject);
#define CMD_PRINT 842
void COMMAND_PRINT(pExecuteObject);
#define CMD_FIX 843
void COMMAND_FIX(pExecuteObject);
#define CMD_FPRINTNL 844
void COMMAND_FPRINTNL(pExecuteObject);
#define CMD_ADDHOUR 845
void COMMAND_ADDHOUR(pExecuteObject);
#define CMD_TRIM 846
void COMMAND_TRIM(pExecuteObject);
#define CMD_SETJOKER 850
void COMMAND_SETJOKER(pExecuteObject);
#define CMD_LETS 851
void COMMAND_LETS(pExecuteObject);
#define CMD_ASECANT 852
void COMMAND_ASECANT(pExecuteObject);
#define CMD_UNTIL 854
void COMMAND_UNTIL(pExecuteObject);
#define CMD_WDAY 860
void COMMAND_WDAY(pExecuteObject);
#define CMD_ASC 861
void COMMAND_ASC(pExecuteObject);
#define CMD_LOG 863
void COMMAND_LOG(pExecuteObject);
#define CMD_WHILE 865
void COMMAND_WHILE(pExecuteObject);
#define CMD_LOCAL 883
#define CMD_AHSIN 884
void COMMAND_AHSIN(pExecuteObject);
#define CMD_AHCOS 885
void COMMAND_AHCOS(pExecuteObject);
#define CMD_AHTAN 886
void COMMAND_AHTAN(pExecuteObject);
#define CMD_AHSECANT 887
void COMMAND_AHSECANT(pExecuteObject);
#define CMD_AHCOSECANT 888
void COMMAND_AHCOSECANT(pExecuteObject);
#define CMD_AHCTAN 889
void COMMAND_AHCTAN(pExecuteObject);
#define CMD_ACTAN 869
void COMMAND_ACTAN(pExecuteObject);
#define KEYWORDCODE_ISINTEGER CMD_ISLONG
#define KEYWORDCODE_ISNUMERIC CMD_ISNUMERIC
#define KEYWORDCODE_ISDEFINED CMD_ISDEF
#define KEYWORDCODE_ACOSECANT CMD_ACOSECANT
#define KEYWORDCODE_HCOSECANT CMD_HCOSECANT
#define KEYWORDCODE_FILEOWNER CMD_FOWNER
#define KEYWORDCODE_TIMEVALUE CMD_TIMEVALUE
#define KEYWORDCODE_ADDMINUTE CMD_ADDMINUTE
#define KEYWORDCODE_ADDSECOND CMD_ADDSECOND
#define KEYWORDCODE_RANDOMIZE 341
#define KEYWORDCODE_DIRECTORY 310
#define KEYWORDCODE_ISSTRING CMD_ISSTRING
#define KEYWORDCODE_COSECANT CMD_COSECANT
#define KEYWORDCODE_INSTRREV CMD_INSTRREV
#define KEYWORDCODE_FREEFILE CMD_FREEFILE
#define KEYWORDCODE_NEXTFILE CMD_NEXTFILE
#define KEYWORDCODE_ENVIRONS CMD_ENVIRON
#define KEYWORDCODE_ADDMONTH CMD_ADDMONTH
#define KEYWORDCODE_HOSTNAME CMD_HOSTNAME
#define KEYWORDCODE_TEXTMODE 334
#define KEYWORDCODE_FUNCTION 268
#define KEYWORDCODE_TRUNCATE 320
#define KEYWORDCODE_FILECOPY 327
#define KEYWORDCODE_ISARRAY CMD_ISARRAY
#define KEYWORDCODE_ISUNDEF CMD_ISUNDEF
#define KEYWORDCODE_ISEMPTY CMD_ISEMPTY
#define KEYWORDCODE_ASECANT CMD_ASECANT
#define KEYWORDCODE_HSECANT CMD_HSECANT
#define KEYWORDCODE_STRINGS CMD_STRING
#define KEYWORDCODE_REPLACE CMD_REPLACE
#define KEYWORDCODE_FILELEN CMD_FILELEN
#define KEYWORDCODE_ENVIRON CMD_ENVIRON
#define KEYWORDCODE_COMMAND CMD_COMMANDF
#define KEYWORDCODE_ADDRESS CMD_ADDRESSF
#define KEYWORDCODE_WEEKDAY CMD_WDAY
#define KEYWORDCODE_YEARDAY CMD_YDAY
#define KEYWORDCODE_ADDYEAR CMD_ADDYEAR
#define KEYWORDCODE_ADDHOUR CMD_ADDHOUR
#define KEYWORDCODE_ADDWEEK CMD_ADDWEEK
#define KEYWORDCODE_EXECUTE CMD_CREATEPROCESSEX
#define KEYWORDCODE_WAITPID CMD_WAITPID
#define KEYWORDCODE_BINMODE 332
#define KEYWORDCODE_DELTREE 325
#define KEYWORDCODE_SPLITAQ 335
#define KEYWORDCODE_PRINTNL 266
#define KEYWORDCODE_DECLARE 260
#define KEYWORDCODE_PATTERN 311
#define KEYWORDCODE_MAXINT CMD_MAXINT
#define KEYWORDCODE_MININT CMD_MININT
#define KEYWORDCODE_STRING CMD_STRING
#define KEYWORDCODE_LBOUND CMD_LBOUND
#define KEYWORDCODE_UBOUND CMD_UBOUND
#define KEYWORDCODE_ISREAL CMD_ISDOUBLE
#define KEYWORDCODE_COTAN2 CMD_COTAN2
#define KEYWORDCODE_SECANT CMD_SECANT
#define KEYWORDCODE_UPPERS CMD_UCASE
#define KEYWORDCODE_LOWERS CMD_LCASE
#define KEYWORDCODE_LTRIMS CMD_LTRIM
#define KEYWORDCODE_RTRIMS CMD_RTRIM
#define KEYWORDCODE_RIGHTS CMD_RIGHT
#define KEYWORDCODE_SPACES CMD_SPACE
#define KEYWORDCODE_OPTION CMD_OPTIONF
#define KEYWORDCODE_ERRORS CMD_ERRORDOLLAR
#define KEYWORDCODE_ISFILE CMD_ISREG
#define KEYWORDCODE_CURDIR CMD_CURDIR
#define KEYWORDCODE_FORMAT CMD_FORMAT
#define KEYWORDCODE_GMTIME CMD_GMTIME
#define KEYWORDCODE_MINUTE CMD_MINUTE
#define KEYWORDCODE_ADDDAY CMD_ADDDAY
#define KEYWORDCODE_SYSTEM CMD_CREATEPROCESS
#define KEYWORDCODE_REGION 322
#define KEYWORDCODE_OUTPUT 333
#define KEYWORDCODE_DELETE 326
#define KEYWORDCODE_REPEAT 299
#define KEYWORDCODE_UNPACK 340
#define KEYWORDCODE_MODULE 256
#define KEYWORDCODE_SPLITA 338
#define KEYWORDCODE_ELSEIF 304
#define KEYWORDCODE_RESUME 292
#define KEYWORDCODE_GLOBAL 263
#define KEYWORDCODE_RETURN 285
#define KEYWORDCODE_REWIND 317
#define KEYWORDCODE_BYVAL CMD_BYVAL
#define KEYWORDCODE_FALSE CMD_FALSE
#define KEYWORDCODE_UNDEF CMD_UNDEF
#define KEYWORDCODE_ROUND CMD_ROUND
#define KEYWORDCODE_LOG10 CMD_LOG10
#define KEYWORDCODE_UCASE CMD_UCASE
#define KEYWORDCODE_LCASE CMD_LCASE
#define KEYWORDCODE_LTRIM CMD_LTRIM
#define KEYWORDCODE_RTRIM CMD_RTRIM
#define KEYWORDCODE_RIGHT CMD_RIGHT
#define KEYWORDCODE_SPACE CMD_SPACE
#define KEYWORDCODE_JOKER CMD_JOKER
#define KEYWORDCODE_CHOMP CMD_CHOMP
#define KEYWORDCODE_ACTAN CMD_ACTAN
#define KEYWORDCODE_COTAN CMD_COTAN
#define KEYWORDCODE_HCTAN CMD_HCTAN
#define KEYWORDCODE_UPPER CMD_UCASE
#define KEYWORDCODE_LOWER CMD_LCASE
#define KEYWORDCODE_TRIMS CMD_TRIM
#define KEYWORDCODE_LEFTS CMD_LEFT
#define KEYWORDCODE_INSTR CMD_INSTR
#define KEYWORDCODE_ERROR CMD_ERROR
#define KEYWORDCODE_INPUT CMD_INPUTFUN
#define KEYWORDCODE_MONTH CMD_MONTH
#define KEYWORDCODE_ICALL CMD_ICALLFUN
#define KEYWORDCODE_CRYPT CMD_FCRYPT
#define KEYWORDCODE_PRINT 267
#define KEYWORDCODE_CONST 262
#define KEYWORDCODE_LOCAL 258
#define KEYWORDCODE_CHDIR 329
#define KEYWORDCODE_MKDIR 324
#define KEYWORDCODE_GOSUB 284
#define KEYWORDCODE_QUOTE 337
#define KEYWORDCODE_ENDIF 308
#define KEYWORDCODE_SPLIT 339
#define KEYWORDCODE_ALIAS 347
#define KEYWORDCODE_CLOSE 314
#define KEYWORDCODE_RESET 313
#define KEYWORDCODE_WHILE 294
#define KEYWORDCODE_UNTIL 297
#define KEYWORDCODE_SLEEP 345
#define KEYWORDCODE_LIKE CMD_LIKEOP
#define KEYWORDCODE_ASIN CMD_ASIN
#define KEYWORDCODE_ACOS CMD_ACOS
#define KEYWORDCODE_EVEN CMD_EVEN
#define KEYWORDCODE_TRUE CMD_TRUE
#define KEYWORDCODE_CINT CMD_INT
#define KEYWORDCODE_FRAC CMD_FRAC
#define KEYWORDCODE_TRIM CMD_TRIM
#define KEYWORDCODE_LEFT CMD_LEFT
#define KEYWORDCODE_TYPE CMD_TYPE
#define KEYWORDCODE_ATAN CMD_ATAN
#define KEYWORDCODE_HCOS CMD_HCOS
#define KEYWORDCODE_HSIN CMD_HSIN
#define KEYWORDCODE_HTAN CMD_HTAN
#define KEYWORDCODE_TAN2 CMD_TAN2
#define KEYWORDCODE_MIDS CMD_MID
#define KEYWORDCODE_CHRS CMD_CHR
#define KEYWORDCODE_STRS CMD_STR
#define KEYWORDCODE_HEXS CMD_HEX
#define KEYWORDCODE_OCTS CMD_OCT
#define KEYWORDCODE_JOIN CMD_JOIN
#define KEYWORDCODE_IMAX CMD_IMAX
#define KEYWORDCODE_IMIN CMD_IMIN
#define KEYWORDCODE_PACK CMD_PACK
#define KEYWORDCODE_TIME CMD_NOW
#define KEYWORDCODE_YEAR CMD_YEAR
#define KEYWORDCODE_HOUR CMD_HOUR
#define KEYWORDCODE_CONF CMD_CONF
#define KEYWORDCODE_KILL CMD_KILL
#define KEYWORDCODE_WEND 295
#define KEYWORDCODE_FILE 331
#define KEYWORDCODE_LOOP 298
#define KEYWORDCODE_SWAP 265
#define KEYWORDCODE_LINE 318
#define KEYWORDCODE_NAME 328
#define KEYWORDCODE_OPEN 309
#define KEYWORDCODE_FROM 323
#define KEYWORDCODE_WILD 343
#define KEYWORDCODE_THEN 303
#define KEYWORDCODE_NEXT 293
#define KEYWORDCODE_LOCK 321
#define KEYWORDCODE_EXIT 269
#define KEYWORDCODE_STEP 301
#define KEYWORDCODE_ELSE 305
#define KEYWORDCODE_NULL 291
#define KEYWORDCODE_GOTO 281
#define KEYWORDCODE_CALL 271
#define KEYWORDCODE_STOP 259
#define KEYWORDCODE_SEEK 316
#define KEYWORDCODE_AND CMD_AND
#define KEYWORDCODE_XOR CMD_XOR
#define KEYWORDCODE_NOT CMD_NOT
#define KEYWORDCODE_SIN CMD_SIN
#define KEYWORDCODE_COS CMD_COS
#define KEYWORDCODE_SGN CMD_SGN
#define KEYWORDCODE_ODD CMD_ODD
#define KEYWORDCODE_GCD CMD_GCD
#define KEYWORDCODE_LCM CMD_LCM
#define KEYWORDCODE_SQRT CMD_SQRT
#define CMD_RAD2DEG 355
void COMMAND_RAD2DEG(pExecuteObject);
#define CMD_DEG2RAD 356
void COMMAND_DEG2RAD(pExecuteObject);
#define KEYWORDCODE_RND CMD_RND
#define KEYWORDCODE_ABS CMD_ABS
#define KEYWORDCODE_VAL CMD_VAL
#define KEYWORDCODE_FIX CMD_FIX
#define KEYWORDCODE_INT CMD_INT
#define KEYWORDCODE_LOG CMD_LOG
#define KEYWORDCODE_POW CMD_POW
#define KEYWORDCODE_EXP CMD_EXP
#define KEYWORDCODE_LEN CMD_LEN
#define KEYWORDCODE_ASC CMD_ASC
#define KEYWORDCODE_MID CMD_MID
#define KEYWORDCODE_CHR CMD_CHR
#define KEYWORDCODE_STR CMD_STR
#define KEYWORDCODE_HEX CMD_HEX
#define KEYWORDCODE_OCT CMD_OCT
#define KEYWORDCODE_ATN CMD_ATN
#define KEYWORDCODE_TAN CMD_TAN
#define KEYWORDCODE_BIN CMD_BIN
#define KEYWORDCODE_MAX CMD_MAX
#define KEYWORDCODE_MIN CMD_MIN
#define KEYWORDCODE_LOC CMD_LOC
#define KEYWORDCODE_LOF CMD_LOF
#define KEYWORDCODE_EOF CMD_EOFFUN
#define KEYWORDCODE_EOD CMD_EODFUN
#define KEYWORDCODE_NOW CMD_NOW
#define KEYWORDCODE_DAY CMD_DAY
#define KEYWORDCODE_SEC CMD_SEC
#define KEYWORDCODE_REF 280
#define KEYWORDCODE_VAR 264
#define KEYWORDCODE_SET 330
#define KEYWORDCODE_POP 286
#define KEYWORDCODE_LET 273
#define KEYWORDCODE_DIM 272
#define KEYWORDCODE_FOR 300
#define KEYWORDCODE_END 257
#define KEYWORDCODE_LIB 348
#define KEYWORDCODE_SUB 270
#define KEYWORDCODE_OR CMD_OR
#define KEYWORDCODE_PI CMD_PI
#define KEYWORDCODE_IF 302
#define KEYWORDCODE_ON 289
#define KEYWORDCODE_DO 296
#define KEYWORDCODE_TO 283
#define KEYWORDCODE_NO 344
#define KEYWORDCODE_BY 336
#define KEYWORDCODE_GO 282
#define KEYWORDCODE_AS 312
#define KEYWORDCODE_GMTIMETOLOCALTIME CMD_GM2LOCAL
#define KEYWORDCODE_LOCALTIMETOGMTIME CMD_LOCAL2GM
#define KEYWORDCODE_FILEACCESSTIME CMD_FTACCESS
#define KEYWORDCODE_FILEMODIFYTIME CMD_FTMODIFY
#define KEYWORDCODE_FILECREATETIME CMD_FTCREATED
#define KEYWORDCODE_STRREVERSES CMD_STRREVERSE
#define KEYWORDCODE_ISDIRECTORY CMD_ISDIR
#define KEYWORDCODE_STRREVERSE CMD_STRREVERSE
#define KEYWORDCODE_FILEEXISTS CMD_FILEXISTS
#define KEYWORDCODE_FORMATDATE CMD_FORMATDATE
#define KEYWORDCODE_FORMATTIME CMD_FORMATDATE
extern PredLConst PREDLCONSTS[];
extern unsigned long BINARIES[];
extern unsigned long UNARIES[];
extern BFun INTERNALFUNCTIONS[];
extern LineSyntax COMMANDS[];
extern LexNASymbol NASYMBOLS[];
extern LexNASymbol ASYMBOLS[];
extern LexNASymbol CSYMBOLS[];
extern char *COMMANDSYMBOLS[];
extern CommandFunctionType CommandFunction[];
#define END_EXEC 894
#ifdef __cplusplus
}
#endif
#endif
