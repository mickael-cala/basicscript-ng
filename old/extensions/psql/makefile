


all : /workspace/old/bin/mod/lib/psql.a /workspace/old/bin/mod/dll/psql.so /workspace/old/bin/texi/mod_psql.texi




/workspace/old/bin/mod/lib/psql.a : /workspace/old/bin/mod/obj/psql/s_interface.o 
	ar -r /workspace/old/bin/mod/lib/psql.a /workspace/old/bin/mod/obj/psql/s_interface.o  

/workspace/old/bin/mod/dll/psql.so : /workspace/old/bin/mod/obj/psql/interface.o 
	ld -shared -Bstatic -o /workspace/old/bin/mod/dll/psql.so /workspace/old/bin/mod/obj/psql/interface.o -lpq

/workspace/old/bin/mod/obj/psql/interface.o : interface.c 
	gcc -w -c -o /workspace/old/bin/mod/obj/psql/interface.o interface.c

/workspace/old/bin/mod/obj/psql/s_interface.o : interface.c 
	gcc -w -DSTATIC_LINK=1 -c -o /workspace/old/bin/mod/obj/psql/s_interface.o interface.c

/workspace/old/bin/texi/mod_psql.texi : manual.texi.jam
	perl /workspace/old/jamal.pl manual.texi.jam /workspace/old/bin/texi/mod_psql.texi
	perl /workspace/old/t2h.pl /workspace/old/bin/texi/mod_psql.texi


