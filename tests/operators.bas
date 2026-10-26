' operators.bas - les operateurs etendus qui restent
'
' Le langage declarait 257 operateurs a deux caracteres, dont 256 refuses
' par extops.c (erreur 28). Le 2026-10-23, la grammaire a ete reduite a :
'
'   += -= *= /= &= \=   affectations composees (deja implementees par LET)
'   << >>              decalages de bits
'   ??                 valeur de gauche si definie, sinon celle de droite
'   **                 puissance, alias de ^
'   //                 division entiere, alias de \
'   #                  le tiret de fichier
'
' Les autres ont ete retires de NASYMBOLS : leur emploi est maintenant une
' erreur de syntaxe &H77 a l'analyse, et non une erreur 28 a l'execution.
' Ce test n'exerce que les survivants ; le refus des autres est tenu par
' tests/refus_operateur.bas, que le lanceur compte parmi les refus attendus.

ok = 1

' --- affectations composees ------------------------------------------------
a = 5
a += 3
a -= 2
a *= 4
a /= 3
print "affectations : ", a, "\n"
if a <> 8 then ok = 0

s = "ab"
s &= "cd"
print "concatenation: ", s, "\n"
if s <> "abcd" then ok = 0

i = 17
i \= 5
print "division int.: ", i, "\n"
if i <> 3 then ok = 0

' --- decalages -------------------------------------------------------------
l = 1 << 4
r = 256 >> 4
n = -8 >> 1
print "decalages    : ", l, " ", r, " ", n, "\n"
if l <> 16 then ok = 0
if r <> 16 then ok = 0
if n <> -4 then ok = 0

' --- puissance et division entiere ----------------------------------------
p = 2 ** 10
d = 7 // 2
m = -7 // 2
print "puissance    : ", p, "\n"
print "division //  : ", d, " ", m, "\n"
if p <> 1024 then ok = 0
if d <> 3 then ok = 0
if m <> -3 then ok = 0

' --- coalescence -----------------------------------------------------------
' u n'est jamais affecte : il est undef.
defaut = u ?? 42
valeur = "present" ?? (1 \ 0)
print "coalescence  : ", defaut, " ", valeur, "\n"
if defaut <> 42 then ok = 0
if valeur <> "present" then ok = 0
' La droite ne doit pas etre evaluee quand la gauche est definie : sans le
' court-circuit, (1 \ 0) aurait provoque une erreur de division.

if ok then
  print "\noperateurs : OK\n"
else
  print "\noperateurs : ECHEC\n"
end if
