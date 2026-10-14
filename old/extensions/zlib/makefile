


all : /workspace/old/bin/mod/lib/zlib.a /workspace/old/bin/mod/dll/zlib.so /workspace/old/bin/texi/mod_zlib.texi




/workspace/old/bin/mod/lib/zlib.a : /workspace/old/bin/mod/obj/zlib/s_interface.o 
	ar -r /workspace/old/bin/mod/lib/zlib.a /workspace/old/bin/mod/obj/zlib/s_interface.o  

/workspace/old/bin/mod/dll/zlib.so : /workspace/old/bin/mod/obj/zlib/interface.o 
	ld -shared -Bstatic -o /workspace/old/bin/mod/dll/zlib.so /workspace/old/bin/mod/obj/zlib/interface.o -lc -lz

/workspace/old/bin/mod/obj/zlib/interface.o : interface.c zlib.h 
	gcc -w -c -o /workspace/old/bin/mod/obj/zlib/interface.o interface.c

/workspace/old/bin/mod/obj/zlib/s_interface.o : interface.c zlib.h 
	gcc -w -DSTATIC_LINK=1 -c -o /workspace/old/bin/mod/obj/zlib/s_interface.o interface.c

/workspace/old/bin/texi/mod_zlib.texi : manual.texi.jam
	perl /workspace/old/jamal.pl manual.texi.jam /workspace/old/bin/texi/mod_zlib.texi
	perl /workspace/old/t2h.pl /workspace/old/bin/texi/mod_zlib.texi


