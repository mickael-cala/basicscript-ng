' refus_directory.bas - « open ... for directory » est refuse a l'analyse.
'
' Sert au cas « refus/for-directory » de tests/run.py, qui attend une erreur
' de syntaxe et non une execution. La ligne etait autrefois acceptee : OPEN
' prenait « directory » pour un mode, ouvrait un fichier de ce nom et rendait
' l'erreur 22 a l'execution (ERRATA E28). Un refus a l'analyse vaut mieux.
'
' Le langage n'a qu'une forme pour lister un repertoire :
'   open directory "tests" pattern "*.bas" option 0 as 1
open "tests" for directory as 1
print "ECHEC : la ligne a ete acceptee"
