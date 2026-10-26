En-tetes de libcurl 8.21.0 (CURL_VERSION 8.21.0, timbre 2026-06-24),
copies de ScriptBasic-master-ng (resources/curl), eux-memes extraits du
tarball officiel curl-8.21.0. Seul system.h a ete retouche (par -ng,
octobre 2026) ; COPYING (licence MIT de curl) est inclus.

Le .def (libcurl-4.def) decrit la DLL d'execution MinGW de Git for
Windows (libcurl-4.dll, installee dans C:\Program Files\Git\mingw64\bin,
repertoire present dans le PATH systeme). La DLL n'est PAS versionnee :
c'est une dependance du systeme, comme odbc32.dll. build.py fabrique la
bibliotheque d'import par `zig dlltool` depuis ce .def, a chaque
construction du module.

INTOUCHABLES comme tout src/vendor/ : on remplace d'un bloc par une
copie fraiche d'une version plus recente, on n'edite pas les fichiers.
Perimetre actuel du module : windows-x64 seulement (pas de DLL 32 bits
ici, pas de libcurl de developpement sous UNIX) - ERRATA E40.
