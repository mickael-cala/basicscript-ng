


all : /workspace/old/bin/mod/lib/trial.a /workspace/old/bin/mod/dll/trial.so /workspace/old/bin/texi/mod_trial.texi




/workspace/old/bin/mod/lib/trial.a : /workspace/old/bin/mod/obj/trial/s_interface.o 
	ar -r /workspace/old/bin/mod/lib/trial.a /workspace/old/bin/mod/obj/trial/s_interface.o  

/workspace/old/bin/mod/dll/trial.so : /workspace/old/bin/mod/obj/trial/interface.o 
	ld -shared -Bstatic -o /workspace/old/bin/mod/dll/trial.so /workspace/old/bin/mod/obj/trial/interface.o  

/workspace/old/bin/mod/obj/trial/interface.o : interface.c 
	gcc -w -c -o /workspace/old/bin/mod/obj/trial/interface.o interface.c

/workspace/old/bin/mod/obj/trial/s_interface.o : interface.c 
	gcc -w -DSTATIC_LINK=1 -c -o /workspace/old/bin/mod/obj/trial/s_interface.o interface.c

/workspace/old/bin/texi/mod_trial.texi : manual.texi.jam
	perl /workspace/old/jamal.pl manual.texi.jam /workspace/old/bin/texi/mod_trial.texi
	perl /workspace/old/t2h.pl /workspace/old/bin/texi/mod_trial.texi


