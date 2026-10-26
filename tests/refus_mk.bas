' refus_mk.bas - la famille MK* a quitte la grammaire.
'
' MKD, MKI, MKL et MKS fabriquaient une chaine binaire a partir d'un nombre.
' Elles n'avaient pas de corps : la grammaire les acceptait, l'execution
' rendait l'erreur 28. Elles ne sont plus des symboles : un appel est une
' fonction inconnue, signalee avant l'execution (error &H68).
'
' Le marqueur ne doit jamais s'afficher.

print mki(1)
print "jamais execute"
