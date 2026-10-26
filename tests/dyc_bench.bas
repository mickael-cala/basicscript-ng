' dyc_bench.bas - mesure du cout du chemin d'appel dyc (chantier overhead)
'
' Trois mesures sur 100 000 appels chacune, chronometrees par GetTickCount
' (resolution ~16 ms, largement suffisante : chaque serie dure plus d'une
' seconde) obtenu... par dyc lui-meme :
'
'   A. 100 000 x len(s$)                : la boucle BASIC seule, reference ;
'   B. 100 000 x dyc(lstrlenA)          : le chemin complet (commande BASIC,
'                                         parseur du format, marshalling,
'                                         DynaCall, retour) ;
'   C. difference B-A, divisee par 100k : le cout AJOUTE par appel dyc,
'                                         independant de la boucle.
'
' Le pendant C direct (appel indirect par pointeur de fonction, mesure
' QueryPerformanceCounter) est bin/bench_dyc.exe, construit par la cible
' bench_dyc (rsp/bench_dyc.rsp) ; PORTAGE_DYC64.md documente les deux.

import dyc.bas

t0 = dyc::dyc("ms,i,KERNEL32.DLL,GetTickCount,")
s$ = "ScriptBasic"
for i = 1 to 100000
  n = len(s$)
next i
t1 = dyc::dyc("ms,i,KERNEL32.DLL,GetTickCount,")
for i = 1 to 100000
  n = dyc::dyc("ms,i,KERNEL32.DLL,lstrlenA,Z",s$)
next i
t2 = dyc::dyc("ms,i,KERNEL32.DLL,GetTickCount,")

refms = t1 - t0
dycms = t2 - t1
surcout = dycms - refms
print "A boucle BASIC seule (100k len)   = ", refms, " ms\n"
print "B 100k appels dyc(lstrlenA)       = ", dycms, " ms\n"
print "C surcout dyc = B-A               = ", surcout, " ms soit ", surcout/100000.0, " ms par appel\n"
print "sanite : lstrlenA rend ", n, " (attendu 11)\n"
if n <> 11 then
  print "dyc_bench : ECHEC\n"
  error 1
  end if
