


all : /workspace/old/bin/mod/lib/hash.a /workspace/old/bin/mod/dll/hash.so /workspace/old/bin/texi/mod_hash.texi




/workspace/old/bin/mod/lib/hash.a : /workspace/old/bin/mod/obj/hash/s_interface.o 
	ar -r /workspace/old/bin/mod/lib/hash.a /workspace/old/bin/mod/obj/hash/s_interface.o  

/workspace/old/bin/mod/dll/hash.so : /workspace/old/bin/mod/obj/hash/interface.o 
	ld -shared -Bstatic -o /workspace/old/bin/mod/dll/hash.so /workspace/old/bin/mod/obj/hash/interface.o  

/workspace/old/bin/mod/obj/hash/interface.o : interface.c 
	gcc -w -c -o /workspace/old/bin/mod/obj/hash/interface.o interface.c

/workspace/old/bin/mod/obj/hash/s_interface.o : interface.c 
	gcc -w -DSTATIC_LINK=1 -c -o /workspace/old/bin/mod/obj/hash/s_interface.o interface.c

/workspace/old/bin/texi/mod_hash.texi : manual.texi.jam
	perl /workspace/old/jamal.pl manual.texi.jam /workspace/old/bin/texi/mod_hash.texi
	perl /workspace/old/t2h.pl /workspace/old/bin/texi/mod_hash.texi


