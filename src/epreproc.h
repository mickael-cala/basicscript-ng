/*
epreproc.h
*/
#ifndef __EPREPROC_H__
#define __EPREPROC_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif

/*FUNDEF*/

int epreproc(ptConfigTree pCONF,
             char *pszInputFileName,
             char **pszOutputFileName,
             char **ppszArgPreprocessor,
             void *(*thismalloc)(unsigned int),
             void (*thisfree)(void *));
/*FEDNUF*/
#ifdef __cplusplus
}
#endif
#endif
