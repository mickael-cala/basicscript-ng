/*
prepext.h
*/
#ifndef __PREPEXT_H__
#define __PREPEXT_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif


#define IP_INTERFACE_VERSION 1

typedef struct _Prepext {
  long lVersion;
  void *pPointer;
  void *pMemorySegment;
  struct _SupportTable *pST;
  } Prepext, *pPrepext;

enum PreprocessorCommands {


  PreprocessorLoad = 0,
  PreprocessorReadStart,
  PreprocessorReadDone0,
  PreprocessorReadDone1,
  PreprocessorReadDone2,
  PreprocessorReadDone3,

  PreprocessorLexInit,
  PreprocessorLexDone,
  PreprocessorLexNASymbol,
  PreprocessorLexASymbol,
  PreprocessorLexSymbol,
  PreprocessorLexString,
  PreprocessorLexMString,
  PreprocessorLexInteger,
  PreprocessorLexReal,
  PreprocessorLexCharacter,

  PreprocessorExStart,
  PreprocessorExStartLine,
  PreprocessorExEnd,
  PreprocessorExFinish,
  PreprocessorExStartLocal,
  PreprocessorExEndLocal,
  PreprocessorExLineNode,

  PreprocessorExeStart,
  PreprocessorExeFinish,
  PreprocessorExeNoRun,



  PreprocessorContinue, 
  PreprocessorDone, 
  PreprocessorUnload,


  _PreprocesorDummy_
  };

#ifdef __cplusplus
}
#endif
#endif
