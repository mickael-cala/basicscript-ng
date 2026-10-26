/*
dynlolib.h
*/
#ifndef __DYNLOLIB_H__
#define __DYNLOLIB_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif

/*FUNDEF*/

void *dynlolib_LoadLibrary(
  char *pszLibraryFile);
/*FEDNUF*/
/*FUNDEF*/

void dynlolib_FreeLibrary(
  void *pLibrary);
/*FEDNUF*/
/*FUNDEF*/

void *dynlolib_GetFunctionByName(
  void *pLibrary,
  char *pszFunctionName);
/*FEDNUF*/
#ifdef __cplusplus
}
#endif
#endif
