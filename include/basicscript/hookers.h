/*
hookers.h
*/
#ifndef __HOOKERS_H__
#define __HOOKERS_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif

#include "filesys.h"

typedef struct _ExecuteObject *pExecuteObject;
#define PEXECUTEOBJECT 1

typedef struct _HookFunctions {

  void *hook_pointer;

#define HOOK_FILE_ACCESS(X) (pEo->pHookers->HOOK_file_access(pEo,(X)))
  int (*HOOK_file_access)(pExecuteObject, char *);

#define HOOK_FOPEN(X,Y) (pEo->pHookers->HOOK_fopen(pEo,(X),(Y)))
  FILE *(*HOOK_fopen)(pExecuteObject, char *, char *);

#define HOOK_FCLOSE(X) (pEo->pHookers->HOOK_fclose(pEo,(X)))
  void (*HOOK_fclose)(pExecuteObject, FILE *);

#define HOOK_SIZE(X) (pEo->pHookers->HOOK_size(pEo,(X)))
  long (*HOOK_size)(pExecuteObject, char *);

#define HOOK_TIME_ACCESSED(X) (pEo->pHookers->HOOK_time_accessed(pEo,(X)))
  long (*HOOK_time_accessed)(pExecuteObject, char *);

#define HOOK_TIME_MODIFIED(X) (pEo->pHookers->HOOK_time_modified(pEo,(X)))
  long (*HOOK_time_modified)(pExecuteObject, char *);

#define HOOK_TIME_CREATED(X) (pEo->pHookers->HOOK_time_created(pEo,(X)))
  long (*HOOK_time_created)(pExecuteObject, char *);

#define HOOK_ISDIR(X) (pEo->pHookers->HOOK_isdir(pEo,(X)))
  int (*HOOK_isdir)(pExecuteObject, char *);

#define HOOK_ISREG(X) (pEo->pHookers->HOOK_isreg(pEo,(X)))
  int (*HOOK_isreg)(pExecuteObject, char *);

#define HOOK_EXISTS(X) (pEo->pHookers->HOOK_exists(pEo,(X)))
  int (*HOOK_exists)(pExecuteObject, char *);

#define HOOK_TRUNCATE(X,Y) (pEo->pHookers->HOOK_truncate(pEo,(X),(Y)))
  int (*HOOK_truncate)(pExecuteObject, FILE *, long);

#define HOOK_FGETC(X) (pEo->pHookers->HOOK_fgetc(pEo,(X)))
  int (*HOOK_fgetc)(pExecuteObject, FILE *);

#define HOOK_FERROR(X) (pEo->pHookers->HOOK_ferror(pEo,(X)))
  int (*HOOK_ferror)(pExecuteObject, FILE *);

#define HOOK_FREAD(X,Y,Z,W) (pEo->pHookers->HOOK_fread(pEo,(X),(Y),(Z),(W)))
  int (*HOOK_fread)(pExecuteObject, char *, int, int, FILE *);

#define HOOK_SETMODE(X,Y) (pEo->pHookers->HOOK_setmode(pEo,(X),(Y)))
  void (*HOOK_setmode)(pExecuteObject,FILE *, int);

#define HOOK_BINMODE(X) (pEo->pHookers->HOOK_binmode(pEo,(X)))
  void (*HOOK_binmode)(pExecuteObject,FILE *);

#define HOOK_TEXTMODE(X) (pEo->pHookers->HOOK_textmode(pEo,(X)))
  void (*HOOK_textmode)(pExecuteObject,FILE *);

#define HOOK_FWRITE(X,Y,Z,W) (pEo->pHookers->HOOK_fwrite(pEo,(X),(Y),(Z),(W)))
  int (*HOOK_fwrite)(pExecuteObject, char *, int, int, FILE *);

#define HOOK_PUTC(X,Y) (pEo->pHookers->HOOK_fputc(pEo,(X),(Y)))
  int (*HOOK_fputc)(pExecuteObject, int, FILE *);

#define HOOK_FLOCK(X,Y) (pEo->pHookers->HOOK_flock(pEo,(X),(Y)))
  int (*HOOK_flock)(pExecuteObject, FILE *, int);

#define HOOK_LOCK(X,Y,Z,W) (pEo->pHookers->HOOK_lock(pEo,(X),(Y),(Z),(W)))
  int (*HOOK_lock)(pExecuteObject, FILE *, int, long, long);

#define HOOK_FEOF(X) (pEo->pHookers->HOOK_feof(pEo,(X)))
  int (*HOOK_feof)(pExecuteObject, FILE *);

#define HOOK_MKDIR(X) (pEo->pHookers->HOOK_mkdir(pEo,(X)))
  int (*HOOK_mkdir)(pExecuteObject, char *);

#define HOOK_RMDIR(X) (pEo->pHookers->HOOK_rmdir(pEo,(X)))
  int (*HOOK_rmdir)(pExecuteObject, char *);

#define HOOK_REMOVE(X) (pEo->pHookers->HOOK_remove(pEo,(X)))
  int (*HOOK_remove)(pExecuteObject, char *);

#define HOOK_DELTREE(X) (pEo->pHookers->HOOK_deltree(pEo,(X)))
  int (*HOOK_deltree)(pExecuteObject, char *);

#define HOOK_MAKEDIRECTORY(X) (pEo->pHookers->HOOK_MakeDirectory(pEo,(X)))
  int (*HOOK_MakeDirectory)(pExecuteObject, char *);

#define HOOK_OPENDIR(X,Y) (pEo->pHookers->HOOK_opendir(pEo,(X),(Y)))
  DIR *(*HOOK_opendir)(pExecuteObject, char *, tDIR *);

#define HOOK_READDIR(X) (pEo->pHookers->HOOK_readdir(pEo,(X)))
  struct dirent *(*HOOK_readdir)(pExecuteObject, DIR *);

#define HOOK_CLOSEDIR(X) (pEo->pHookers->HOOK_closedir(pEo,(X)))
  void (*HOOK_closedir)(pExecuteObject, DIR *);

#define HOOK_SLEEP(X) (pEo->pHookers->HOOK_sleep(pEo,(X)))
  void (*HOOK_sleep)(pExecuteObject, long);

#define HOOK_CURDIR(X,Y) (pEo->pHookers->HOOK_curdir(pEo,(X),(Y)))
  int (*HOOK_curdir)(pExecuteObject, char *, unsigned long);

#define HOOK_CHDIR(X) (pEo->pHookers->HOOK_chdir(pEo,(X)))
  int (*HOOK_chdir)(pExecuteObject, char *);

#define HOOK_CHOWN(X,Y) (pEo->pHookers->HOOK_chown(pEo,(X),(Y)))
  int (*HOOK_chown)(pExecuteObject, char *, char *);

#define HOOK_SETCREATETIME(X,Y) (pEo->pHookers->HOOK_SetCreateTime(pEo,(X),(Y)))
  int (*HOOK_SetCreateTime)(pExecuteObject, char *, long);

#define HOOK_SETMODIFYTIME(X,Y) (pEo->pHookers->HOOK_SetModifyTime(pEo,(X),(Y)))
  int (*HOOK_SetModifyTime)(pExecuteObject, char *, long);

#define HOOK_SETACCESSTIME(X,Y) (pEo->pHookers->HOOK_SetAccessTime(pEo,(X),(Y)))
  int (*HOOK_SetAccessTime)(pExecuteObject, char *, long);

  int (*HOOK_ExecBefore)(pExecuteObject);
  int (*HOOK_ExecAfter)(pExecuteObject);
  int (*HOOK_ExecCall)(pExecuteObject);
  int (*HOOK_ExecReturn)(pExecuteObject);

#define HOOK_GETHOSTNAME(X,Y) (pEo->pHookers->HOOK_gethostname(pEo,(X),(Y)))
  int (*HOOK_gethostname)(pExecuteObject, char *, long);
#define HOOK_GETHOST(X,Y) (pEo->pHookers->HOOK_gethost(pEo,(X),(Y)))
  int (*HOOK_gethost)(pExecuteObject, char *, struct hostent *);
#define HOOK_TCPCONNECT(X,Y) (pEo->pHookers->HOOK_tcpconnect(pEo,(X),(Y)))
  int (*HOOK_tcpconnect)(pExecuteObject, SOCKET *, char *);
#define HOOK_TCPSEND(X,Y,Z,W) (pEo->pHookers->HOOK_tcpsend(pEo,(X),(Y),(Z),(W)))
  int (*HOOK_tcpsend)(pExecuteObject, SOCKET, char *, long, int);
#define HOOK_TCPRECV(X,Y,Z,W) (pEo->pHookers->HOOK_tcprecv(pEo,(X),(Y),(Z),(W)))
  int (*HOOK_tcprecv)(pExecuteObject, SOCKET, char *, long, int);
#define HOOK_TCPCLOSE(X) (pEo->pHookers->HOOK_tcpclose(pEo,(X)))
  int (*HOOK_tcpclose)(pExecuteObject, SOCKET);

  int (*HOOK_killproc)(pExecuteObject, long);
#define HOOK_KILLPROC(X) (pEo->pHookers->HOOK_killproc(pEo,(X)))
  int (*HOOK_getowner)(pExecuteObject, char *, char *, long);
#define HOOK_GETOWNER(X,Y,Z) (pEo->pHookers->HOOK_getowner(pEo,(X),(Y),(Z)))
  char *(*HOOK_fcrypt)(pExecuteObject, char *, char *, char *);
#define HOOK_FCRYPT(X,Y,Z) (pEo->pHookers->HOOK_fcrypt(pEo,(X),(Y),(Z)))
  long (*HOOK_CreateProcess)(pExecuteObject, char *);
#define HOOK_CREATEPROCESS(X) (pEo->pHookers->HOOK_CreateProcess(pEo,(X)))

  int (*HOOK_CallScribaFunction)(pExecuteObject,
                                 unsigned long,
                                 pFixSizeMemoryObject *,
                                 unsigned long,
                                 pFixSizeMemoryObject *);
#define HOOK_CALLSCRIBAFUNCTION(X,Y,Z,W) (pEo->pHookers->HOOK_CallScribaFunction(pEo,(X),(Y),(Z),(W)))

  long (*HOOK_CreateProcessEx)(pExecuteObject,
                              char *,
                              long,
                              unsigned long *,
                              unsigned long *);
#define HOOK_CREATEPROCESSEX(P,TO,PID,EXIT) (pEo->pHookers->HOOK_CreateProcessEx(pEo,(P),(TO),(PID),(EXIT)))

  int (*HOOK_waitpid)(pExecuteObject, long, unsigned long *);
#define HOOK_WAITPID(X,Y) (pEo->pHookers->HOOK_waitpid(pEo,(X),(Y)))
  } HookFunctions
#ifndef PHOOKFUNCTIONS  
  , *pHookFunctions
#endif
  ;

/*FUNDEF*/

int hook_Init(pExecuteObject pEo,
              pHookFunctions *pHookers);
/*FEDNUF*/
/*FUNDEF*/

int hook_file_access(pExecuteObject pEo,
                     char *pszFileName);
/*FEDNUF*/
/*FUNDEF*/

FILE *hook_fopen(pExecuteObject pEo,
                 char *pszFileName,
                 char *pszOpenMode);
/*FEDNUF*/
/*FUNDEF*/

void hook_fclose(pExecuteObject pEo,
                  FILE *fp);
/*FEDNUF*/
/*FUNDEF*/

long hook_size(pExecuteObject pEo,
               char *pszFileName);
/*FEDNUF*/
/*FUNDEF*/

long hook_time_accessed(pExecuteObject pEo,
                        char *pszFileName);
/*FEDNUF*/
/*FUNDEF*/

long hook_time_modified(pExecuteObject pEo,
                        char *pszFileName);
/*FEDNUF*/
/*FUNDEF*/

long hook_time_created(pExecuteObject pEo,
                        char *pszFileName);
/*FEDNUF*/
/*FUNDEF*/

int hook_isdir(pExecuteObject pEo,
               char *pszFileName);
/*FEDNUF*/
/*FUNDEF*/

int hook_isreg(pExecuteObject pEo,
               char *pszFileName);
/*FEDNUF*/
/*FUNDEF*/

int hook_exists(pExecuteObject pEo,
                char *pszFileName);
/*FEDNUF*/
/*FUNDEF*/

int hook_truncate(pExecuteObject pEo,
                  FILE *fp,
                  long lNewFileSize);
/*FEDNUF*/
/*FUNDEF*/

int hook_fgetc(pExecuteObject pEo,
               FILE *fp);
/*FEDNUF*/
/*FUNDEF*/

int hook_ferror(pExecuteObject pEo,
               FILE *fp);
/*FEDNUF*/
/*FUNDEF*/

int hook_fread(pExecuteObject pEo,
               char *buf,
               int size,
               int count,
               FILE *fp);
/*FEDNUF*/
/*FUNDEF*/

void hook_setmode(pExecuteObject pEo,
                  FILE *fp,
                  int mode);
/*FEDNUF*/
/*FUNDEF*/

void hook_binmode(pExecuteObject pEo,
                  FILE *fp);
/*FEDNUF*/
/*FUNDEF*/

void hook_textmode(pExecuteObject pEo,
                   FILE *fp);
/*FEDNUF*/
/*FUNDEF*/

int hook_fwrite(pExecuteObject pEo,
               char *buf,
               int size,
               int count,
               FILE *fp);
/*FEDNUF*/
/*FUNDEF*/

int hook_fputc(pExecuteObject pEo,
               int c,
               FILE *fp);
/*FEDNUF*/
/*FUNDEF*/

int hook_flock(pExecuteObject pEo,
               FILE *fp,
               int iLockType);
/*FEDNUF*/
/*FUNDEF*/

int hook_lock(pExecuteObject pEo,
              FILE *fp,
              int iLockType,
              long lStart,
              long lLength);
/*FEDNUF*/
/*FUNDEF*/

int hook_feof(pExecuteObject pEo,
              FILE *fp);
/*FEDNUF*/
/*FUNDEF*/

int hook_mkdir(pExecuteObject pEo,
               char *pszDirectoryName);
/*FEDNUF*/
/*FUNDEF*/

int hook_rmdir(pExecuteObject pEo,
               char *pszDirectoryName);
/*FEDNUF*/
/*FUNDEF*/

int hook_remove(pExecuteObject pEo,
                char *pszFileName);
/*FEDNUF*/
/*FUNDEF*/

int hook_deltree(pExecuteObject pEo,
                 char *pszDirectoryName);
/*FEDNUF*/
/*FUNDEF*/

int hook_MakeDirectory(pExecuteObject pEo,
                       char *pszDirectoryName);
/*FEDNUF*/
/*FUNDEF*/

DIR *hook_opendir(pExecuteObject pEo,
                  char *pszDirectoryName,
                  tDIR *pDirectory);
/*FEDNUF*/
/*FUNDEF*/

struct dirent *hook_readdir(pExecuteObject pEo,
                            DIR *pDirectory);
/*FEDNUF*/
/*FUNDEF*/

void hook_closedir(pExecuteObject pEo,
                   DIR *pDirectory);
/*FEDNUF*/
/*FUNDEF*/

void hook_sleep(pExecuteObject pEo,
                long lSeconds);
/*FEDNUF*/
/*FUNDEF*/

int hook_curdir(pExecuteObject pEo,
                char *Buffer,
                unsigned long cbBuffer);
/*FEDNUF*/
/*FUNDEF*/

int hook_chdir(pExecuteObject pEo,
               char *Buffer);
/*FEDNUF*/
/*FUNDEF*/

int hook_chown(pExecuteObject pEo,
               char *pszFileName,
               char *pszOwner);
/*FEDNUF*/
/*FUNDEF*/

int hook_SetCreateTime(pExecuteObject pEo,
                       char *pszFileName,
                       long lTime);
/*FEDNUF*/
/*FUNDEF*/

int hook_SetModifyTime(pExecuteObject pEo,
                       char *pszFileName,
                       long lTime);
/*FEDNUF*/
/*FUNDEF*/

int hook_SetAccessTime(pExecuteObject pEo,
                       char *pszFileName,
                       long lTime);
/*FEDNUF*/
/*FUNDEF*/

int hook_gethostname(pExecuteObject pEo,
                     char *pszBuffer,
                     long cbBuffer);
/*FEDNUF*/
/*FUNDEF*/

int hook_gethost(pExecuteObject pEo,
                 char *pszBuffer,
                 struct hostent *pHost);
/*FEDNUF*/
/*FUNDEF*/

int hook_tcpconnect(pExecuteObject pEo,
                    SOCKET *sClient,
                    char *pszRemoteSocket);
/*FEDNUF*/
/*FUNDEF*/

int hook_tcpsend(pExecuteObject pEo,
                 SOCKET sClient,
                 char *pszBuffer,
                 long cbBuffer,
                 int iFlags);
/*FEDNUF*/
/*FUNDEF*/

int hook_tcprecv(pExecuteObject pEo,
                 SOCKET sClient,
                 char *pszBuffer,
                 long cbBuffer,
                 int iFlags);
/*FEDNUF*/
/*FUNDEF*/

int hook_tcpclose(pExecuteObject pEo,
                  SOCKET sClient);
/*FEDNUF*/
/*FUNDEF*/

int hook_killproc(pExecuteObject pEo,
                  long pid);
/*FEDNUF*/
/*FUNDEF*/

int hook_getowner(pExecuteObject pEo,
                  char *pszFileName,
                  char *pszOwnerBuffer,
                  long cbOwnerBuffer);
/*FEDNUF*/
/*FUNDEF*/

char *hook_fcrypt(pExecuteObject pEo,
                  char *buf,
                  char *salt,
                  char *buff);
/*FEDNUF*/
/*FUNDEF*/

long hook_CreateProcess(pExecuteObject pEo,
                         char *pszCommandLine);
/*FEDNUF*/
/*FUNDEF*/

long hook_CreateProcessEx(pExecuteObject pEo,
                          char *pszCommandLine,
                          long lTimeOut,
                          unsigned long *plPid,
                          unsigned long *plExitCode);
/*FEDNUF*/
/*FUNDEF*/

int hook_waitpid(pExecuteObject pEo,
                 long pid,
                 unsigned long *plExitCode);
/*FEDNUF*/
/*FUNDEF*/

int hook_CallScribaFunction(pExecuteObject pEo,
                            unsigned long lStartNode,
                            pFixSizeMemoryObject *pArgument,
                            unsigned long NumberOfPassedArguments,
                            pFixSizeMemoryObject *pFunctionResult);
/*FEDNUF*/
#ifdef __cplusplus
}
#endif
#endif
