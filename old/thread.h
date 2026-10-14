/*
thread.h
*/
#ifndef __THREAD_H__
#define __THREAD_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif

#ifdef WIN32
#include <windows.h>
#include <winbase.h>
#include <io.h>
#include <direct.h>
#else
#include <sys/file.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#endif

#ifdef WIN32
typedef HANDLE THREADHANDLE,*PTHREADHANDLE;
typedef HANDLE MUTEX,*PMUTEX;
#else
typedef pthread_t THREADHANDLE,*PTHREADHANDLE;
typedef pthread_mutex_t MUTEX,*PMUTEX;
#endif

typedef struct _SHAREDLOCK {
  MUTEX mxWrite;
  MUTEX mxRead;
  MUTEX mxCounter;
  int dwReaders; 
  } SHAREDLOCK, *PSHAREDLOCK;


/*FUNDEF*/

int thread_CreateThread(PTHREADHANDLE pThread,
                      void *pStartFunction,
                      void *pThreadParameter);
/*FEDNUF*/
/*FUNDEF*/

void thread_ExitThread();
/*FEDNUF*/
/*FUNDEF*/

void thread_InitMutex(PMUTEX pMutex);
/*FEDNUF*/
/*FUNDEF*/

void thread_FinishMutex(PMUTEX pMutex);
/*FEDNUF*/
/*FUNDEF*/

void thread_LockMutex(PMUTEX pMutex);
/*FEDNUF*/
/*FUNDEF*/

void thread_UnlockMutex(PMUTEX pMutex);
/*FEDNUF*/
/*FUNDEF*/

void shared_InitLock(PSHAREDLOCK p);
/*FEDNUF*/
/*FUNDEF*/

void shared_FinishLock(PSHAREDLOCK p);
/*FEDNUF*/
/*FUNDEF*/

void shared_LockRead(PSHAREDLOCK p);
/*FEDNUF*/
/*FUNDEF*/

void shared_LockWrite(PSHAREDLOCK p);
/*FEDNUF*/
/*FUNDEF*/

void shared_UnlockRead(PSHAREDLOCK p);
/*FEDNUF*/
/*FUNDEF*/

void shared_UnlockWrite(PSHAREDLOCK p);
/*FEDNUF*/
#ifdef __cplusplus
}
#endif
#endif
