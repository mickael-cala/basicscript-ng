' err_load.bas - erreur de chargement, avant toute execution
'
' Sert au cas « erreur-chargement » de tests/run.py. L'include vise un
' fichier qui n'existe pas : l'erreur est detectee par le lecteur pendant
' le chargement, puis rapportee avec ses propres drapeaux, donc la reponse
' du serveur doit deja etre complete (statut 200 et page HTML). C'est
' l'autre chemin du rapporteur, distinct de celui d'err_runtime.bas qui
' echoue a l'execution (ERRATA E24).
include "scriba-absent-xyz.bas"
print "jamais atteint"
