' refus_sqr.bas - SQR n'existe plus, c'est SQRT.
'
' Le nom historique SQR a laisse la place a SQRT, comme dans tous les autres
' langages. L'ancien nom est une fonction inconnue, signalee avant
' l'execution (error &H68), et non un alias silencieux. Le marqueur ne doit
' jamais s'afficher.

print sqr(9)
print "jamais execute"
