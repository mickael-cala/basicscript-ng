/*
rpool.h
*/
#ifndef __RPOOL_H__
#define __RPOOL_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif

#include "../basext.h"
#include "../thread.h"

typedef struct _rpm_resource_t {
  void *handle; 
  unsigned long lBurn; 
  unsigned long lCreateTime; 
  unsigned long lUseCounter; 
  struct _rpm_resource_t *flink,*blink; 
  struct _rpm_pool_t *pool; 
  } rpm_resource_t;

typedef struct _rpm_pool_t {
  pSupportTable pSt;
  unsigned long lMaxBurn; 
  unsigned long lMaxTime; 
  unsigned long lMaxUse;  

  void *pool;             

  void *(*fpOpen)(void *);         
  void (*fpClose)(void *, void *); 

  void *pMemorySegment; 

  unsigned long (*timefun)(void *);

  unsigned long nFree;    
  unsigned long nUsed;    
  unsigned long nOpening; 

  unsigned long lMinFree; 
  unsigned long lMaxFree;  

  unsigned long lWaitSleep; 

  rpm_resource_t *first_resource;  
  rpm_resource_t *first_used;      
  MUTEX mxPool;                    
  MUTEX mxRun;                     

  } rpm_pool_t;

/*FUNDEF*/

void *rpm_NewPool(
  pSupportTable pSt,
  unsigned long lMaxBurn,
  unsigned long lMaxTime,
  unsigned long lMaxUse,

  unsigned long lMinFree,
  unsigned long lMaxFree,

  unsigned long lWaitSleep,

  void *pool,

  void *(*fpOpen)(void *),
  void (*fpClose)(void *, void *),

  void *(*myalloc)(size_t),
  void (*myfree)(void *),
  unsigned long (*timefun)(void *));
/*FEDNUF*/
/*FUNDEF*/

void *rpm_GetResource(
  rpm_pool_t *RPool,
  unsigned long lMaxWait);
/*FEDNUF*/
/*FUNDEF*/

void rpm_PutResource(
  rpm_pool_t *RPool,
  void *p);
/*FEDNUF*/
#ifdef __cplusplus
}
#endif
#endif
