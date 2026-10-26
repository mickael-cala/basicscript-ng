' contrat 08 : undef et reflexivite - sans module
u = undef
print "isundef  : ", isundef(u), "\n"
print "isdef    : ", isdefined(u), "\n"
dim t[3]
t[1] = 1
print "isarray  : ", isarray(t), "\n"
print "isstr    : ", isstring("a"), "\n"
print "isint    : ", isinteger(5), "\n"
print "isreal   : ", isreal(2.5), "\n"
print "isnumint : ", isnumeric(5), "\n"
print "isnumstr : ", isnumeric("12"), "\n"
print "isempty  : ", isempty(""), "\n"
print "fin 08\n"
