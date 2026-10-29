' explicit_passe - avec declarevars 1, un programme qui declare tout
' (global, const, local) tourne jusqu'au bout.
global declaree
const LIMITE = 4
declaree = 5
sub S
  local l1
  l1 = LIMITE + declaree
  print "dans S : l1=", l1, "\n"
end sub
S
print "fin propre\n"
