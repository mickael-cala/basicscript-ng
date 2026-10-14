/*
logger.h
*/
#ifndef __LOGGER_H__
#define __LOGGER_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif

typedef struct _tLogItem {
  char *pszMessage;
  long Time;
  struct _tLogItem *p,*n;
  } tLogItem, *ptLogItem;

#define FNMAX 256
typedef struct _tLogger {
  char *pszFileName;
  char szFileName[FNMAX];
  FILE *fp;
  long LastTime; 
  long TimeSpan;
  void *(*memory_allocating_function)(size_t, void *);
  void (*memory_releasing_function)(void *, void *);
  void *pMemorySegment;
  long MaxItemLen; 
  ptLogItem QueueStart,QueueEnd;
  MUTEX mxChain;
  MUTEX mxRun;
  MUTEX mxState;
  int type; 
  int state; 
             
  } tLogger,*ptLogger;

#define LOGSTATE_NORMAL      0
#define LOGSTATE_SHUTTING    1
#define LOGSTATE_DEAD        2
#define LOGSTATE_SYNCHRONOUS 3

#define LOGTYPE_SYNCHRONOUS 0
#define LOGTYPE_NORMAL      1
/*FUNDEF*/

int log_state(ptLogger pLOG);
/*FEDNUF*/
/*FUNDEF*/

int log_init(ptLogger pLOG,
             void *(*memory_allocating_function)(size_t, void *),
             void (*memory_releasing_function)(void *, void *),
             void *pMemorySegment,
             char *pszLogFileName,
             int iLogType);
/*FEDNUF*/
/*FUNDEF*/

int log_printf(ptLogger pLOG,
               char *pszFormat,
               ...);
/*FEDNUF*/
/*FUNDEF*/

int log_shutdown(ptLogger pLOG);
/*FEDNUF*/
#ifdef __cplusplus
}
#endif
#endif
