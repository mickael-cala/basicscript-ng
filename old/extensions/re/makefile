


all : /workspace/old/bin/mod/lib/re.a /workspace/old/bin/mod/dll/re.so /workspace/old/bin/texi/mod_re.texi




/workspace/old/bin/mod/lib/re.a : /workspace/old/bin/mod/obj/re/s_interface.o /workspace/old/bin/mod/obj/re/regcomp.o /workspace/old/bin/mod/obj/re/regerror.o /workspace/old/bin/mod/obj/re/regex_test.o /workspace/old/bin/mod/obj/re/regexec.o /workspace/old/bin/mod/obj/re/regfree.o 
	ar -r /workspace/old/bin/mod/lib/re.a /workspace/old/bin/mod/obj/re/s_interface.o /workspace/old/bin/mod/obj/re/regcomp.o /workspace/old/bin/mod/obj/re/regerror.o /workspace/old/bin/mod/obj/re/regex_test.o /workspace/old/bin/mod/obj/re/regexec.o /workspace/old/bin/mod/obj/re/regfree.o  

/workspace/old/bin/mod/dll/re.so : /workspace/old/bin/mod/obj/re/interface.o /workspace/old/bin/mod/obj/re/regcomp.o /workspace/old/bin/mod/obj/re/regerror.o /workspace/old/bin/mod/obj/re/regex_test.o /workspace/old/bin/mod/obj/re/regexec.o /workspace/old/bin/mod/obj/re/regfree.o 
	ld -shared -Bstatic -o /workspace/old/bin/mod/dll/re.so /workspace/old/bin/mod/obj/re/interface.o /workspace/old/bin/mod/obj/re/regcomp.o /workspace/old/bin/mod/obj/re/regerror.o /workspace/old/bin/mod/obj/re/regex_test.o /workspace/old/bin/mod/obj/re/regexec.o /workspace/old/bin/mod/obj/re/regfree.o  

/workspace/old/bin/mod/obj/re/interface.o : interface.c regex.h 
	gcc -w -c -o /workspace/old/bin/mod/obj/re/interface.o interface.c

/workspace/old/bin/mod/obj/re/s_interface.o : interface.c regex.h 
	gcc -w -DSTATIC_LINK=1 -c -o /workspace/old/bin/mod/obj/re/s_interface.o interface.c

/workspace/old/bin/mod/obj/re/regcomp.o : regcomp.c cclass.h regex2.h regex.h utils.h cname.h 
	gcc -w -c -o /workspace/old/bin/mod/obj/re/regcomp.o regcomp.c

/workspace/old/bin/mod/obj/re/regerror.o : regerror.c regex.h utils.h 
	gcc -w -c -o /workspace/old/bin/mod/obj/re/regerror.o regerror.c

/workspace/old/bin/mod/obj/re/regex_test.o : regex_test.c regex.h 
	gcc -w -c -o /workspace/old/bin/mod/obj/re/regex_test.o regex_test.c

/workspace/old/bin/mod/obj/re/regexec.o : regexec.c regex2.h regex.h utils.h 
	gcc -w -c -o /workspace/old/bin/mod/obj/re/regexec.o regexec.c

/workspace/old/bin/mod/obj/re/regfree.o : regfree.c regex2.h regex.h utils.h 
	gcc -w -c -o /workspace/old/bin/mod/obj/re/regfree.o regfree.c

/workspace/old/bin/texi/mod_re.texi : manual.texi.jam
	perl /workspace/old/jamal.pl manual.texi.jam /workspace/old/bin/texi/mod_re.texi
	perl /workspace/old/t2h.pl /workspace/old/bin/texi/mod_re.texi


