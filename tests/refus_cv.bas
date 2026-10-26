' refus_cv.bas - la famille CV* a quitte la grammaire.
'
' CVD, CVI, CVL et CVS convertissaient une chaine binaire en nombre. Elles
' n'avaient pas de corps : la grammaire les acceptait, l'execution rendait
' l'erreur 28. Elles ne sont plus des symboles : un appel est une fonction
' inconnue, signalee avant l'execution (error &H68).
'
' Le marqueur ne doit jamais s'afficher.

print cvd("ab")
print "jamais execute"
