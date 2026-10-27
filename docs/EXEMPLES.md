<!-- DEBUT EXEMPLES ENGENDRES - scripts/exemples.py -->

_Mesure du 2026-10-27 sur `windows-x64`, delai 5 s par exemple, configuration du depot fournie (include + modules construits), publication par `scripts/exemples.py --write`. Un exemple n'est jamais repare dans la copie de l'amont._

| verdict | exemples |
|---|---|
| `OK` | 87 |
| `module` | 16 |
| `include` | 48 |
| `analyse` | 12 |
| `interaction` | 9 |
| `execution` | 1 |
| `ecarte` | 6 |
| **total** | **179** |

`module` et `interaction` sont normaux (module non construit, saisie, reseau, serveur). `include` attend une declaration d'un module hors distribution (bdb, mysql...). Les `analyse` listent la face du langage que les exemples amont frappent - matiere de la passe langage (PLAN phase 8). Detail des exemples hors OK :

| exemple | verdict | detail |
|---|---|---|
| `examples/1runrestart.bas` | ecarte | redemarrage presume (nom) |
| `examples/bdbtransact.bas` | include | fichier d'inclusion absent |
| `examples/cgi-bin/echo.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\cgi-bin\echo.bas(2): err |
| `examples/cgi-bin/echoquerystring.bas` | module | module non construit |
| `examples/cgi-bin/getparam.bas` | module | module non construit |
| `examples/cgi-bin/listenv.bas` | module | module non construit |
| `examples/cgi-bin/mysqlcgi.bas` | include | fichier d'inclusion absent |
| `examples/cgi-bin/postparam.bas` | module | module non construit |
| `examples/cgi-proba.bas` | analyse | C:/Users/micka/4QWEN/scriptbasic2-ng/examples/CGI(0): error &H44:The f |
| `examples/cgigetex.bas` | module | module non construit |
| `examples/cgimodtest.bas` | module | module non construit |
| `examples/cgitest.bas` | module | module non construit |
| `examples/concol.bas` | interaction | > 5 s |
| `examples/debug.bas` | include | fichier d'inclusion absent |
| `examples/debugtest2.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\debugtest2.bas(1): error |
| `examples/echo.bas` | module | module non construit |
| `examples/echoquerystring.bas` | module | module non construit |
| `examples/external.bas` | analyse | (0): error &H10:The requested module can not be loaded. |
| `examples/gdtest.bas` | include | fichier d'inclusion absent |
| `examples/getparam.bas` | module | module non construit |
| `examples/hello.bas` | include | fichier d'inclusion absent |
| `examples/hello_cgi.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\hello_cgi.bas(4): error  |
| `examples/kakukk.bas` | analyse | (0): error &H16:The file can not be opened. |
| `examples/listenv.bas` | module | module non construit |
| `examples/mandel1.bas` | include | fichier d'inclusion absent |
| `examples/math.bas` | analyse | (0): error &H3:Division by zero or other calculation error |
| `examples/msgbox.bas` | ecarte | popup GUI bloquante |
| `examples/mt.bas` | module | module non construit |
| `examples/mtlock.bas` | module | module non construit |
| `examples/mysqlcgi.bas` | include | fichier d'inclusion absent |
| `examples/postparam.bas` | module | module non construit |
| `examples/reftest5.bas` | analyse | (0): error &H2:Function can not return a whole array |
| `examples/runrestart.bas` | ecarte | redemarrage presume (nom) |
| `examples/sibawa/cgi-bin/addacl.bas` | include | fichier d'inclusion absent |
| `examples/sibawa/cgi-bin/addacltype.bas` | include | fichier d'inclusion absent |
| `examples/sibawa/cgi-bin/addgroup.bas` | include | fichier d'inclusion absent |
| `examples/sibawa/cgi-bin/addrole.bas` | include | fichier d'inclusion absent |
| `examples/sibawa/cgi-bin/adduser.bas` | include | fichier d'inclusion absent |
| `examples/sibawa/cgi-bin/chpass.bas` | include | fichier d'inclusion absent |
| `examples/sibawa/cgi-bin/delacl.bas` | include | fichier d'inclusion absent |
| `examples/sibawa/cgi-bin/delacltype.bas` | include | fichier d'inclusion absent |
| `examples/sibawa/cgi-bin/delgroup.bas` | include | fichier d'inclusion absent |
| `examples/sibawa/cgi-bin/delrole.bas` | include | fichier d'inclusion absent |
| `examples/sibawa/cgi-bin/deluser.bas` | include | fichier d'inclusion absent |
| `examples/sibawa/cgi-bin/descacltbit.bas` | include | fichier d'inclusion absent |
| `examples/sibawa/cgi-bin/editacl.bas` | include | fichier d'inclusion absent |
| `examples/sibawa/cgi-bin/editacltype.bas` | include | fichier d'inclusion absent |
| `examples/sibawa/cgi-bin/editgroup.bas` | include | fichier d'inclusion absent |
| `examples/sibawa/cgi-bin/editrole.bas` | include | fichier d'inclusion absent |
| `examples/sibawa/cgi-bin/edituser.bas` | include | fichier d'inclusion absent |
| `examples/sibawa/cgi-bin/listacls.bas` | include | fichier d'inclusion absent |
| `examples/sibawa/cgi-bin/listacltypes.bas` | include | fichier d'inclusion absent |
| `examples/sibawa/cgi-bin/listgroups.bas` | include | fichier d'inclusion absent |
| `examples/sibawa/cgi-bin/listroles.bas` | include | fichier d'inclusion absent |
| `examples/sibawa/cgi-bin/listusers.bas` | include | fichier d'inclusion absent |
| `examples/sibawa/cgi-bin/logout.bas` | include | fichier d'inclusion absent |
| `examples/sibawa/cgi-bin/newacl.bas` | include | fichier d'inclusion absent |
| `examples/sibawa/cgi-bin/newacltype.bas` | include | fichier d'inclusion absent |
| `examples/sibawa/cgi-bin/newgroup.bas` | include | fichier d'inclusion absent |
| `examples/sibawa/cgi-bin/newrole.bas` | include | fichier d'inclusion absent |
| `examples/sibawa/cgi-bin/newuser.bas` | include | fichier d'inclusion absent |
| `examples/sibawa/cgi-bin/setacl.bas` | include | fichier d'inclusion absent |
| `examples/sibawa/cgi-bin/setacltype.bas` | include | fichier d'inclusion absent |
| `examples/sibawa/cgi-bin/setgroup.bas` | include | fichier d'inclusion absent |
| `examples/sibawa/cgi-bin/setrole.bas` | include | fichier d'inclusion absent |
| `examples/sibawa/cgi-bin/setuser.bas` | include | fichier d'inclusion absent |
| `examples/sibawa/cgi-bin/sibawa.bas` | include | fichier d'inclusion absent |
| `examples/sibawa/create_tables.bas` | include | fichier d'inclusion absent |
| `examples/sibawa/sibawastart.bas` | include | fichier d'inclusion absent |
| `examples/sleep.bas` | interaction | > 5 s |
| `examples/test_ext_trial.bas` | analyse | (0): error &H10:The requested module can not be loaded. |
| `examples/test_sleep.bas` | interaction | > 5 s |
| `examples/testbdb.bas` | include | fichier d'inclusion absent |
| `examples/testbdbtransaction.bas` | include | fichier d'inclusion absent |
| `examples/testcio.bas` | interaction | > 5 s |
| `examples/testconcol.bas` | interaction | > 5 s |
| `examples/testfork.bas` | include | fichier d'inclusion absent |
| `examples/testhostname.bas` | interaction | > 5 s |
| `examples/testhttp.bas` | interaction | > 5 s |
| `examples/testjapi.bas` | include | fichier d'inclusion absent |
| `examples/testmathop.bas` | analyse | (0): error &H68:The user function "sqr" is used but is not defined. |
| `examples/testmodl.bas` | analyse | (0): error &H10:The requested module can not be loaded. |
| `examples/testmsgbox.bas` | ecarte | popup GUI bloquante |
| `examples/testmt.bas` | module | module non construit |
| `examples/testmtlock.bas` | module | module non construit |
| `examples/testmysql.bas` | include | fichier d'inclusion absent |
| `examples/testnt.bas` | execution | code 528385 |
| `examples/testpow.bas` | analyse | (0): error &H68:The user function "sqr" is used but is not defined. |
| `examples/testshutdown.bas` | ecarte | arret machine presume (nom) |
| `examples/teststservice.bas` | ecarte | services Windows (nom) |
| `examples/testtime.bas` | interaction | > 5 s |
| `examples/time.bas` | interaction | > 5 s |

<!-- FIN EXEMPLES ENGENDRES -->
