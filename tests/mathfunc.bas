' mathfunc.bas - max, min, bin, gcd et lcm sortent enfin de la liste des
' fonctions annoncees et jamais construites.
'
' Ce programme verifie les cinq implantations de mathfunc.c / string.c :
'   - GCD et LCM acceptent deux arguments ou plus, ignorent le signe, et
'     LCM(0,n) vaut 0 ;
'   - MAX et MIN rendent la valeur gagnante avec son type (entier ou reel),
'     la premiere en cas d'egalite ;
'   - IMAX et IMIN rendent la position du gagnant, comptee a partir de 0, la
'     premiere en cas d'egalite ;
'   - BIN ecrit les chiffres binaires sans prefixe, et BIN(0) vaut "0" ;
'   - un argument undef rend undef, sans lever d'erreur par defaut.
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

print "=== 1. gcd ===\n"

check "gcd(12,18)",              gcd(12,18),              6
check "gcd(12,18,24)",           gcd(12,18,24),           6
check "gcd(2,4,8,16)",           gcd(2,4,8,16),           2
check "gcd(17,5)",               gcd(17,5),               1
check "gcd(7,7)",                gcd(7,7),                7
check "gcd(0,0)",                gcd(0,0),                0
check "gcd(0,9)",                gcd(0,9),                9
check "gcd(-12,18) ignore le signe", gcd(-12,18),         6
check "gcd(-12,-18)",            gcd(-12,-18),            6
' execute_GetLongValue tronque un reel vers zero, comme HEX et OCT.
check "gcd(12,18.9) tronque",    gcd(12,18.9),            6
check "gcd rend un entier",      isinteger(gcd(12,18)),   -1

print "\n=== 2. lcm ===\n"

check "lcm(4,6)",                lcm(4,6),                12
check "lcm(4,6,8)",              lcm(4,6,8),              24
check "lcm(21,6)",               lcm(21,6),               42
check "lcm(2,3,5,7,11)",         lcm(2,3,5,7,11),         2310
check "lcm(0,5) est absorbant",  lcm(0,5),                0
check "lcm(5,0)",                lcm(5,0),                0
check "lcm(-4,6) ignore le signe", lcm(-4,6),             12
' Au-dela de LONG_MAX le resultat est rendu en reel : sous Windows, ou long
' fait 32 bits, 999999000000 ne tient pas dans un long et lcm le rend en reel ;
' sous Linux il reste entier. Le type depend donc de la plateforme, la valeur
' non : la comparaison ci-dessous est numerique et vaut sur les deux.
check "lcm(1000000,999999)",     (lcm(1000000,999999) = 9.99999e11), -1
check "lcm divise par 999999",   int(lcm(1000000,999999)/999999), 1000000
check "lcm(4,6) rend un entier", isinteger(lcm(4,6)),    -1

print "\n=== 3. max ===\n"

check "max(1,2,3)",              max(1,2,3),              3
check "max(-1,-5)",              max(-1,-5),              -1
check "max(1,2,3,4,5,6,7,8,9,10)", max(1,2,3,4,5,6,7,8,9,10), 10
check "max(2.5,2)",              max(2.5,2),              2.5
check "max(-2.5,-3)",            max(-2.5,-3),            -2.5
check "max(\"12\",7) compare en nombre", max("12",7),     12
check "max egalite garde le premier", max(7,7),           7
' Le type suit l'argument gagnant.
check "max(1,2) est entier",     isinteger(max(1,2)),     -1
check "max(1,2.5) est reel",     isreal(max(1,2.5)),      -1
check "max(2.5,3) est entier",   isinteger(max(2.5,3)),   -1

print "\n=== 4. min ===\n"

check "min(1,2,3)",              min(1,2,3),              1
check "min(-1,-5)",              min(-1,-5),              -5
check "min(1.5,2)",              min(1.5,2),              1.5
check "min(\"7\",12) compare en nombre", min("7",12),     7
check "min(3,3) garde le premier", min(3,3),              3
check "min(1,2) est entier",     isinteger(min(1,2)),     -1
check "min(1.5,2) est reel",     isreal(min(1.5,2)),      -1
check "min(1,2.5) est entier",   isinteger(min(1,2.5)),   -1

print "\n=== 5. imax et imin ===\n"

' L'indice est celui de l'argument, compte a partir de 0 ; il ne depend pas
' du type du gagnant ni de celui des autres arguments.
check "imax(5,3,9)",             imax(5,3,9),             2
check "imax(1,2)",               imax(1,2),               1
check "imax(7)",                 imax(7),                 0
check "imax de dix arguments",   imax(1,2,3,4,5,6,7,8,9,10), 9
check "imax egalite garde le premier", imax(3,3),         0
check "imax(3,3,3)",             imax(3,3,3),             0
check "imax compare en nombre",  imax("7",12),            1
check "imax(1.5,2,0.5)",         imax(1.5,2,0.5),         1
check "imax rend un entier",     isinteger(imax(1,2)),    -1
check "imin(5,3,9)",             imin(5,3,9),             1
check "imin(1,2)",               imin(1,2),               0
check "imin(7)",                 imin(7),                 0
check "imin(-1,-5)",             imin(-1,-5),             1
check "imin egalite garde le premier", imin(3,3),         0
check "imin compare en nombre",  imin("7",12),            0
check "imin(1.5,0.5,2)",         imin(1.5,0.5,2),         1
check "imin rend un entier",     isinteger(imin(1,2)),    -1
' La position rendue designe bien le plus grand ou le plus petit des
' arguments : lue comme indice d'un tableau, elle rend le bon element.
a[0] = 5
a[1] = 3
a[2] = 9
check "imax pointe le maximum",  a[imax(5,3,9)],          9
check "imin pointe le minimum",  a[imin(5,3,9)],          3

print "\n=== 6. bin ===\n"

check "bin(0)",                  bin(0),                  "0"
check "bin(1)",                  bin(1),                  "1"
check "bin(2)",                  bin(2),                  "10"
check "bin(5)",                  bin(5),                  "101"
check "bin(255)",                bin(255),                "11111111"
check "bin(256)",                bin(256),                "100000000"
check "bin(1024)",               bin(1024),               "10000000000"
check "bin(\"6\") lit la chaine", bin("6"),               "110"
check "bin(255) a huit chiffres", len(bin(255)),          8
' Les freres de bin n'ont pas bouge.
check "hex(255)",                hex(255),                "FF"
check "oct(8)",                  oct(8),                  "10"

print "\n=== 7. argument undef ===\n"

' Par defaut RaiseMatherror ne leve rien : l'argument undef se propage.
u = undef
on error resume next
check "max(1,undef) est undef",  isundef(max(1,u)),      -1
check "min(1,undef) est undef",  isundef(min(1,u)),      -1
check "gcd(12,undef) est undef", isundef(gcd(12,u)),     -1
check "lcm(12,undef) est undef", isundef(lcm(12,u)),     -1
check "imax(1,undef) est undef", isundef(imax(1,u)),     -1
check "imin(1,undef) est undef", isundef(imin(1,u)),     -1
check "aucune erreur levee",     error,                   0

print "\n", essais, " essais, ", echecs, " echec(s)\n"
if echecs then error 1
