Sources zlib 1.3.1 (15 .c, 11 .h), copiees sans retouche depuis
ScriptBasic-master-ng (qui les avait prises de
https://zlib.net/fossils/zlib-1.3.1.tar.gz, SHA256
9a93b2b7dfdac77ceba5a558a580e74667dd6fede4585b91eefb60f03b72df23 ;
cf. PATCHES.md B.10 de -ng). zconf.h est celui de la distribution
vanille, pas une variante generee.

INTOUCHABLES comme tout src/vendor/ : on remplace le dossier d'un bloc
par une copie fraiche d'une version plus recente, on n'edite pas les
fichiers. La licence zlib est dans LICENSE. Le module qui les consomme
est src/extensions/zlib/ (interface seule ; les 15 .c sont listes dans
la cle "zlib" de build.py et compiles avec -Isrc/vendor/zlib).
