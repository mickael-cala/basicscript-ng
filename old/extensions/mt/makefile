


all : /workspace/old/bin/mod/lib/mt.a /workspace/old/bin/mod/dll/mt.so /workspace/old/bin/texi/mod_mt.texi




/workspace/old/bin/mod/lib/mt.a : /workspace/old/bin/mod/obj/mt/s_interface.o 
	ar -r /workspace/old/bin/mod/lib/mt.a /workspace/old/bin/mod/obj/mt/s_interface.o  

/workspace/old/bin/mod/dll/mt.so : /workspace/old/bin/mod/obj/mt/interface.o 
	ld -shared -Bstatic -o /workspace/old/bin/mod/dll/mt.so /workspace/old/bin/mod/obj/mt/interface.o  

/workspace/old/bin/mod/obj/mt/interface.o : interface.c 
	gcc -w -c -o /workspace/old/bin/mod/obj/mt/interface.o interface.c

/workspace/old/bin/mod/obj/mt/s_interface.o : interface.c 
	gcc -w -DSTATIC_LINK=1 -c -o /workspace/old/bin/mod/obj/mt/s_interface.o interface.c

/workspace/old/bin/texi/mod_mt.texi : manual.texi.jam
	perl /workspace/old/jamal.pl manual.texi.jam /workspace/old/bin/texi/mod_mt.texi
	perl /workspace/old/t2h.pl /workspace/old/bin/texi/mod_mt.texi


