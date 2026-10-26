' contrat 07 : fichiers textes - sans module, nettoyage garanti
' (open exige sa clause for ; line input garde la fin de ligne : chomp)
fn = "contrat_07_tmp.txt"
open fn for output as 1
print#1, "ligne un"
print#1, "ligne deux"
print#1, "ligne trois"
close 1
print "existe   : ", fileexists(fn), "\n"
open fn for input as 1
n = 0
while eof(1) = 0
  line input #1, l
  n = n + 1
  print "lu ", n, " : ", chomp(l), "\n"
wend
close 1
print "lignes   : ", n, "\n"
delete fn
print "apres    : ", fileexists(fn), "\n"
print "fin 07\n"
