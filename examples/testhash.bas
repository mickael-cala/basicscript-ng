import hash.bas
Const nl="\n"
h = hash::New()
hash::SetValue h,"evil",666
Zazu = hash::Value(h,"evil")
print Zazu,nl
' Value renvoie une copie : changer Zazu ne change pas la table.
Zazu = "BILL"
print hash::Value(h,"evil"),nl
hash::Release h
