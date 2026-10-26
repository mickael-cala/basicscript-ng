/*
modumana.h
*/
#ifndef __MODUMANA_H__
#define __MODUMANA_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif
/*FUNDEF*/

int modu_Init(pExecuteObject pEo,
              int iForce);
/*FEDNUF*/
/*FUNDEF*/

int modu_Preload(pExecuteObject pEo);
/*FEDNUF*/
/*FUNDEF*/

void *modu_GetModuleFunctionByName(
  pModule pThisModule,
  char *pszFunctionName);
/*FEDNUF*/
/*FUNDEF*/

void *modu_GetStaticFunctionByName(
  void *pLibrary,
  char *pszFunctionName);
/*FEDNUF*/
/*FUNDEF*/

int modu_LoadModule(pExecuteObject pEo,
                    char *pszLibraryFile,
                    pModule **pThisModule);
/*FEDNUF*/
/*FUNDEF*/

int modu_GetFunctionByName(pExecuteObject pEo,
                           char *pszLibraryFile,
                           char *pszFunctionName,
                           void **ppFunction,
                           pModule **pThisModule);
/*FEDNUF*/
/*FUNDEF*/

int modu_UnloadAllModules(pExecuteObject pEo);
/*FEDNUF*/
/*FUNDEF*/

int modu_UnloadModule(pExecuteObject pEo,
                      char *pszLibraryFile);
/*FEDNUF*/
/*FUNDEF*/

int modu_ShutdownModule(pExecuteObject pEo,
                        pModule pThisModule);
/*FEDNUF*/
#ifdef __cplusplus
}
#endif
#endif
