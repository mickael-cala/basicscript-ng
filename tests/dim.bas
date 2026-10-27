' dim.bas - DIM declare un tableau de taille connue.
'
' Ce programme verifie l'instruction ajoutee a let.c :
'   - DIM a[n] cree un tableau d'indices 0 a n, tous undef, et lbound/ubound
'     le disent avant la premiere ecriture ;
'   - chaque crochet ajoute une dimension, allouee dans chaque element de la
'     precedente, comme l'acces a[i,j] l'attend ;
'   - la taille est une expression, evaluee une seule fois, au moment ou la
'     commande s'execute ;
'   - DIM remplace l'ancienne valeur et ne conserve rien ;
'   - les indices au-dela de la borne restent accessibles : DIM fixe
'     l'etendue initiale, pas une limite ;
'   - la portee est celle de la variable (locale, globale, argument par
'     reference) ;
'   - une taille undef, negative, ou non numerique est refusee a l'execution
'     (erreurs 7 et 8) et laisse l'ancienne valeur en place.
'
' Le programme se termine par `error 1` des qu'un ecart est vu, ce que la
' campagne signale comme un echec.

global echecs
global essais
global appels
global gv
echecs = 0
essais = 0
appels = 0

sub check(nom, obtenu, attendu)
  essais = essais + 1
  if str(obtenu) = str(attendu) then
    print "  OK    ", nom, "   -> ", str(obtenu), "\n"
  else
    print "  ECHEC ", nom, "   obtenu=", str(obtenu), " attendu=", str(attendu), "\n"
    echecs = echecs + 1
  endif
end sub

function taille
  appels = appels + 1
  taille = 7
end function

print "=== 1. taille declaree ===\n"

dim a[5]
check "isarray(a)",                 isarray(a),             -1
check "lbound(a)",                  lbound(a),              0
check "ubound(a)",                  ubound(a),              5
check "a[0] undef",                 isundef(a[0]),          -1
check "a[5] undef",                 isundef(a[5]),          -1
a[3] = 42
check "a[3] apres ecriture",        a[3],                   42
check "ubound inchange",            ubound(a),              5

dim zero[0]
check "dim zero[0] : ubound",       ubound(zero),           0
check "dim zero[0] : undef",        isundef(zero[0]),       -1

print "\n=== 2. plusieurs dimensions ===\n"

dim m[2,3]
check "ubound(m)",                  ubound(m),              2
check "ubound(m[0])",               ubound(m[0]),           3
check "ubound(m[2])",               ubound(m[2]),           3
check "m[2,3] undef",               isundef(m[2,3]),        -1
m[1,2] = "case"
check "m[1,2] apres ecriture",      m[1,2],                 "case"
check "ubound(m[1]) inchange",      ubound(m[1]),           3
check "m[1,0] undef",               isundef(m[1,0]),        -1

print "\n=== 3. taille calculee ===\n"

n = 4
dim b[n]
check "dim b[n] avec n=4",          ubound(b),              4

n = 9
check "la taille ne suit pas n",    ubound(b),              4

dim c[taille()]
check "taille par fonction",        ubound(c),              7
check "evaluee une seule fois",     appels,                 1

dim reel[2.9]
check "un reel est tronque",        ubound(reel),           2

dim texte["6"]
check "une chaine entiere passe",   ubound(texte),          6

print "\n=== 4. plusieurs tableaux sur une ligne ===\n"

dim p[1], q[3]
check "ubound(p)",                  ubound(p),              1
check "ubound(q)",                  ubound(q),              3

print "\n=== 5. DIM remplace, ne conserve pas ===\n"

dim garde[3]
garde[1] = "vieux"
dim garde[1]
check "nouvelle borne",             ubound(garde),          1
check "les valeurs sont perdues",   isundef(garde[1]),      -1

print "\n=== 6. DIM borne le tableau (phase 8.1) ===\n"

dim grand[2]
on error goto HorsBornes
grand[8] = 1
print "  ECHEC : l'ecriture hors bornes ne leve rien\n"
echecs = echecs + 1
goto ApresBornes
HorsBornes:
check "hors bornes leve 133",       error,                  133
check "la borne ne bouge pas",      ubound(grand),          2
' desarme : l'erreur pendante ne sera pas reporquee a la fin, et la
' suite du test n'a plus rien a y attraper.
on error goto null
ApresBornes:

print "\n=== 7. portee ===\n"

t[0] = "global"
sub locale
  local t
  dim t[4]
  gv = ubound(t)
end sub
locale
check "tableau local",              gv,                     4
check "le global est intact",       t[0],                   "global"

sub remplit(a)
  dim a[3]
end sub
x = 1
remplit x
check "argument par reference",     ubound(x),              3
check "et le tableau existe",       isarray(x),             -1

print "\n=== 8. refus a l'execution ===\n"

' on error resume next ne couvre qu'une erreur : il faut le re-armer.
on error resume next
dim neg[-1]
check "taille negative",            error,                  8
check "la variable reste undef",    isdefined(neg),         0

on error resume next
dim inconnue[absente]
check "taille undef",               error,                  7
check "la variable reste undef",    isdefined(inconnue),    0

on error resume next
dim mots["abc"]
check "taille non numerique",       error,                  7

on error resume next
dim enorme[1.0e300]
check "taille hors plage",          error,                  8

' Un refus ne doit pas detruire ce que la variable portait.
dim garde2[2]
garde2[0] = "conserve"
on error resume next
dim garde2[-5]
check "le refus leve l'erreur",     error,                  8
check "l'ancien tableau survit",    garde2[0],              "conserve"
check "et sa borne aussi",          ubound(garde2),         2

print "\n", essais, " essais, ", echecs, " echec(s)\n"
if echecs then error 1
