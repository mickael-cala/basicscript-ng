' trig.bas - la trigonometrie complete, en radians.
'
' Ce programme verifie les quinze fonctions qui etaient annoncees et jamais
' construites, les six reciproques hyperboliques ajoutees, et les deux
' conversions :
'   - tangente, cotangente, secante, cosecante et leurs variantes a deux
'     arguments (tangente et cotangente du RAPPORT, pas atan2) ;
'   - hyperboliques directs et inverses ;
'   - reciproques : actan (arc cotangente), asecant, acosecant ;
'   - reciproques hyperboliques : ahsin, ahcos, ahtan, ahsecant, ahcosecant,
'     ahctan (aires hyperboliques) ;
'   - rad2deg et deg2rad, et PI() a la precision du double.
'
' Les egalites exactes sont comparees telles quelles ; les autres passent par
' round(x,n) des deux cotes, pour ne pas dependre du dernier bit de la libm.
' Les pôles (sinus, cosinus ou tangente hyperbolique exactement nuls) doivent
' rendre undef.
'
' Le programme se termine par `error 1` des qu'un ecart est vu.

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

print "=== 1. pi et conversions ===\n"

' PI() valait 3.1415926, huit chiffres : rad2deg(PI()) ne tombait pas sur 180
' et sin(PI()) valait 6.5e-9. La constante est maintenant celle du double.
check "rad2deg(pi())",            rad2deg(pi()),           180
check "rad2deg(pi()/2)",          rad2deg(pi()/2),         90
check "rad2deg(0)",               rad2deg(0),              0
check "deg2rad(0)",               deg2rad(0),              0
check "round(deg2rad(180),9)",    round(deg2rad(180),9),   round(pi(),9)
check "round(deg2rad(90),9)",     round(deg2rad(90),9),    round(pi()/2,9)
check "aller-retour 37",          round(deg2rad(rad2deg(37)),9), 37
check "sin(pi()) reste nul",      round(sin(pi()),12),     0

print "\n=== 2. tangente et cotangente ===\n"

check "tan(0)",                   tan(0),                  0
check "round(tan(pi()/4),9)",     round(tan(pi()/4),9),    1
check "round(tan(1),9)",          round(tan(1),9),         round(sin(1)/cos(1),9)
check "cotan(pi()/4) vaut 1",     round(cotan(pi()/4),9),  1
check "cotan = 1/tan",            round(cotan(1)*tan(1),9), 1
check "cotan(0) est undef",       isundef(cotan(0)),       -1
check "tan2(1,1) = tan(1)",       round(tan2(1,1),9),      round(tan(1),9)
check "tan2(1,2) = tan(0.5)",     round(tan2(1,2),9),      round(tan(0.5),9)
check "tan2(1,0) est undef",      isundef(tan2(1,0)),      -1
check "cotan2(1,1) = cotan(1)",   round(cotan2(1,1),9),    round(cotan(1),9)
check "cotan2(1,0) est undef",    isundef(cotan2(1,0)),    -1

print "\n=== 3. secante et cosecante ===\n"

check "secant(0)",                secant(0),               1
check "secant = 1/cos",           round(secant(1)*cos(1),9), 1
check "cosecant(pi()/2)",         round(cosecant(pi()/2),9), 1
check "cosecant = 1/sin",         round(cosecant(1)*sin(1),9), 1
check "cosecant(0) est undef",    isundef(cosecant(0)),    -1

print "\n=== 4. reciproques ===\n"

check "round(actan(1),9)",        round(actan(1),9),       round(pi()/4,9)
check "round(actan(0),9)",        round(actan(0),9),       round(pi()/2,9)
' La branche continue : au-dela de zero, l'arc cotangente reste entre 0 et PI.
check "round(actan(-1),9)",       round(actan(-1),9),      round(3*pi()/4,9)
check "actan = pi/2-atan",        round(actan(2)+atan(2),9), round(pi()/2,9)
check "round(asecant(2),9)",      round(asecant(2),9),     round(pi()/3,9)
check "asecant(1)",               asecant(1),              0
check "round(asecant(-1),9)",     round(asecant(-1),9),    round(pi(),9)
check "asecant(0.5) est undef",   isundef(asecant(0.5)),   -1
check "asecant(0) est undef",     isundef(asecant(0)),     -1
check "round(acosecant(2),9)",    round(acosecant(2),9),   round(pi()/6,9)
check "round(acosecant(1),9)",    round(acosecant(1),9),   round(pi()/2,9)
check "acosecant(-1)",            round(acosecant(-1),9),  round(-pi()/2,9)
check "acosecant(0.5) est undef", isundef(acosecant(0.5)), -1

print "\n=== 5. hyperboliques ===\n"

check "hsin(0)",                  hsin(0),                 0
check "hcos(0)",                  hcos(0),                 1
check "htan(0)",                  htan(0),                 0
check "hsecant(0)",               hsecant(0),              1
check "hcos(1) = (e+1/e)/2",      round(hcos(1),9),        round((exp(1)+exp(-1))/2,9)
check "hsin(1) = (e-1/e)/2",      round(hsin(1),9),        round((exp(1)-exp(-1))/2,9)
check "htan = hsin/hcos",         round(htan(1),9),        round(hsin(1)/hcos(1),9)
check "hsecant = 1/hcos",         round(hsecant(1)*hcos(1),9), 1
check "hctan = 1/htan",           round(hctan(1)*htan(1),9), 1
check "hcosecant = 1/hsin",       round(hcosecant(1)*hsin(1),9), 1
check "hcosecant(0) est undef",   isundef(hcosecant(0)),   -1
check "hctan(0) est undef",       isundef(hctan(0)),       -1

print "\n=== 6. reciproques hyperboliques ===\n"

check "ahsin(0)",                  ahsin(0),                 0
check "ahsin(hsin(1))",            round(ahsin(hsin(1)),9),  1
check "ahsin(1) = ln(1+sqrt(2))",  round(ahsin(1),9),        round(log(1+sqrt(2)),9)
check "ahcos(1)",                  ahcos(1),                 0
check "ahcos(hcos(1))",            round(ahcos(hcos(1)),9),  1
check "ahcos(2) = ln(2+sqrt(3))",  round(ahcos(2),9),        round(log(2+sqrt(3)),9)
check "ahtan(0)",                  ahtan(0),                 0
check "ahtan(htan(1))",            round(ahtan(htan(1)),9),  1
check "ahtan(0.5) = ln(3)/2",      round(ahtan(0.5),9),      round(log(3)/2,9)
check "ahsecant(1)",               ahsecant(1),              0
check "ahsecant(0.5) = ahcos(2)",  round(ahsecant(0.5),9),   round(ahcos(2),9)
check "ahcosecant(1) = ahsin(1)",  round(ahcosecant(1),9),   round(ahsin(1),9)
check "ahcosecant(-1) = -ahsin(1)", round(ahcosecant(-1),9),  round(-ahsin(1),9)
check "ahctan(2) = ahtan(0.5)",    round(ahctan(2),9),       round(ahtan(0.5),9)
check "ahctan(-2) = -ahtan(0.5)",  round(ahctan(-2),9),      round(-ahtan(0.5),9)
check "ahctan(hctan(1))",          round(ahctan(hctan(1)),9), 1
check "ahcos(0.5) est undef",      isundef(ahcos(0.5)),      -1
check "ahtan(1) est undef",        isundef(ahtan(1)),        -1
check "ahtan(-1) est undef",       isundef(ahtan(-1)),       -1
check "ahsecant(0) est undef",     isundef(ahsecant(0)),     -1
check "ahsecant(1.5) est undef",   isundef(ahsecant(1.5)),   -1
check "ahcosecant(0) est undef",   isundef(ahcosecant(0)),   -1
check "ahctan(1) est undef",       isundef(ahctan(1)),       -1
check "ahctan(0.5) est undef",     isundef(ahctan(0.5)),     -1

print "\n", essais, " essais, ", echecs, " echec(s)\n"
if echecs then error 1
