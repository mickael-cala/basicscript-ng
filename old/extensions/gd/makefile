


all : /workspace/old/bin/mod/lib/gd.a /workspace/old/bin/mod/dll/gd.so /workspace/old/bin/texi/mod_gd.texi




/workspace/old/bin/mod/lib/gd.a : /workspace/old/bin/mod/obj/gd/s_interface.o 
	ar -r /workspace/old/bin/mod/lib/gd.a /workspace/old/bin/mod/obj/gd/s_interface.o  

/workspace/old/bin/mod/dll/gd.so : /workspace/old/bin/mod/obj/gd/interface.o 
	ld -shared -Bstatic -o /workspace/old/bin/mod/dll/gd.so /workspace/old/bin/mod/obj/gd/interface.o -lc -lpng -lz -lgd

/workspace/old/bin/mod/obj/gd/interface.o : interface.c gdfontl.h gd.h gdfonts.h gdfontt.h gdfontg.h gdfontmb.h 
	gcc -w -c -o /workspace/old/bin/mod/obj/gd/interface.o interface.c

/workspace/old/bin/mod/obj/gd/s_interface.o : interface.c gdfontl.h gd.h gdfonts.h gdfontt.h gdfontg.h gdfontmb.h 
	gcc -w -DSTATIC_LINK=1 -c -o /workspace/old/bin/mod/obj/gd/s_interface.o interface.c

/workspace/old/bin/texi/mod_gd.texi : manual.texi.jam
	perl /workspace/old/jamal.pl manual.texi.jam /workspace/old/bin/texi/mod_gd.texi
	perl /workspace/old/t2h.pl /workspace/old/bin/texi/mod_gd.texi


