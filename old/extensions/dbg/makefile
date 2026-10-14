


all : /workspace/old/bin/mod/lib/dbg.a /workspace/old/bin/mod/dll/dbg.so /workspace/old/bin/texi/mod_dbg.texi




/workspace/old/bin/mod/lib/dbg.a : /workspace/old/bin/mod/obj/dbg/dbg_con.o /workspace/old/bin/mod/obj/dbg/s_interface.o 
	ar -r /workspace/old/bin/mod/lib/dbg.a /workspace/old/bin/mod/obj/dbg/dbg_con.o /workspace/old/bin/mod/obj/dbg/s_interface.o  

/workspace/old/bin/mod/dll/dbg.so : /workspace/old/bin/mod/obj/dbg/dbg_con.o /workspace/old/bin/mod/obj/dbg/interface.o 
	ld -shared -Bstatic -o /workspace/old/bin/mod/dll/dbg.so /workspace/old/bin/mod/obj/dbg/dbg_con.o /workspace/old/bin/mod/obj/dbg/interface.o  

/workspace/old/bin/mod/obj/dbg/dbg_con.o : dbg_con.c dbg.h 
	gcc -w -c -o /workspace/old/bin/mod/obj/dbg/dbg_con.o dbg_con.c

/workspace/old/bin/mod/obj/dbg/interface.o : interface.c dbg_comm.h dbg.h 
	gcc -w -c -o /workspace/old/bin/mod/obj/dbg/interface.o interface.c

/workspace/old/bin/mod/obj/dbg/s_interface.o : interface.c dbg_comm.h dbg.h 
	gcc -w -DSTATIC_LINK=1 -c -o /workspace/old/bin/mod/obj/dbg/s_interface.o interface.c

/workspace/old/bin/texi/mod_dbg.texi : manual.texi.jam
	perl /workspace/old/jamal.pl manual.texi.jam /workspace/old/bin/texi/mod_dbg.texi
	perl /workspace/old/t2h.pl /workspace/old/bin/texi/mod_dbg.texi


