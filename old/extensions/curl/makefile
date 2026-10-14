


all : /workspace/old/bin/mod/lib/curl.a /workspace/old/bin/mod/dll/curl.so /workspace/old/bin/texi/mod_curl.texi




/workspace/old/bin/mod/lib/curl.a : /workspace/old/bin/mod/obj/curl/s_interface.o 
	ar -r /workspace/old/bin/mod/lib/curl.a /workspace/old/bin/mod/obj/curl/s_interface.o  

/workspace/old/bin/mod/dll/curl.so : /workspace/old/bin/mod/obj/curl/interface.o 
	ld -shared -Bstatic -o /workspace/old/bin/mod/dll/curl.so /workspace/old/bin/mod/obj/curl/interface.o -lcurl -lssl -lcrypto -ldl -lc

/workspace/old/bin/mod/obj/curl/interface.o : interface.c 
	gcc -w -c -o /workspace/old/bin/mod/obj/curl/interface.o interface.c

/workspace/old/bin/mod/obj/curl/s_interface.o : interface.c 
	gcc -w -DSTATIC_LINK=1 -c -o /workspace/old/bin/mod/obj/curl/s_interface.o interface.c

/workspace/old/bin/texi/mod_curl.texi : manual.texi.jam
	perl /workspace/old/jamal.pl manual.texi.jam /workspace/old/bin/texi/mod_curl.texi
	perl /workspace/old/t2h.pl /workspace/old/bin/texi/mod_curl.texi


