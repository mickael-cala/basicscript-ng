


all : /workspace/old/bin/mod/lib/curses.a /workspace/old/bin/mod/dll/curses.so /workspace/old/bin/texi/mod_curses.texi




/workspace/old/bin/mod/lib/curses.a : /workspace/old/bin/mod/obj/curses/s_interface.o 
	ar -r /workspace/old/bin/mod/lib/curses.a /workspace/old/bin/mod/obj/curses/s_interface.o  

/workspace/old/bin/mod/dll/curses.so : /workspace/old/bin/mod/obj/curses/interface.o 
	ld -shared -Bstatic -o /workspace/old/bin/mod/dll/curses.so /workspace/old/bin/mod/obj/curses/interface.o -lcurses

/workspace/old/bin/mod/obj/curses/interface.o : interface.c 
	gcc -w -c -o /workspace/old/bin/mod/obj/curses/interface.o interface.c

/workspace/old/bin/mod/obj/curses/s_interface.o : interface.c 
	gcc -w -DSTATIC_LINK=1 -c -o /workspace/old/bin/mod/obj/curses/s_interface.o interface.c

/workspace/old/bin/texi/mod_curses.texi : manual.texi.jam
	perl /workspace/old/jamal.pl manual.texi.jam /workspace/old/bin/texi/mod_curses.texi
	perl /workspace/old/t2h.pl /workspace/old/bin/texi/mod_curses.texi


