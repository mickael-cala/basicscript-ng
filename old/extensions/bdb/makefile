


all : /workspace/old/bin/mod/lib/bdb.a /workspace/old/bin/mod/dll/bdb.so /workspace/old/bin/texi/mod_bdb.texi




/workspace/old/bin/mod/lib/bdb.a : /workspace/old/bin/mod/obj/bdb/s_interface.o 
	ar -r /workspace/old/bin/mod/lib/bdb.a /workspace/old/bin/mod/obj/bdb/s_interface.o  

/workspace/old/bin/mod/dll/bdb.so : /workspace/old/bin/mod/obj/bdb/interface.o 
	ld -shared -Bstatic -o /workspace/old/bin/mod/dll/bdb.so /workspace/old/bin/mod/obj/bdb/interface.o -lbdb

/workspace/old/bin/mod/obj/bdb/interface.o : interface.c 
	gcc -w -c -o /workspace/old/bin/mod/obj/bdb/interface.o interface.c

/workspace/old/bin/mod/obj/bdb/s_interface.o : interface.c 
	gcc -w -DSTATIC_LINK=1 -c -o /workspace/old/bin/mod/obj/bdb/s_interface.o interface.c

/workspace/old/bin/texi/mod_bdb.texi : manual.texi.jam
	perl /workspace/old/jamal.pl manual.texi.jam /workspace/old/bin/texi/mod_bdb.texi
	perl /workspace/old/t2h.pl /workspace/old/bin/texi/mod_bdb.texi


