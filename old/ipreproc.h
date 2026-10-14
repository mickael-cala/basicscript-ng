/*
ipreproc.h
*/
#ifndef __IPREPROC_H__
#define __IPREPROC_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif

#include "report.h"
#include "sym.h"
#include "lexer.h"
#include "expression.h"
#include "syntax.h"
#include "reader.h"
#include "myalloc.h"
#include "builder.h"
#include "memory.h"
#include "execute.h"
#include "prepext.h"

typedef struct _Preprocessor {
  void *pDllHandle;
  void *pFunction;
  char *pszPreprocessorName;
  struct _Preprocessor *next,*prev;
  Prepext pEXT; 
  } Preprocessor, *pPreprocessor;

typedef struct _PreprocObject {
  void *pMemorySegment;
  unsigned long n;
  pPreprocessor pFirst,pLast;
  ExecuteObject EXE; 
  struct _SbProgram *pSB;
  }PreprocObject,*pPreprocObject;

/*FUNDEF*/

void ipreproc_InitStructure(pPreprocObject pPre);
/*FEDNUF*/
/*FUNDEF*/

void ipreproc_PurgePreprocessorMemory(pPreprocObject pPre);
/*FEDNUF*/
/*FUNDEF*/

pPreprocessor ipreproc_InsertPreprocessor(pPreprocObject pPre);
/*FEDNUF*/
/*FUNDEF*/

void ipreproc_DeletePreprocessor(pPreprocObject pPre,
                                 pPreprocessor pOld);
/*FEDNUF*/
/*FUNDEF*/

int ipreproc_LoadInternalPreprocessor(pPreprocObject pPre,
                                      char *pszPreprocessorName);
/*FEDNUF*/
/*FUNDEF*/

int ipreproc_Process(pPreprocObject pPre,
                     long lCommand,
                     void *pPointer);
/*FEDNUF*/
#ifdef NO_IPREPROC
#define ipreproc_Process(X,Y,Z)
#endif

#ifdef __cplusplus
}
#endif
#endif
