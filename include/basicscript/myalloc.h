/*
myalloc.h
*/
#ifndef __MYALLOC_H__
#define __MYALLOC_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif
/*FUNDEF*/

void *alloc_InitSegment(void * (*maf)(size_t), /* a 'malloc' and a 'free' like functions */
                        void   (*mrf)(void *));
/*FEDNUF*/
/*FUNDEF*/

void alloc_GlobalUseGlobalMutex();
/*FEDNUF*/
/*FUNDEF*/

long alloc_SegmentLimit(void *p,
                        unsigned long NewMaxSize);
/*FEDNUF*/
/*FUNDEF*/

void alloc_FreeSegment(void *p);
/*FEDNUF*/
/*FUNDEF*/

void alloc_FinishSegment(void *p);
/*FEDNUF*/
/*FUNDEF*/

void *alloc_Alloc(size_t n,
                  void *p);
/*FEDNUF*/
/*FUNDEF*/

void alloc_Free(void *pMem, void *p);
/*FEDNUF*/
/*FUNDEF*/

void alloc_Merge(void *p1, void *p2);
/*FEDNUF*/
/*FUNDEF*/

void alloc_MergeAndFinish(void *p1, void *p2);
/*FEDNUF*/
/*FUNDEF*/

void alloc_InitStat();
/*FEDNUF*/
/*FUNDEF*/

void alloc_GlobalGetStat(unsigned long *pNetMax,
                         unsigned long *pNetMin,
                         unsigned long *pBruMax,
                         unsigned long *pBruMin,
                         unsigned long *pNetSize,
                         unsigned long *pBruSize);
/*FEDNUF*/
/*FUNDEF*/

void alloc_GetStat(void *p,
                   unsigned long *pMax,
                   unsigned long *pMin,
                   unsigned long *pActSize);
/*FEDNUF*/
#ifdef __cplusplus
}
#endif
#endif
