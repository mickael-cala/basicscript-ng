


all : /workspace/old/bin/mod/lib/cgi.a /workspace/old/bin/mod/dll/cgi.so /workspace/old/bin/texi/mod_cgi.texi




/workspace/old/bin/mod/lib/cgi.a : /workspace/old/bin/mod/obj/cgi/cgi.o /workspace/old/bin/mod/obj/cgi/s_interface.o 
	ar -r /workspace/old/bin/mod/lib/cgi.a /workspace/old/bin/mod/obj/cgi/cgi.o /workspace/old/bin/mod/obj/cgi/s_interface.o  

/workspace/old/bin/mod/dll/cgi.so : /workspace/old/bin/mod/obj/cgi/cgi.o /workspace/old/bin/mod/obj/cgi/interface.o 
	ld -shared -Bstatic -o /workspace/old/bin/mod/dll/cgi.so /workspace/old/bin/mod/obj/cgi/cgi.o /workspace/old/bin/mod/obj/cgi/interface.o  

/workspace/old/bin/mod/obj/cgi/cgi.o : cgi.c cgi.h 
	gcc -w -c -o /workspace/old/bin/mod/obj/cgi/cgi.o cgi.c

/workspace/old/bin/mod/obj/cgi/interface.o : interface.c cgi.h 
	gcc -w -c -o /workspace/old/bin/mod/obj/cgi/interface.o interface.c

/workspace/old/bin/mod/obj/cgi/s_interface.o : interface.c cgi.h 
	gcc -w -DSTATIC_LINK=1 -c -o /workspace/old/bin/mod/obj/cgi/s_interface.o interface.c

/workspace/old/bin/texi/mod_cgi.texi : manual.texi.jam
	perl /workspace/old/jamal.pl manual.texi.jam /workspace/old/bin/texi/mod_cgi.texi
	perl /workspace/old/t2h.pl /workspace/old/bin/texi/mod_cgi.texi


