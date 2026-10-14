


all : /workspace/old/bin/mod/lib/xml.a /workspace/old/bin/mod/dll/xml.so /workspace/old/bin/texi/mod_xml.texi




/workspace/old/bin/mod/lib/xml.a : /workspace/old/bin/mod/obj/xml/s_interface.o 
	ar -r /workspace/old/bin/mod/lib/xml.a /workspace/old/bin/mod/obj/xml/s_interface.o  

/workspace/old/bin/mod/dll/xml.so : /workspace/old/bin/mod/obj/xml/interface.o 
	ld -shared -Bstatic -o /workspace/old/bin/mod/dll/xml.so /workspace/old/bin/mod/obj/xml/interface.o -lxml2

/workspace/old/bin/mod/obj/xml/interface.o : interface.c 
	gcc -w -c -o /workspace/old/bin/mod/obj/xml/interface.o interface.c

/workspace/old/bin/mod/obj/xml/s_interface.o : interface.c 
	gcc -w -DSTATIC_LINK=1 -c -o /workspace/old/bin/mod/obj/xml/s_interface.o interface.c

/workspace/old/bin/texi/mod_xml.texi : manual.texi.jam
	perl /workspace/old/jamal.pl manual.texi.jam /workspace/old/bin/texi/mod_xml.texi
	perl /workspace/old/t2h.pl /workspace/old/bin/texi/mod_xml.texi


