' contrat 04 : tableaux - sans module
dim a[5]
for i = 1 to 5
  a[i] = i * i
next i
print "lb   : ", lbound(a), "\n"
print "ub   : ", ubound(a), "\n"
for i = 1 to 5
  print "a[", i, "] = ", a[i], "\n"
next i
dim m[2, 3]
m[1, 2] = 12
m[2, 3] = 23
print "m1,2 : ", m[1, 2], "\n"
print "m2,3 : ", m[2, 3], "\n"
print "fin 04\n"
