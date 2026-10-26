/*
testalloc.h
*/
#ifndef __TESTALLOC_H__
#define __TESTALLOC_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif

typedef struct _tAllocUnit {
  long n;                       
  long id;                      
  struct _tAllocUnit *next;     
  struct _tAllocUnit *prev;     
  unsigned char memory[1];      
  } tAllocUnit, *ptAllocUnit;



typedef struct _TAlloc {
  void * (*memory_allocating_function)(size_t);
  void (*memory_releasing_function)(void *);

  ptAllocUnit FirstUnit;
  } TAlloc, *pTAlloc;

/*FUNDEF*/

void testa_InitSegment();
/*FEDNUF*/
/*FUNDEF*/

void testa_Assert0x80();
/*FEDNUF*/
/*FUNDEF*/

void *testa_Alloc(size_t n);
/*FEDNUF*/
/*FUNDEF*/

void testa_Free(void *pMem);
/*FEDNUF*/
/*FUNDEF*/

void testa_Check(void *pMem);
/*FEDNUF*/
/*FUNDEF*/

void testa_AssertLeak();
/*FEDNUF*/
/*FUNDEF*/

unsigned long testa_ReportLeak();
/*FEDNUF*/
#ifdef __cplusplus
}
#endif
#endif
