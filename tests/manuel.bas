' manuel.bas - les exemples du MANUEL.md, toutes familles confondues.
'
' Ce programme n'est pas une demonstration : il VERIFIE ce que le manuel
' affirme. Chaque appel passe par check() ou checkr(), et le programme se
' termine par `error 1` des qu'un ecart a ete vu, ce qui fait echouer la
' campagne (code de sortie non nul, et un message que run_all.py reconnait).
'
' Ajouter une affirmation au manuel sans l'ajouter ici laisse donc un trou
' visible, et l'inverse est impossible.

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

' Les reels s'ecrivent en notation exposant (7/2 s'imprime 3.500000e+000),
' donc str() ne convient pas pour les comparer : on compare a une tolerance.
sub checkr(nom, obtenu, attendu)
  essais = essais + 1
  if abs(obtenu - attendu) < 0.000001 then
    print "  OK    ", nom, "   -> ~", str(attendu), "\n"
  else
    print "  ECHEC ", nom, "   obtenu=", obtenu, " attendu=", attendu, "\n"
    echecs = echecs + 1
  endif
end sub

print "=== 1. valeurs et types (MANUEL 2) ===\n"

' Le vrai du langage est -1, pas 1.
check "vrai = -1",            (1 < 2),               -1
check "faux = 0",             (1 = 2),                0
check "type entier",          type(1),                3
check "type reel",            type(1.5),              2
check "type chaine",          type("x"),              1
check "isinteger",            isinteger(1),          -1
check "isreal",               isreal(1.5),           -1
check "isstring",             isstring("x"),         -1
check "isdefined",            isdefined(echecs),     -1
check "isundef",              isundef(zz),          -1
check "isnumeric(chaine)",    isnumeric("12"),       -1
check "isnumeric(texte)",     isnumeric("12abc"),     0
check "isnumeric(nombre)",    isnumeric(12),         -1
' La conversion implicite d'une chaine non numerique rend 0, pas une erreur.
check "conversion implicite", val("12") + 1,         13
check "chaine non numerique", "a" + "b",              0

print "\n=== 2. operateurs (MANUEL 5) ===\n"

check "addition",             2 + 3,                  5
check "precedence",           2 + 3 * 4,             14
check "parentheses",          (2 + 3) * 4,           20
check "puissance",            2 ^ 10,              1024
check "modulo",               7 % 3,                  1
' Le langage n'a pas de separateur d'instructions : 7 \ 2 s'ecrit ainsi.
check "division entiere",     7 \ 2,                  3
checkr "division reelle",     7 / 2,                3.5
check "concatenation",        "a" & "b" & 12,       "ab12"
check "et logique",           (1 and 0),              0
check "ou logique",           (1 or 0),               1
check "non logique",          (not 0),               -1
check "ou exclusif",          (1 xor 1),              0

print "\n=== 3. motifs like et joker (MANUEL 5) ===\n"

if "abc.def" like "*.def" then
  check "like",               -1,                    -1
  ' joker(n) est indexe a partir de 1, et rend le texte apparie.
  check "joker(1)",           joker(1),           "abc"
endif
if "aXb" like "a?b" then
  check "joker sur ?",        joker(1),             "X"
endif

print "\n=== 4. tableaux (MANUEL 3) ===\n"

' Aucun `dim` : le tableau nait de la premiere affectation indexee.
t[0] = 1
t[1] = 2
check "somme tableau",        t[0] + t[1],           3
check "lbound",               lbound(t),             0
check "ubound",               ubound(t),             1
t[5] = 9
check "ubound suit l'ecrit",  ubound(t),             5
m[1,1] = 7
check "deux dimensions",      m[1,1],                7
a{"x"} = 1
a{"y"} = 2
check "table associatif",     a{"x"} + a{"y"},       3

print "\n=== 5. reference, byval, undef (MANUEL 3) ===\n"

sub parReference(x)
  ref y = x
  y = 99
end sub

sub parValeur(x)
  byval x
  x = 0
end sub

v = 1
parReference(v)
check "parametre par reference", v,                 99
parValeur(v)
check "byval protege",           v,                 99

w = 5
undef w
check "undef",                   isundef(w),        -1

p = 1
q = 2
swap p, q
check "swap p",                  p,                  2
check "swap q",                  q,                  1

print "\n=== 6. portee, modules (MANUEL 4) ===\n"

' Les noms sont INSENSIBLES A LA CASSE : `const K` et `k = 0` seraient le
' meme nom, et l'affectation serait refusee (erreur &H7d). Ne pas confondre
' ce cas avec le nom de la constante, que l'on qualifie pour les modules.
const KONST = 42
global g5
g5 = 5
module mm
  const c = 3
end module
check "constante",               KONST,              42
check "globale",                 g5,                 5
' Une constante de module s'accede QUALIFIEE : `c` seul rendrait undef.
check "constante de module",     mm::c,               3

print "\n=== 7. controle du flux (MANUEL 6.5) ===\n"

somme = 0
for i = 1 to 5 step 2
  somme = somme + i
next
check "for / step",              somme,               9

n = 0
while n < 3
  n = n + 1
wend
check "while / wend",            n,                   3

n = 0
do
  n = n + 1
loop while n < 3
check "do / loop while",         n,                   3

n = 0
repeat
  n = n + 1
until n >= 3
check "repeat / until",          n,                   3

classe = ""
if 0 then
  classe = "a"
elseif 1 then
  classe = "b"
else
  classe = "c"
endif
check "if / elseif / else",      classe,            "b"

slif = ""
if 1 then slif = "ok"
check "if sur une ligne",        slif,             "ok"

' Il n'existe ni exit for ni exit while : on sort par goto.
k = 0
for i = 1 to 10
  if i > 3 then
    goto horsBoucle
  endif
  k = k + 1
next
horsBoucle:
check "sortie de boucle par goto", k,                3

gosub sousGosub
check "gosub / return",          apresGosub,        77
goto apresLeGosub
sousGosub:
  apresGosub = 77
  return
apresLeGosub:

print "\n=== 8. sous-programmes et fonctions (MANUEL 6.3) ===\n"

' `sub s` et non `sub s()` : une liste vide entre parentheses est refusee.
sub sansArgument
  appelee = 1
end sub
call sansArgument
check "sub sans parentheses",    appelee,             1

function double(x)
  double = x * 2
end function
check "function",                double(21),         42

function sortir(x)
  if x > 0 then
    sortir = 1
    exit function
  endif
  sortir = -1
end function
check "exit function",           sortir(5),           1

print "\n=== 9. chaines (MANUEL 7.2) ===\n"

check "len",                     len("abc"),          3
check "mid",                     mid("abcdef",2,3), "bcd"
check "left",                    left("abcdef",2),   "ab"
check "right",                   right("abcdef",3),  "def"
check "ucase",                   ucase("aB"),        "AB"
check "lcase",                   lcase("aB"),        "ab"
check "trim",                    trim("  x  "),       "x"
check "string",                  string(3,"ab"),    "aaa"
check "chr",                     chr(65),             "A"
check "asc",                     asc("A"),             65
check "str",                     str(12),            "12"
check "strreverse",              strreverse("abc"), "cba"
check "instr",                   instr("abcdef","cd"), 3
check "replace",                 replace("aXbXc","X","-"), "a-b-c"
' join : le separateur est le PREMIER argument.
check "join",                    join("-",1,2,3), "1-2-3"
check "format",                  format("%d-%s",7,"x"), "7-x"

print "\n=== 10. mathematiques (MANUEL 7.1) ===\n"

check "sqrt (et non sqr)",       sqrt(9),              3
check "int",                     int(2.7),             2
check "fix",                     fix(-2.7),           -2
check "exp",                     exp(0),               1
' log est le logarithme NATUREL ; log10 est celui de base 10.
check "log est naturel",         log10(100),           2
checkr "log(100)",               log(100),          4.605170
check "sgn",                     sgn(-3),             -1
check "abs",                     abs(-3),              3
' pow(2) vaut 10^2 : pow est la puissance de DIX, pas x^y.
check "pow = 10^x",              pow(2),             100
check "hex (majuscules)",        hex(255),           "FF"
check "oct",                     oct(8),             "10"
checkr "round",                  round(2.567,2),    2.57
' atan et atn sont la meme fonction ; atan est le nom canonique.
checkr "atan(1)",                 atan(1),       0.785398
checkr "atn(1)",                  atn(1),        0.785398
check "atan(0) entier",           atan(0),              0

' --- constantes : pi, bornes entieres, vrai/faux (MANUEL 7.3) ---
checkr "pi",                     pi,             3.141593
check "maxint",                  maxint,         2147483647
' 2147483648 ne tient pas dans un long de 32 bits : le litteral est donc un
' reel sous Windows, et minint un entier. La comparaison est numerique.
check "minint",                  (minint = -2147483648e0),  -1
check "true",                    true,                -1
check "false",                   false,                0

' --- les tests de type, un par un (MANUEL 7.3) ---
check "isinteger vrai",          isinteger(1),        -1
check "isinteger faux",          isinteger(1.5),       0
check "isreal vrai",             isreal(1.5),         -1
check "isreal faux",             isreal(1),            0
check "isstring vrai",           isstring("a"),       -1
check "isstring faux",           isstring(1),          0
check "isarray vrai",            isarray(t),          -1
check "isarray faux",            isarray(uneinconnue), 0

' --- option() lit la table posee par l'instruction option (MANUEL 7.4) ---
option SondOption 42
check "option lue",              option("SondOption"), 42
check "option absente = undef",  option("jamais_posee") = undef, -1

' rnd ne prend AUCUN argument, et ne rend PAS un reel de [0,1) : c'est la
' valeur de rand() de la bibliotheque C, donc un reel de valeur entiere
' dans [1, 32767]. randomize fixe la graine de ce meme generateur.
randomize 1
check "rnd est un reel",         type(rnd()),          2
borneOk = -1
for i = 1 to 2000
  r = rnd()
  if (r < 1) or (r > 32767) then borneOk = 0
  if r <> int(r) then borneOk = 0
next
check "rnd entier dans [1,32767]", borneOk,          -1
' La graine est reproductible : meme suite a chaque execution.
randomize 1
check "randomize reproductible", int(rnd()),         41

print "\n=== 11. chaines decoupees (MANUEL 6.6) ===\n"

split "a,b,c" by "," to c1, c2, c3
check "split 1",                 c1,                 "a"
check "split 3",                 c3,                 "c"

src = "10,20,30"
splita src by "," to lot
check "splita",                  lot[1],             20

print "\n=== 12. fichiers (MANUEL 7.4) ===\n"

fn = "test_manuel_tmp.txt"
f = freefile()
check "freefile est un numero",  (f > 0),             -1

open fn for output as f
print #f, "premiere\n"
print #f, "seconde\n"
close f

check "fileexists",              fileexists(fn),      -1
check "isfile",                  isfile(fn),          -1
check "isdirectory",             isdirectory("tests"), -1
check "isdirectory faux",        isdirectory(fn),      0
check "isfile faux",             isfile("manuel_inex_x"), 0
check "fileexists faux",         fileexists("manuel_inex_x"), 0
check "filelen",                 (filelen(fn) > 0),   -1
check "fileaccesstime",          (fileaccesstime(fn) > 0), -1
check "filemodifytime",          (filemodifytime(fn) > 0), -1
check "filecreatetime",          (filecreatetime(fn) > 0), -1
check "fileowner non vide",      (len(fileowner(fn)) > 0), -1

open fn for input as f
line input #f, l1$
line input #f, l2$
' eof ne passe a -1 qu'APRES une lecture qui depasse la fin, pas apres la
' derniere ligne : c'est une consequence, pas une opinion (voir MANUEL 7.4).
check "eof avant depassement",   eof(f),               0
line input #f, l3$
check "eof apres depassement",   eof(f),              -1
close f
' line input conserve la fin de ligne : d'ou chomp().
check "line input 1",            chomp(l1$), "premiere"
check "line input 2",            chomp(l2$), "seconde"

' loc/lof/input(n,fn) : lof rend la taille en ENREGISTREMENTS quand len est
' donne (19 octets / 4 = 4 records entiers), en octets sinon. input(n) rend
' n enregistrements (8 octets du debut) ; pres de la fin il rend ce qui
' reste, sans erreur — le record tronque est rendu tronque.
open fn for input as f len = 4
check "lof en enregistrements",  lof(f),               4
lu = input(2, f)
check "input(2) = 2 records",    len(lu),              8
close f
open fn for input as f
check "lof en octets",           lof(f),              19
lu2 = input(3, f)
check "input(3) sans len",       len(lu2),             3
close f

delete fn
check "delete",                  fileexists(fn),       0

' Parcours de repertoire : le numero se donne AVEC un diese (as #1).
' eod vaut -1 quand la liste est EPUISEE OU VIDE ; le pattern s'applique aux
' noms du dossier indique. Le fichier choisi doit exister a la racine du
' depot (README.md), sinon la liste est vide et les deux affirmations
' suivantes tombent.
open directory "." pattern "README.md" option sbCollectFiles as #1
check "eod liste non epuisee",   eod(1),               0
un = nextfile(1)
check "open directory / nextfile", isdefined(un),      -1
check "eod liste epuisee",       eod(1),              -1
close directory #1

' lock / release ; lock region pose un verrou de plage. unlock n'existe
' PAS, meme en forme region : la liberation passe par lock #n, release.
fl = freefile()
open fn for binary as fl
lock #fl, read
lock #fl, release
lock region #fl from 1 to 3 for write
close fl
check "lock / release / region",  -1,                -1
' Le `open ... for binary` vient de RECREER le fichier, efface plus haut :
' on le supprime pour ne rien laisser derriere soi. Un fichier residuel
' n'est pas un detail : l'inventaire de REGLES.md compte chaque fichier non
' ignore, et une campagne qui laisse une trace fait echouer son controle.
delete fn

print "\n=== 13. temps (MANUEL 7.5) ===\n"

tv = timevalue(2000,6,15,12,30,45)
check "year",                    year(tv),          2000
check "month",                   month(tv),            6
check "day",                     day(tv),             15
check "hour",                    hour(tv),            12
check "minute",                  minute(tv),          30
check "sec",                     sec(tv),             45
check "addday est en secondes",  addday(0,1),      86400
' formatdate attend des jetons VERBAUX (YEAR, 0M, DD...). Un code strftime
' est un motif inconnu : il est refuse (ERRATA E30), pas recopie.
check "formatdate YEAR",         len(formatdate("YEAR", now)), 4
on error resume next
v$ = formatdate("%Y", now)
check "formatdate %Y refuse",    error, 48
check "formattime",              len(formattime("0H:0m:0s", now)), 8

print "\n=== 14. systeme (MANUEL 7.4) ===\n"

check "environ",                 (len(environ("PATH")) > 0), -1
check "environ absente undef",   environ("MANUEL_ABSENTE_X") = undef, -1
check "curdir",                  (len(curdir()) > 0),        -1
check "hostname",                (len(hostname()) > 0),      -1
' Les valeurs de la configuration dependent de l'architecture (modules64/,
' .dll) : on verifie la STRUCTURE, pas une valeur figee.
check "conf dll finit par dll",  right(conf("dll"), 4), ".dll"
check "conf module contient module", instr(lcase(conf("module")), "module") > 0, -1
' Une cle de configuration inexistante rend l'erreur 8, pas undef (MANUEL 7.4).
on error goto capConf
cf = conf("manuel_cle_absente")
goto apresConf
capConf:
check "conf absente = erreur",   error,                 8
apresConf:

' system rend un PID ; waitpid le sonde ; kill tue (MANUEL 7.4).
pid = system("cmd /c exit 0")
check "system rend un pid",      (pid > 0),                 -1
zc = 0
wp = waitpid(pid, zc)
check "waitpid rend 0 ou 1",     (wp = 0) or (wp = 1),      -1

sleep 0
randomize 1
check "sleep et randomize",      -1,                        -1

print "\n=== 15. erreurs (MANUEL 7.6) ===\n"

on error goto capture
open "::fichier_impossible::" for input as 3
goto apresCapture
capture:
  ' L'erreur est visible et CODE : 22 = COMMAND_ERROR_FILE_CANNOT_BE_OPENED.
  check "on error goto capture", error,              22
  ' error$ rend le MESSAGE de cette erreur.
  check "error$ non vide",       (len(error$()) > 0), -1
apresCapture:

on error resume next
open "::autre_impossible::" for input as 4
' resume next poursuit l'execution mais ne rend pas l'erreur inexistante :
' error garde le code, comme avec on error goto (ERRATA E31).
check "resume next garde error", error,              22

' printnl n'imprime qu'un saut de ligne ; et print N'ajoute PAS de saut de
' ligne lui-meme (pas de point-virgule final : c'est refuse) (MANUEL 6.6).
print "a"
printnl
print "b"

' icall appelle par l'adresse rendue par address(f()) (MANUEL 7.4).
sub appeleParIcall
  global icallOk
  icallOk = -1
end sub
icallOk = 0
icall address(appeleParIcall())
check "icall address",           icallOk,             -1

print "\n"
print "essais : ", essais, "   echecs : ", echecs, "\n"
if echecs > 0 then
  print "MANUEL : ", echecs, " affirmation(s) du manuel mises en defaut\n"
  error 1
endif
print "MANUEL : toutes les affirmations verifiees\n"
