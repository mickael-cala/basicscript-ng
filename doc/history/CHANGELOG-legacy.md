# CHANGELOG hérité — ScriptBasic (amont)

> Copie de référence de l'historique des versions du projet **amont** ScriptBasic, conservée pour contextualiser le portage. Les numéros de version/build exacts sont visibles dans le dépôt via `old/version.txt` (2.0) et la gestion de build `newbuild.bas` + `buildnum.c`.

## Note de méthode

L'amont n'a pas tenu de changelog exhaustif versionné dans le dépôt source ; l'essentiel de l'histoire des releases est documenté :
- sur le site historique **http://www.scriptbasic.com** (cf. `old/readme.txt`) ;
- dans la documentation Texinfo (`old/texi/`, `old/esd.texi.jam`) ;
- dans l'historique Git du dépôt amont https://github.com/verhas/ScriptBasic.

Cette page compile les jalons connus ; elle doit être complétée en Phase 1 (ROADMAP) en extrayant les notes de release amont.

## Jalons connus

| Période | Version | Faits marquants |
|---------|---------|-----------------|
| 1998 | 0.x | Première publication (SourceForge), interpréteur BASIC en C |
| 2000s | 1.0–1.x | Extensions (mysql, xml, cgi, gd…), modes standalone/httpd, builds Windows & Unix |
| 2010s | 1.2+ | Migration GitHub, préprocesseur externe (`epreproc`), LSP server expérimental |
| 2020 | **2.0** | Dernier état amont ; base du fork BasicScript (présent dépôt, archivé dans `old/`) |

## Licence amont

LGPL v2.1 (voir `old/LICENSE.txt`). Les mentions de copyright Peter Verhas et contributeurs restent en vigueur sur tout le contenu de `old/`.

## À faire
- [ ] Extraire l'historique des tags/releases du dépôt amont et le fusionner ici (Phase 1)
- [ ] Conserver une copie offline des notes de release du site scriptbasic.com si encore disponible
