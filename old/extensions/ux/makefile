


all : /workspace/old/bin/mod/lib/ux.a /workspace/old/bin/mod/dll/ux.so /workspace/old/bin/texi/mod_ux.texi




/workspace/old/bin/mod/lib/ux.a : /workspace/old/bin/mod/obj/ux/s_interface.o 
	ar -r /workspace/old/bin/mod/lib/ux.a /workspace/old/bin/mod/obj/ux/s_interface.o  

/workspace/old/bin/mod/dll/ux.so : /workspace/old/bin/mod/obj/ux/interface.o 
	ld -shared -Bstatic -o /workspace/old/bin/mod/dll/ux.so /workspace/old/bin/mod/obj/ux/interface.o  

/workspace/old/bin/mod/obj/ux/interface.o : interface.c 
	gcc -w -c -o /workspace/old/bin/mod/obj/ux/interface.o interface.c

/workspace/old/bin/mod/obj/ux/s_interface.o : interface.c 
	gcc -w -DSTATIC_LINK=1 -c -o /workspace/old/bin/mod/obj/ux/s_interface.o interface.c

/workspace/old/bin/texi/mod_ux.texi : manual.texi.jam
	perl /workspace/old/jamal.pl manual.texi.jam /workspace/old/bin/texi/mod_ux.texi
	perl /workspace/old/t2h.pl /workspace/old/bin/texi/mod_ux.texi


