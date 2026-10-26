' err_module.bas - erreur d'un module a l'execution, code > MAX_ERROR_CODE
'
' Sert au cas « erreur-module » de tests/run.py. cgi::SaveFile sur un nom
' qui n'est pas un fichier televerse rend 0x00080000, au-dela de
' MAX_ERROR_CODE (133) : le rapporteur imprime alors le code en 0x%08x et
' remplace le message par « Extension specific error: ... ». Le serveur doit
' repondre 200 avec la page d'erreur complete et le message du module.
' Le script n'ecrit rien avant : la reponse ne contient que cette page.
import cgi.bas
cgi::SaveFile "pas-televerse", "sortie.bin"
print "jamais atteint"
