/*
 * service.h - shim
 * ---------------
 *
 * Ce fichier est ABSENT de la distribution ScriptBasic : variations/httpd/
 * ne contient que service.c et websrv.c, et les deux font pourtant
 * #include "service.h". Il a donc ete ecrit ici pour le portage.
 *
 * Le contenu est complete au fur et a mesure que le compilateur signale les
 * symboles manquants. service.c et websrv.c declarent eux-memes l'essentiel de
 * leurs prototypes, ce fichier porte les constantes et noms partages.
 */

#ifndef __SERVICE_SHIM_H
#define __SERVICE_SHIM_H

/* L'API des services Windows vit dans winsvc.h, absent de TCC (shim fourni
   dans include/). service.c et websrv.c incluent tous deux "service.h"
   APRES leurs en-tetes Windows, ce qui en fait le bon endroit pour le tirer,
   sans avoir a modifier les sources du projet. */
#include <winsvc.h>

/* service.c definit ReportStatusToSCMgr, websrv.c l'appelle (ligne ~1676,
   au demarrage en mode service). TCC acceptait l'appel sans prototype ;
   clang refuse une declaration implicite, d'ou ce prototype, qui est celui
   de service.c. */
BOOL ReportStatusToSCMgr(DWORD dwCurrentState,
                         DWORD dwWin32ExitCode,
                         DWORD dwWaitHint);

/* Inversement, ces deux fonctions sont definies par websrv.c et appelees par
   service.c (traitement des ordres du gestionnaire de services). */
VOID ServiceStart(DWORD dwArgc, LPTSTR *lpszArgv);
VOID ServiceStop(void);

/* Nom du service Windows installe par `sbhttpd -i` et de sa description.
   Les deux fichiers doivent les partager pour s'accorder. */
#define SZSERVICENAME        "sbhttpd"
#define SZSERVICEDISPLAYNAME "ScriptBasic HTTP server"
#define SZDEPENDENCIES       ""

#endif /* __SERVICE_SHIM_H */
