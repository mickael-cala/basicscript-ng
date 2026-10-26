/*
libscriba.h — la notice du composant ScriptBasic2-ng.

LA bibliothèque est l'archive `libscriba.a` (construite par
`python build.py --target <cible> --libscriba`, avec les en-têtes dans
`dist/<cible>/include/`). Ce fichier est la porte d'entrée : il inclut
l'API complète (`scriba.h`, 71 fonctions) et documente le cheminement
recommandé d'un programme hôte.

Cheminement minimal d'un hôte :

    pSbProgram p = scriba_new(malloc, free);      // sortir le moteur
    scriba_LoadConfiguration(p, NULL);            // config facultative
    scriba_SetFileName(p, "script.bas");          // le programme BASIC
    scriba_LoadSourceProgram(p);                  // lecture + analyse
    scriba_Run(p, "");                            // exécution
    scriba_destroy(p);                            // rangement

Aller plus loin (éprouvé par examples/embed/ et la campagne --embed) :

    scriba_LookupFunctionByName(p, "main::mafonction")  -> noeud
    (la table interne est en minuscules ; noeud <= 0 = echec)
    scriba_CallArgEx(p, noeud, retour, n, args)         -> appel avec arguments
    scriba_LookupVariableByName(p, "main::total")       -> numéro de variable
    scriba_GetVariable(p, numero, &donnees)             -> lecture
    scriba_GetLong / scriba_GetString / scriba_GetDouble(donnees)

Deux pièges connus :

- sous UNIX, l'hôte doit définir `char **_environ;` : sinon l'éditeur de
  liens tire de l'archive l'objet qui porte un main() de démonstration ;
- l'archive n'embarque pas les modules dynamiques : un script qui importe
  `zlib` a besoin de `modules/` et d'une configuration (ou d'un exécutable
  construit par `--standalone --modules`, qui les colle dedans).

Le moteur s'attend à recevoir malloc/free (ou équivalent) ; c'est le
premier argument de scriba_new.
*/
#ifndef __LIBSCRIBA_H__
#define __LIBSCRIBA_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif

/* Les en-tetes du projet ne s'incluent pas eux-memes (heritage du
   generateur, voir README) : la notice fournit donc les standards que
   scriba.h et ses voisins supposent presents, comme le fait build.py
   pour compiler l'interpreteur. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "scriba.h"

#ifdef __cplusplus
}
#endif
#endif
