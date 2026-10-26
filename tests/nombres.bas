' nombres.bas - les grands nombres ne mentent plus.
'
' Deux defauts mesures avant cette passe :
'   - un litteral au-dela de LONG_MAX saturait : 3000000000 valait
'     -2147483648 sous Windows, ou long fait 32 bits ;
'   - une multiplication dont le produit depassait LONG_MAX etait calculee
'     en entier et rendait un resultat enroule : 1000000*10000 valait
'     1410065408. La verification d'overflow d'apres coup etait supprimee par
'     le compilateur, parce que le debordement signe est un comportement
'     indefini.
'
' Les deux rendent maintenant un reel exact, comme le fait deja l'addition.
' Les produits qui tiennent dans un long restent entiers.
'
' Les comparaisons sont numeriques : sur Windows 3000000000 est un reel, sous
' Linux un entier, et le test doit valoir sur les deux. Le programme se
' termine par `error 1` des qu'un ecart est vu.

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

print "=== 1. litteraux au-dela de LONG_MAX ===\n"

' Le signe est un operateur unaire : c'est bien le litteral qui est en cause.
check "3000000000 positif",       (3000000000 > 0),         -1
check "3000000000 / 1e9",         round(3000000000/1000000000,6), 3
check "-3000000000 / 1e9",        round(-3000000000/1000000000,6), -3
check "4294967296 / 1e9",         round(4294967296/1000000000,6), 4.294967296
check "2147483647 reste entier",  isinteger(2147483647),    -1

print "\n=== 2. multiplication qui deborde ===\n"

' 10^10 tient dans un long de 64 bits mais pas dans un long de 32 bits :
' sous Windows le resultat est un reel, sous Linux un entier. La comparaison
' est donc numerique, sinon str() distinguerait les deux types.
check "1000000*10000",            (1000000*10000 = 1e10),          -1
check "1000000*10000 > 2^31",     (1000000*10000 > 2147483647),    -1
check "1000000.0*10000",          (1000000.0*10000 = 1e10),        -1
check "2147483647*2",             (2147483647*2 = 4294967294e0),   -1
check "999999*999999",            (999999*999999 = 999998000001e0), -1
check "(-1000000)*10000",         ((-1000000)*10000 = -1e10),      -1
check "-2147483647*2",            (-2147483647*2 = -4294967294e0), -1
check "46341*46341",              (46341*46341 = 2147488281e0),    -1
check "produit relu en reels",    round(1000000*10000/1000000,6),  10000

print "\n=== 3. ce qui tient dans un long y reste ===\n"

check "1000*1000",                1000*1000,                1000000
check "1000*1000 est entier",     isinteger(1000*1000),     -1
' 46340^2 = 2147395600 tient encore dans 2^31-1.
check "46340*46340",              46340*46340,              2147395600
check "46340*46340 est entier",   isinteger(46340*46340),   -1
check "(-12)*12",                (-12)*12,                 -144

print "\n", essais, " essais, ", echecs, " echec(s)\n"
if echecs then error 1
