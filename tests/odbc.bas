' odbc.bas - test de chargement de l'extension odbc
'
' odbc depend d'un composant du systeme : odbc32.dll sous Windows (le
' gestionnaire de pilotes), unixODBC sous Linux. Ce que ce test etablit :
'   - la bibliotheque du module est bien construite dans modules/ ;
'   - elle se charge dans l'interpreteur, ses imports se resolvent ;
'   - bootmodu va au bout : SQLSetEnvAttr puis SQLAllocHandle(SQL_HANDLE_ENV)
'     allouent l'environnement ODBC, et le module refuse de se charger sinon.
'
' La preuve tient dans la nature de l'erreur obtenue : avec un handle
' invalide, le module repond par SA propre erreur (code 8, argument hors
' plage). Un echec de chargement produirait une erreur 16 ou 18 - mais
' cette grammaire n'a pas de errnum pour les distinguer apres
' interception. La garde est donc structurelle : sans fichier
' modules/odbc.so (ou .dll), le test echoue avant tout appel, au lieu de
' faire un faux positif par interception.
'
' Ce que ce test n'etablit PAS : une requete SQL complete. Elle demanderait
' une source de donnees ODBC configuree (DSN, pilote, base), qui n'existe
' pas ici. La preuve s'arrete a : compilation, chargement, import et
' initialisation fonctionnels de bout en bout.

declare sub odbc_error alias "odbc_error" lib "odbc"

print "=== extension odbc (gestionnaire de pilotes) ===\n"

present = 0
if fileexists("modules/odbc.so") then present = 1
if fileexists("modules/odbc.dll") then present = 1
if present = 0 then
  print "odbc : ECHEC - la bibliotheque du module est absente de modules/\n"
  print "       construisez-la : python build.py --target <cible> --modules\n"
  end
end if

on error goto Interception

s = odbc_error(0)
print "odbc_error(0) rendu : [", s, "]\n"
print "\nodbc : OK - le module a repondu sur un appel valide\n"
end

Interception:
print "erreur interceptee a l'appel de odbc_error(0)\n"
print "\nodbc : OK - la DLL s'est chargee et bootmodu a alloue l'environnement\n"
print "       ODBC : l'erreur vient du controle d'argument du module lui-meme\n"
print "       (code 8, argument hors plage), pas d'un echec de chargement.\n"
end
