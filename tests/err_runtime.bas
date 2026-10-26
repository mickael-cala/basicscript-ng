' err_runtime.bas - erreur d'execution, sans aucune sortie prealable
'
' Sert au cas « erreur-execution » de tests/run.py. Le programme echoue des
' sa premiere instruction, donc avant tout en-tete CGI : la reponse du
' serveur ne contient que sa page d'erreur. Le dossier vise n'existe pas,
' sur aucune plateforme, et l'ouverture echoue a l'execution.
'
' Le code rendu est COMMAND_ERROR_FILE_CANNOT_BE_OPENED (22), dont le bit
' 0x2 est leve. HttpProc passait autrefois ce code comme mot de drapeaux a
' httpd_report, qui y teste REPORT_F_FRST (0x2) et sautait alors la ligne de
' statut : le client levait BadStatusLine (ERRATA E24). Le test exige donc
' un statut 200 et la page HTML complete, en plus du code 22.
open "scriba-absent-xyz/fichier.txt" for input as 1
print "jamais atteint"
