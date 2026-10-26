' dyc.bas - test fonctionnel de l'extension dyc
'
' dyc est le module qui construit une pile d'appel a la main pour invoquer une
' fonction quelconque d'une DLL quelconque. C'est le seul module dont le code
' d'origine utilisait l'asm inline de Microsoft (_asm mov pStack, esp ...), que
' TinyCC refuse : son DynaCall a donc ete reecrit en asm inline GNU. Ce test
' verifie la traduction sur les quatre chemins de retour de valeur.
'
' Format : dyc::dyc("convention,retour,dll,fonction,types_arguments", args...)
'   m / s  Microsoft, stdcall        c  convention C (cdecl)
'   4 / 8  retour flottant dans ST (simple / double)
'   i l p f d v   type du retour : entier, long, pointeur, float, double, int64
' Types d'arguments :
'   L  entier 32 bits      Z  chaine terminee par zero (passee par pointeur)
'   D  double (8 octets, passe par pointeur)     P  pointeur
'
' Remarque importante : le format doit comporter AU MOINS un caractere de type
' d'argument. La distribution de ScriptBasic 1.0 build 2020 a une lacune dans
' l'analyseur : la section des arguments n'est traitee que si la boucle de
' lecture a encore des caracteres a examiner, si bien qu'un format comme
' "ms,i,KERNEL32.DLL,GetTickCount," (zero argument) laisse hDll, szFunction,
' nArgs et Parm NON INITIALISES. L'appel echoue alors sur l'erreur 17
' (COMMAND_ERROR_MODULE_FUNCTION) alors que la fonction existe. Ce n'est pas
' une regression du portage : le meme format echoue avec un build MSVC. Le test
' n'utilise donc que des fonctions prenant au moins un argument.

import dyc.bas

ok = 1

' ---------------------------------------------------------------- stdcall ---
' lstrlenA("ScriptBasic") doit valoir exactement 11. Cela valide le passage
' d'un argument chaine par pointeur (le format 'Z' place l'adresse de la chaine
' dans la pile) et la restauration de ESP apres un appel stdcall.
n1 = dyc::dyc("ms,i,KERNEL32.DLL,lstrlenA,Z","ScriptBasic")
print "lstrlenA(\"ScriptBasic\")          = ", n1, "  (attendu 11)\n"
if n1 <> 11 then ok = 0

' Chaine vide : zero caractere, ce qui valide aussi que la valeur 0 traversee
' par la pile n'est pas confondue avec une absence d'argument.
n2 = dyc::dyc("ms,i,KERNEL32.DLL,lstrlenA,Z","")
print "lstrlenA(\"\")                     = ", n2, "  (attendu 0)\n"
if n2 <> 0 then ok = 0

' Un argument entier par valeur : Sleep(0) rend la main immediatement.
r1 = dyc::dyc("ms,i,KERNEL32.DLL,Sleep,L",0)
print "Sleep(0)                        = ", r1, "\n"

' Retour pointeur : le handle de kernel32.dll doit etre non nul.
' La base de chargement de kernel32 est toujours superieure a 0x10000000.
h1 = dyc::dyc("ms,p,KERNEL32.DLL,GetModuleHandleA,Z","kernel32.dll")
print "GetModuleHandleA(\"kernel32.dll\")  = ", h1, "\n"
if h1 <= 0 then ok = 0

' ------------------------------------------------------------------ cdecl ---
' strlen est en convention C : c'est l'appelant qui nettoie la pile. Dans le
' code d'origine, ce cas empruntait le chemin `_asm add esp, dwStSize` ; la
' traduction restaure ESP inconditionnellement, ce qui doit donner le meme
' resultat.
n3 = dyc::dyc("cs,i,msvcrt.dll,strlen,Z","abcdef")
print "strlen(\"abcdef\")                 = ", n3, "  (attendu 6)\n"
if n3 <> 6 then ok = 0

' ------------------------------------------------- retour flottant double ---
' cos(0.0) = 1.0. La convention '8' indique a DynaCall que le resultat revient
' dans le registre du coprocesseur et non dans eax/edx : c'est le chemin
' `_asm fstp qword ptr [Res]` du code d'origine, traduit en `fstpl`. Le type
' d'argument 'D' exerce en plus DC_FLAG_ARGPTR, qui copie deux mots depuis la
' valeur pointee.
d1 = dyc::dyc("c8,d,msvcrt.dll,cos,D",0.0)
print "cos(0.0)                        = ", d1, "  (attendu 1)\n"
if d1 < 0.999 or d1 > 1.001 then ok = 0

d2 = dyc::dyc("c8,d,msvcrt.dll,sin,D",0.0)
print "sin(0.0)                        = ", d2, "  (attendu 0)\n"
if d2 < -0.001 or d2 > 0.001 then ok = 0

' ------------------------------------------------------------------ bilan ---
if ok then
  print "\ndyc : OK - les quatre chemins de retour de valeur sont corrects\n"
else
  print "\ndyc : ECHEC\n"
end if
