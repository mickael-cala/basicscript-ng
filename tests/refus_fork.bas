' refus_fork.bas - FORK() a quitte la grammaire.
'
' La fonction etait declaree, rendait l'erreur 28 a l'execution, et le module
' unix fournit deja fork(). Un appel a fork() est maintenant une fonction
' inconnue, signalee avant l'execution (error &H68). Le marqueur ne doit
' jamais s'afficher.

print fork()
print "jamais execute"
