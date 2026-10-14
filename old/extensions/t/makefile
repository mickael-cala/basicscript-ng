


all : /workspace/old/bin/mod/lib/t.a /workspace/old/bin/mod/dll/t.so /workspace/old/bin/texi/mod_t.texi




/workspace/old/bin/mod/lib/t.a : /workspace/old/bin/mod/obj/t/s_interface.o 
	ar -r /workspace/old/bin/mod/lib/t.a /workspace/old/bin/mod/obj/t/s_interface.o  

/workspace/old/bin/mod/dll/t.so : /workspace/old/bin/mod/obj/t/interface.o 
	ld -shared -Bstatic -o /workspace/old/bin/mod/dll/t.so /workspace/old/bin/mod/obj/t/interface.o  

/workspace/old/bin/mod/obj/t/interface.o : interface.c 
	gcc -w -c -o /workspace/old/bin/mod/obj/t/interface.o interface.c

/workspace/old/bin/mod/obj/t/s_interface.o : interface.c 
	gcc -w -DSTATIC_LINK=1 -c -o /workspace/old/bin/mod/obj/t/s_interface.o interface.c

/workspace/old/bin/texi/mod_t.texi : manual.texi.jam
	perl /workspace/old/jamal.pl manual.texi.jam /workspace/old/bin/texi/mod_t.texi
	perl /workspace/old/t2h.pl /workspace/old/bin/texi/mod_t.texi


