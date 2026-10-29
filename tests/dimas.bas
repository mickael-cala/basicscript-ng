' dimas.bas - DIM variable AS type : la contrainte de type (phase 8.3).
'
' Ce que le test etablit :
'   - DIM x AS integer/string/real declare et la variable repond ;
'   - la contrainte est verifiee a CHAQUE affectation :
'     * un reel non entier dans AS integer leve l'erreur 134 trappable ;
'   - string accepte les nombres (tout est convertible) ;
'   - un champ de type inconnu est refuse a l'analyse (refus	separe).
'
' La contrainte est calculee a l'analyse (ex_IsCommandDIMAS marque le
' node), copiee par le builder, verifiee par COMMAND(LET) au runtime.

global echecs
global essais
global ok
echecs = 0
essais = 0
ok = 1

sub check(nom, obtenu, attendu)
  essais = essais + 1
  if str(obtenu) = str(attendu) then
    print "  OK    ", nom, "   -> ", str(obtenu), "\n"
  else
    print "  ECHEC ", nom, "   obtenu=", str(obtenu), " attendu=", str(attendu), "\n"
    echecs = echecs + 1
  endif
end sub

print "=== DIM AS : la contrainte de type ===\n"

' 1. les declarations et affectations valides
dim x as integer
x = 5
check "integer affecte",      x,           5
dim s as string
s = "bonjour"
check "string affecte",       s,           "bonjour"
dim r as real
r = 2.5
check "real affecte",         r,           2.5

' 2. la contrainte : un reel non entier dans AS integer leve 134
on error goto Contrainte
x = 2.5
print "  ECHEC : la contrainte n'a pas leve\n"
echecs = echecs + 1
goto ApresContrainte
Contrainte:
check "contrainte leve 134",  error,       134
ApresContrainte:
on error goto null

' 3. un reel entier passe dans AS integer (conversion exacte) :
'    la comparaison est numerique (str(4.0) rend "4.000000")
on error goto Contrainte2
x = 4.0
check "reel entier passe",    x = 4,       -1
goto ApresContrainte2
Contrainte2:
print "  ECHEC : 4.0 a ete refuse\n"
echecs = echecs + 1
ApresContrainte2:
on error goto null

if ok = 1 and echecs = 0 then
  print "\ndimas : OK\n"
else
  print "\ndimas : ECHEC\n"
end if
print "essais : ", essais, "   echecs : ", echecs, "\n"
