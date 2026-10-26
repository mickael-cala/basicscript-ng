' commandes.bas - les commandes disent la verite (ERRATA E26 a E31).
'
' Ce programme verifie les corrections de la dix-septieme passe :
'   - nextfile/eod/reset directory/close directory sur un numero jamais
'     ouvert rendent l'erreur 33 (DIR_NO_OPEN) au lieu de dereferencer NULL ;
'   - print #n sur un fichier non ouvert rend 23 (FILE_IS_NOT_OPENED), et 20
'     (BAD_FILE_NUMBER) hors de la plage ;
'   - formatdate refuse un motif « % » etranger au langage (48) ;
'   - isnumeric analyse vraiment la chaine ;
'   - on error resume next laisse le code dans error ;
'   - isdefined(f(...)) ne tue plus le processus (regression) ;
'   - time, ucase$, lcase$, upper$ et lower$ rendent le meme resultat que
'     leur nom canonique (ERRATA E35) ;
'   - ucase/lcase ne modifient plus leur argument, donc plus les litteraux
'     partages (ERRATA E36).
'
' Le programme se termine par `error 1` des qu'un ecart est vu, ce que la
' campagne signale comme un echec.

global echecs
global essais
echecs = 0
essais = 0

sub check(nom, obtenu, attendu)
  essais = essais + 1
  if str(obtenu) = str(attendu) then
    print "  OK    ", nom, "   -> ", str(obtenu), "\n"
  else
    print "  ECHEC ", nom, "   obtenu=", str(obtenu), " attendu=", str(attendu), "\n"
    echecs = echecs + 1
  endif
end sub

print "=== 1. repertoires non ouverts (ERRATA E26) ===\n"

' Avant la correction, chacun de ces appels faisait tomber le processus en
' violation d'acces (0xC0000005) : le pointeur de la table des repertoires
' etait dereference sans test.
on error resume next
v = nextfile(1)
check "nextfile sans ouverture",    error,  33
on error resume next
v = eod(1)
check "eod sans ouverture",         error,  33
on error resume next
reset directory 1
check "reset directory sans ouverture", error, 33
on error resume next
close directory 1
check "close directory sans ouverture", error, 33

print "\n=== 2. print #n (ERRATA E27) ===\n"

' Un numero libre ne doit plus avaler la sortie en silence.
on error resume next
print #9,"x"
check "print #9 libre",             error,  23
on error resume next
print #0,"x"
check "print #0 hors plage",        error,  20

print "\n=== 3. formatdate (ERRATA E30) ===\n"

' Le langage de format s'ecrit YEAR, MON, DD... ; « %Y » est un motif
' etranger et doit etre refuse, pas recopie tel quel.
on error resume next
d$ = formatdate("%Y-%m-%d")
check "formatdate motif strftime",  error,  48
' Un « % » final reste un caractere litteral.
g$ = formatdate("100%")
check "formatdate pour cent garde",  g$,   "100%"
' Les directives du langage marchent toujours.
a$ = formatdate("YEAR")
check "formatdate YEAR rend 4 chiffres", (len(a$) = 4), -1

print "\n=== 4. isnumeric (ERRATA E29) ===\n"

check "isnumeric 12",               isnumeric("12"),        -1
check "isnumeric -3.5",             isnumeric("-3.5"),      -1
check "isnumeric espaces",          isnumeric(" 7 "),       -1
check "isnumeric +2",               isnumeric("+2"),        -1
check "isnumeric exposant",         isnumeric("1e3"),       -1
check "isnumeric point seul",       isnumeric("."),          0
check "isnumeric 12abc",            isnumeric("12abc"),      0
check "isnumeric vide",             isnumeric(""),           0
check "isnumeric virgule",          isnumeric("1,5"),        0
check "isnumeric nombre",           isnumeric(12),          -1

print "\n=== 5. on error resume next (ERRATA E31) ===\n"

' La reprise ne rend pas l'erreur inexistante : error la nomme encore.
on error resume next
open "::impossible::" for input as 2
check "resume next garde error",    error,  22

print "\n=== 6. isdefined(f(...)) (regression) ===\n"

' L'audit reprochait a ISDEF de faire tomber le processus des que son argument
' est un appel de fonction. Mesure : ce n'est plus le cas, et pas seulement
' pour les fonctions internes. Le bloc couvre les fonctions utilisateur, le
' resultat undef, un sous-programme, l'evaluation unique et une boucle qui
' exerce la duree de vie du resultat.
on error resume next
check "isdefined(crypt)",           isdefined(crypt("a","b")), -1
check "isdefined(len)",             isdefined(len("ab")),      -1

appels = 0
function fval
  appels = appels + 1
  fval = 7
end function
function fvide
end function
function fmer
  local a
  a[1] = 3
  fmer = a[1]
end function
function fidem(x)
  fidem = x
end function
function sidem(x)
  sidem = x
end function
function frec(n)
  if n > 0 then
    frec = frec(n-1)
  else
    frec = 1
  endif
end function
sub svide
end sub

check "isdefined(f) valeur",        isdefined(fval()),        -1
check "isdefined(f) un seul appel",  appels,                   1
check "isdefined(f) undef",          isdefined(fvide()),        0
check "isdefined(f) element",        isdefined(fmer()),        -1
check "isdefined(f) chaine",         isdefined(sidem(crypt("a","b"))), -1
check "isdefined(f) arg undef",      isdefined(fidem(zz)),      0
check "isdefined(f) recursif",       isdefined(frec(3)),       -1
check "isdefined(sub)",              isdefined(svide()),        0
check "isdefined dans isdefined",    isdefined(isdefined(fval())), -1
check "isundef(f) vide",             isundef(fvide()),         -1
check "isundef(f) valeur",           isundef(fval()),           0

' 200 appels : un resultat dont la duree de vie serait liee a l'appel se
' ferait liberer ici et le processus tomberait.
n = 0
for i = 1 to 200
  if isdefined(fval()) then n = n + 1
next
check "isdefined(f) 200 fois",       n,                       200

print "\n=== 7. listes de repertoires (temoin) ===\n"

' Les gardes ne doivent pas casser le chemin normal : ouvrir, compter,
' fermer, puis verifier que le numero est bien redevenu libre.
n = 0
open directory "tests" pattern "*.bas" option 0 as 1
while eod(1) = 0
  f$ = nextfile(1)
  n = n + 1
wend
close directory 1
check "listing tests/*.bas",        (n > 5),              -1
on error resume next
v = nextfile(1)
check "nextfile apres fermeture",   error,  33

' « as 0 » doit allouer le premier numero libre.
dn = 0
open directory "tests" pattern "*.bas" option 0 as dn
check "open directory as 0",        dn,     1
close directory 1

print "\n=== 8. les alias perdus au portage (ERRATA E35) ===\n"

' Le portage avait perdu des noms que l'amont acceptait : time, ucase$ et
' lcase$ ; upper$ et lower$ etaient deja la et servent de temoin.
' Chacun doit rendre exactement ce que rend son nom canonique.
check "time() suit now()",          abs(time()-now()) <= 1, -1
check "ucase(\"ab\")",              ucase("ab"),  "AB"
check "ucase$(\"ab\")",             ucase$("ab"), "AB"
check "upper$(\"ab\")",             upper$("ab"), "AB"
check "lcase(\"AB\")",              lcase("AB"),  "ab"
check "lcase$(\"AB\")",             lcase$("AB"), "ab"
check "lower$(\"AB\")",             lower$("AB"), "ab"

u = ucase("ab")
check "ucase laisse le littéral",  "ab", chr(97) & chr(98)
l = lcase("XY")
check "lcase laisse le littéral",  "XY", chr(88) & chr(89)

print "\n"
print "essais : ", essais, "   echecs : ", echecs, "\n"
if echecs > 0 then
  print "COMMANDES : ", echecs, " verification(s) en echec\n"
  error 1
endif
print "COMMANDES : toutes les verifications passent\n"
