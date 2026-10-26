' err_pending.bas - erreur alors qu'une premiere ligne attend encore
'
' Sert au cas « erreur-ligne-en-attente » de tests/run.py. Le script ecrit
' « debut » sans saut de ligne : pfStdOut garde cette ligne dans szBuffer et
' laisse FirstHeaderLine leve, donc aucun octet n'est encore parti sur le
' reseau. Le serveur peut donc encore choisir le statut : il repond 500 avec
' sa page d'erreur complete, comme pour err_runtime.bas.
'
' C'est le controle negatif de la condition d'ERRATA E25 : un tampon non
' vide ne signifie pas que la reponse a commence, et traiter ce cas comme
' une reponse commencee supprimerait la ligne de statut, que le client ne
' saurait plus lire (le defaut d'E24, par un autre chemin).
print "debut"
open "scriba-absent-xyz/fichier.txt" for input as 1
