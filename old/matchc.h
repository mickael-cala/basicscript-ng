/*
matchc.h
*/
#ifndef __MATCHC_H__
#define __MATCHC_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif
typedef struct _PatternParam {
  unsigned long cArraySize;
  unsigned long cAArraySize;
  unsigned long *pcbParameterArray;
  char **ParameterArray;
  unsigned long cbBufferSize;
  char *pszBuffer;
  int iMatches;
  pMatchSets pThisMatchSets;
  } PatternParam, *pPatternParam;
int initialize_like(pExecuteObject pEo);
#ifdef __cplusplus
}
#endif
#endif
