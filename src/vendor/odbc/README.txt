En-tetes unixODBC (sql.h, sqlext.h, sqltypes.h, (sqlucode.h, unixodbc.h - multiarch), copies de
/usr/include d'Ubuntu 26.04 (paquet unixodbc-dev). Ils rendent le module
odbc compilable par zig cc pour linux-x64, dont le sysroot n'embarque pas
ces en-tetes : ils ne font pas partie de la libc.

INTOUCHABLES comme tout src/vendor/ : on les remplace d'un bloc par une
copie fraiche d'un unixodbc-dev plus recent, on ne les edite pas. Sous
Windows ces en-tetes ne sont pas utilises : zig fournit ceux de MinGW,
et la cle include_dirs du module odbc est conditionnee a la famille d'OS.
