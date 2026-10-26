' refus_pause.bas - PAUSE a quitte la grammaire.
'
' La commande dormait en NOTIMPLEMENTED : elle s'analysait puis rendait
' l'erreur 28 a l'execution. Elle est desormais un mot inconnu, refuse a
' l'analyse (error &H77). Le marqueur ne doit jamais s'afficher.

pause 100
print "jamais execute"
