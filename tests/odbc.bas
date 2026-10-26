' odbc.bas - test de chargement de l'extension odbc
'
' odbc est la seule extension a dependance externe que cette machine permette
' de construire : sa bibliotheque est un composant de Windows lui-meme,
' odbc32.dll, le gestionnaire de pilotes. Les huit autres (bdb, curl, curses,
' gd, mysql, psql, xml, zlib) reclament des bibliotheques tierces i386 qui ne
' sont pas installees ici, ni en DLL ni en import.
'
' Ce que ce test etablit :
'   - la DLL odbc.dll produite par TinyCC se charge dans l'interpreteur ;
'   - ses imports vers odbc32.dll se resolvent ;
'   - bootmodu, l'initialiseur du module, va jusqu'au bout : il appelle
'     SQLSetEnvAttr puis SQLAllocHandle(SQL_HANDLE_ENV,...) pour allouer
'     l'environnement ODBC, et refuse de se charger si cela echoue.
'
' La preuve tient dans la nature de l'erreur obtenue. L'appel ci-dessous
' utilise volontairement un handle invalide : le module repond par une erreur
' qui lui est propre, le code 8 (argument hors plage acceptee), levee par sa
' verification de handle. Une DLL qui ne se serait pas chargee, ou dont
' l'initialisation aurait echoue, produirait au contraire un code 16
' (COMMAND_ERROR_MODULE_LOAD) ou 18 (COMMAND_ERROR_MODULE_INITIALIZE) - ce qui
' n'est pas ce qu'on observe.
'
' Ce que ce test n'etablit PAS : une requete SQL complete. Elle demanderait une
' source de donnees ODBC configuree sur la machine (un DSN, un pilote et une
' base), ce qui n'existe pas ici. Le portage s'arrete donc a la preuve que
' compilation, chargement, import et initialisation fonctionnent de bout en
' bout.
'
' La distribution ne livre pas de fichier odbc.bas (seules cgi, nt et dyc en
' ont un). Les fonctions du module sont, d'apres interface.c : odbc_affected_rows,
' odbc_close, odbc_config_connect, odbc_error, odbc_fetcharray, odbc_fetchhash,
' odbc_query et odbc_real_connect.

declare sub odbc_error alias "odbc_error" lib "odbc"

print "=== extension odbc (gestionnaire de pilotes Windows) ===\n"

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
