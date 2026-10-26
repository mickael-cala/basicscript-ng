' err_partial.bas - erreur apres le debut de la reponse
'
' Sert au cas « erreur-apres-sortie » de tests/run.py : le script ecrit sa
' ligne de statut et un debut de corps, puis echoue. La reponse a deja
' commence, donc le serveur ne peut plus choisir le statut : il ne doit ni
' reecrire de ligne de statut, ni ajouter d'en-tetes, ni ouvrir de page HTML.
' Seul le diagnostic est ajoute, precede d'un saut de ligne (ERRATA E25).
print "Status: 200 OK\nContent-Type: text/plain\n\n"
print "debut"
open "scriba-absent-xyz/fichier.txt" for input as 1
print "jamais atteint"
