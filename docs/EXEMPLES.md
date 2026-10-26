<!-- DEBUT EXEMPLES ENGENDRES - scripts/exemples.py -->

_Mesure du 2026-10-26 sur `windows-x64`, delai 5 s par exemple, publication par `scripts/exemples.py --write`. Un exemple n'est jamais repare dans la copie de l'amont._

| verdict | exemples |
|---|---|
| `OK` | 83 |
| `module` | 0 |
| `analyse` | 90 |
| `interaction` | 7 |
| `execution` | 0 |
| **total** | **180** |

Les verdicts `module` et `interaction` sont normaux : ils attendent un module non construit, une saisie, un reseau ou un serveur. Les `analyse` listent la face du langage que les exemples amont frappent - matiere de la passe langage (PLAN phase 8). Detail par exemple :

| exemple | verdict | detail |
|---|---|---|
| `examples/1runrestart.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\1runrestart.bas(1): erro |
| `examples/bdbtransact.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\bdbtransact.bas(1): erro |
| `examples/cgi-bin/echo.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\cgi-bin\echo.bas(6): err |
| `examples/cgi-bin/echoquerystring.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\cgi-bin\echoquerystring. |
| `examples/cgi-bin/getparam.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\cgi-bin\getparam.bas(2): |
| `examples/cgi-bin/listenv.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\cgi-bin\listenv.bas(2):  |
| `examples/cgi-bin/mysqlcgi.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\cgi-bin\mysqlcgi.bas(1): |
| `examples/cgi-bin/postparam.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\cgi-bin\postparam.bas(2) |
| `examples/cgi-proba.bas` | analyse | C:/Users/micka/4QWEN/scriptbasic2-ng/examples/CGI(0): error &H44:The f |
| `examples/cgigetex.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\cgigetex.bas(1): error & |
| `examples/cgimodtest.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\cgimodtest.bas(1): error |
| `examples/cgitest.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\cgitest.bas(2): error &H |
| `examples/concol.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\concol.bas(1): error &H3 |
| `examples/curltest.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\curltest.bas(1): error & |
| `examples/debug.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\debug.bas(1): error &H3e |
| `examples/debugtest2.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\debugtest2.bas(1): error |
| `examples/echo.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\echo.bas(5): error &H3e: |
| `examples/echoquerystring.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\echoquerystring.bas(2):  |
| `examples/external.bas` | analyse | (0): error &H10:The requested module can not be loaded. |
| `examples/gdtest.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\gdtest.bas(1): error &H3 |
| `examples/getparam.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\getparam.bas(2): error & |
| `examples/hash.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\hash.bas(1): error &H3e: |
| `examples/hello.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\hello.bas(1): error &H3e |
| `examples/hello_cgi.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\hello_cgi.bas(4): error  |
| `examples/kakukk.bas` | analyse | (0): error &H16:The file can not be opened. |
| `examples/listenv.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\listenv.bas(2): error &H |
| `examples/mandel1.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\mandel1.bas(1): error &H |
| `examples/msgbox.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\msgbox.bas(1): error &H3 |
| `examples/mt.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\mt.bas(5): error &H3e:In |
| `examples/mtlock.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\mtlock.bas(11): error &H |
| `examples/mysqlcgi.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\mysqlcgi.bas(1): error & |
| `examples/postparam.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\postparam.bas(2): error  |
| `examples/reftest5.bas` | analyse | (0): error &H2:Function can not return a whole array |
| `examples/runrestart.bas` | interaction | > 5 s |
| `examples/sibawa/cgi-bin/addacl.bas` | analyse | C:/Users/micka/4QWEN/scriptbasic2-ng/examples/sibawa/sibawa.ini(6): er |
| `examples/sibawa/cgi-bin/addacltype.bas` | analyse | C:/Users/micka/4QWEN/scriptbasic2-ng/examples/sibawa/sibawa.ini(6): er |
| `examples/sibawa/cgi-bin/addgroup.bas` | analyse | C:/Users/micka/4QWEN/scriptbasic2-ng/examples/sibawa/sibawa.ini(6): er |
| `examples/sibawa/cgi-bin/addrole.bas` | analyse | C:/Users/micka/4QWEN/scriptbasic2-ng/examples/sibawa/sibawa.ini(6): er |
| `examples/sibawa/cgi-bin/adduser.bas` | analyse | C:/Users/micka/4QWEN/scriptbasic2-ng/examples/sibawa/sibawa.ini(6): er |
| `examples/sibawa/cgi-bin/chpass.bas` | analyse | C:/Users/micka/4QWEN/scriptbasic2-ng/examples/sibawa/sibawa.ini(6): er |
| `examples/sibawa/cgi-bin/delacl.bas` | analyse | C:/Users/micka/4QWEN/scriptbasic2-ng/examples/sibawa/sibawa.ini(6): er |
| `examples/sibawa/cgi-bin/delacltype.bas` | analyse | C:/Users/micka/4QWEN/scriptbasic2-ng/examples/sibawa/sibawa.ini(6): er |
| `examples/sibawa/cgi-bin/delgroup.bas` | analyse | C:/Users/micka/4QWEN/scriptbasic2-ng/examples/sibawa/sibawa.ini(6): er |
| `examples/sibawa/cgi-bin/delrole.bas` | analyse | C:/Users/micka/4QWEN/scriptbasic2-ng/examples/sibawa/sibawa.ini(6): er |
| `examples/sibawa/cgi-bin/deluser.bas` | analyse | C:/Users/micka/4QWEN/scriptbasic2-ng/examples/sibawa/sibawa.ini(6): er |
| `examples/sibawa/cgi-bin/descacltbit.bas` | analyse | C:/Users/micka/4QWEN/scriptbasic2-ng/examples/sibawa/sibawa.ini(6): er |
| `examples/sibawa/cgi-bin/editacl.bas` | analyse | C:/Users/micka/4QWEN/scriptbasic2-ng/examples/sibawa/sibawa.ini(6): er |
| `examples/sibawa/cgi-bin/editacltype.bas` | analyse | C:/Users/micka/4QWEN/scriptbasic2-ng/examples/sibawa/sibawa.ini(6): er |
| `examples/sibawa/cgi-bin/editgroup.bas` | analyse | C:/Users/micka/4QWEN/scriptbasic2-ng/examples/sibawa/sibawa.ini(6): er |
| `examples/sibawa/cgi-bin/editrole.bas` | analyse | C:/Users/micka/4QWEN/scriptbasic2-ng/examples/sibawa/sibawa.ini(6): er |
| `examples/sibawa/cgi-bin/edituser.bas` | analyse | C:/Users/micka/4QWEN/scriptbasic2-ng/examples/sibawa/sibawa.ini(6): er |
| `examples/sibawa/cgi-bin/listacls.bas` | analyse | C:/Users/micka/4QWEN/scriptbasic2-ng/examples/sibawa/sibawa.ini(6): er |
| `examples/sibawa/cgi-bin/listacltypes.bas` | analyse | C:/Users/micka/4QWEN/scriptbasic2-ng/examples/sibawa/sibawa.ini(6): er |
| `examples/sibawa/cgi-bin/listgroups.bas` | analyse | C:/Users/micka/4QWEN/scriptbasic2-ng/examples/sibawa/sibawa.ini(6): er |
| `examples/sibawa/cgi-bin/listroles.bas` | analyse | C:/Users/micka/4QWEN/scriptbasic2-ng/examples/sibawa/sibawa.ini(6): er |
| `examples/sibawa/cgi-bin/listusers.bas` | analyse | C:/Users/micka/4QWEN/scriptbasic2-ng/examples/sibawa/sibawa.ini(6): er |
| `examples/sibawa/cgi-bin/logout.bas` | analyse | C:/Users/micka/4QWEN/scriptbasic2-ng/examples/sibawa/sibawa.ini(6): er |
| `examples/sibawa/cgi-bin/newacl.bas` | analyse | C:/Users/micka/4QWEN/scriptbasic2-ng/examples/sibawa/sibawa.ini(6): er |
| `examples/sibawa/cgi-bin/newacltype.bas` | analyse | C:/Users/micka/4QWEN/scriptbasic2-ng/examples/sibawa/sibawa.ini(6): er |
| `examples/sibawa/cgi-bin/newgroup.bas` | analyse | C:/Users/micka/4QWEN/scriptbasic2-ng/examples/sibawa/sibawa.ini(6): er |
| `examples/sibawa/cgi-bin/newrole.bas` | analyse | C:/Users/micka/4QWEN/scriptbasic2-ng/examples/sibawa/sibawa.ini(6): er |
| `examples/sibawa/cgi-bin/newuser.bas` | analyse | C:/Users/micka/4QWEN/scriptbasic2-ng/examples/sibawa/sibawa.ini(6): er |
| `examples/sibawa/cgi-bin/setacl.bas` | analyse | C:/Users/micka/4QWEN/scriptbasic2-ng/examples/sibawa/sibawa.ini(6): er |
| `examples/sibawa/cgi-bin/setacltype.bas` | analyse | C:/Users/micka/4QWEN/scriptbasic2-ng/examples/sibawa/sibawa.ini(6): er |
| `examples/sibawa/cgi-bin/setgroup.bas` | analyse | C:/Users/micka/4QWEN/scriptbasic2-ng/examples/sibawa/sibawa.ini(6): er |
| `examples/sibawa/cgi-bin/setrole.bas` | analyse | C:/Users/micka/4QWEN/scriptbasic2-ng/examples/sibawa/sibawa.ini(6): er |
| `examples/sibawa/cgi-bin/setuser.bas` | analyse | C:/Users/micka/4QWEN/scriptbasic2-ng/examples/sibawa/sibawa.ini(6): er |
| `examples/sibawa/cgi-bin/sibawa.bas` | analyse | C:/Users/micka/4QWEN/scriptbasic2-ng/examples/sibawa/sibawa.ini(6): er |
| `examples/sibawa/create_tables.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\sibawa\create_tables.bas |
| `examples/sibawa/sibawastart.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\sibawa\sibawastart.bas(9 |
| `examples/sleep.bas` | interaction | > 5 s |
| `examples/test_ext_trial.bas` | analyse | (0): error &H10:The requested module can not be loaded. |
| `examples/test_sleep.bas` | interaction | > 5 s |
| `examples/testbdb.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\testbdb.bas(1): error &H |
| `examples/testbdbtransaction.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\testbdbtransaction.bas(1 |
| `examples/testcio.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\testcio.bas(1): error &H |
| `examples/testconcol.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\testconcol.bas(6): error |
| `examples/testfork.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\testfork.bas(1): error & |
| `examples/testhash.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\testhash.bas(1): error & |
| `examples/testhostname.bas` | interaction | > 5 s |
| `examples/testhttp.bas` | interaction | > 5 s |
| `examples/testjapi.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\testjapi.bas(1): error & |
| `examples/testmathop.bas` | analyse | (0): error &H68:The user function "sqr" is used but is not defined. |
| `examples/testmodl.bas` | analyse | (0): error &H10:The requested module can not be loaded. |
| `examples/testmsgbox.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\testmsgbox.bas(1): error |
| `examples/testmt.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\testmt.bas(5): error &H3 |
| `examples/testmtlock.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\testmtlock.bas(11): erro |
| `examples/testmysql.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\testmysql.bas(1): error  |
| `examples/testnt.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\testnt.bas(1): error &H3 |
| `examples/testpow.bas` | analyse | (0): error &H68:The user function "sqr" is used but is not defined. |
| `examples/testproclist.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\testproclist.bas(1): err |
| `examples/testre.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\testre.bas(1): error &H3 |
| `examples/testshutdown.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\testshutdown.bas(1): err |
| `examples/teststservice.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\teststservice.bas(1): er |
| `examples/testtime.bas` | interaction | > 5 s |
| `examples/time.bas` | interaction | > 5 s |
| `examples/toolstest.bas` | analyse | C:\Users\micka\4QWEN\scriptbasic2-ng\examples\toolstest.bas(1): error  |

<!-- FIN EXEMPLES ENGENDRES -->
