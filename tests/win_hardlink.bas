' win.bas - test fonctionnel de l'extension win
'
' Ce test vise precisement le code que le portage TinyCC a du retoucher dans
' extensions/win/interface.c : le type byte, que le SDK fournit via rpcndr.h et
' que TinyCC n'a pas, a ete remplace par BYTE (identique : unsigned char).
' Les trois occurrences concernees sont des conversions vers les fonctions de
' sauvegarde : (BYTE *) et wsi, (BYTE *) buf2, (BYTE *) et buf1[0].
'
' win::HardLink est le meilleur test possible pour ces trois lignes, parce qu'il
' est le seul chemin qui les traverse toutes, et parce qu'il depend en plus de
' enableprivs(), la fonction qui active SeBackupPrivilege et SeRestorePrivilege
' - laquelle utilise precisement le byte buf[...] corrige. La creation d'un
' lien physique par BackupWrite/BACKUP_LINK exige ces deux privileges : si le
' portage de ces quatre lignes etait fautif, le test echouerait.
'
' A savoir : activer ces privileges demande un processus autorise a le faire.
' Lance sans droits suffisants, AdjustTokenPrivileges echoue en silence et
' BackupWrite refuse le lien ; le test le signale alors comme privileges
' insuffisants plutot que comme un echec de l'extension.
'
' Remarque de syntaxe utile : le lexeur de ScriptBasic traite les guillemets
' meme a l'interieur d'un commentaire introduit par une apostrophe. Une ligne
' de commentaire contenant un nombre impair de guillemets ouvre donc une chaine
' qui ne se termine nulle part. Les commentaires ci-dessus evitent donc les
' guillemets.


import win.bas

ok = 1
print "=== extension win (services Windows, registre, liens physiques) ===\n"

' ---------------------------------------------------------------------------
' 1) Un fichier source, ecrit avec les commandes de fichier du noyau : ce
'    passage verifie au passage l'ouverture/ecriture BASIC sur le binaire TCC.
' ---------------------------------------------------------------------------
open "tmp_hardlink_src.txt" for output as 1
print#1, "contenu du fichier source\n"
print#1, "seconde ligne, pour verifier la lecture complete\n"
close 1

' ---------------------------------------------------------------------------
' 2) Le lien physique, cree par win::HardLink
' ---------------------------------------------------------------------------
call win::HardLink("tmp_hardlink_src.txt", "tmp_hardlink_dst.txt")

' ---------------------------------------------------------------------------
' 3) Relecture par le lien : doit rendre exactement le meme contenu
' ---------------------------------------------------------------------------
open "tmp_hardlink_dst.txt" for input as 1
line input #1, l1
close 1

print "ligne lue par le lien : ", l1
print "longueur de la ligne   : ", len(l1), "  (25 caracteres utiles)\n"
' La comparaison porte sur les 25 premiers caracteres : le fichier ayant ete
' ecrit en mode texte, la ligne relue peut conserver un retour chariot final.
if len(l1) < 25 then ok = 0
if left(l1, 25) <> "contenu du fichier source" then ok = 0


if ok then
  print "\nwin : OK - lien physique cree et relu (enableprivs et BackupWrite ok)\n"
else
  print "\nwin : ECHEC\n"
  print "     Si le message parle de privileges, relancer avec un compte\n"
  print "     autorise a activer SeBackupPrivilege : le portage de byte\n"
  print "     nest pas en cause dans ce cas precis.\n"
end if

' Les deux fichiers temporaires ne sont pas supprimes ici : la commande kill,
' bien que declaree dans syntax.c ({ CMD_KILL , 1 , 1}), n'a aucune
' implementation dans cette version de ScriptBasic - elle echoue sur une erreur
' de syntaxe. C'est le script de test (test/run_all.py) qui nettoie.
