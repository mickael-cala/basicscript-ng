' refus_operateur.bas - les operateurs morts sont refuses a l'analyse.
'
' Le langage declarait 257 operateurs a deux caracteres ; 256 d'entre eux
' n'etaient que des entrees d'extops.c, acceptees par le lexer puis rendues
' par l'erreur 28 (COMMAND_ERROR_NOTIMP) a l'execution. Ils ont quitte
' NASYMBOLS, BINARIES, UNARIES et extops.c : leur emploi est desormais une
' erreur de syntaxe &H77 a l'analyse.
'
' == est l'un d'eux. Les survivants (+= -= << >> ?? ** // #) sont exerces par
' tests/operators.bas ; ce fichier tient l'autre moitie de la propriete.
'
' Le marqueur ne doit jamais s'afficher.

print 1 == 1
print "jamais execute"
