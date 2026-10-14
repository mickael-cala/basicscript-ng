


all : /workspace/old/bin/mod/lib/mysql.a /workspace/old/bin/mod/dll/mysql.so /workspace/old/bin/texi/mod_mysql.texi




/workspace/old/bin/mod/lib/mysql.a : /workspace/old/bin/mod/obj/mysql/s_interface.o 
	ar -r /workspace/old/bin/mod/lib/mysql.a /workspace/old/bin/mod/obj/mysql/s_interface.o  

/workspace/old/bin/mod/dll/mysql.so : /workspace/old/bin/mod/obj/mysql/interface.o 
	ld -shared -Bstatic -o /workspace/old/bin/mod/dll/mysql.so /workspace/old/bin/mod/obj/mysql/interface.o -lmysqlclient

/workspace/old/bin/mod/obj/mysql/interface.o : interface.c 
	gcc -w -c -o /workspace/old/bin/mod/obj/mysql/interface.o interface.c

/workspace/old/bin/mod/obj/mysql/s_interface.o : interface.c 
	gcc -w -DSTATIC_LINK=1 -c -o /workspace/old/bin/mod/obj/mysql/s_interface.o interface.c

/workspace/old/bin/texi/mod_mysql.texi : manual.texi.jam
	perl /workspace/old/jamal.pl manual.texi.jam /workspace/old/bin/texi/mod_mysql.texi
	perl /workspace/old/t2h.pl /workspace/old/bin/texi/mod_mysql.texi


