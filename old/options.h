/*
options.h
*/
#ifndef __OPTIONS_H__
#define __OPTIONS_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif

/*FUNDEF*/

int options_Reset(pExecuteObject pEo,
                  char *name);
/*FEDNUF*/
/*FUNDEF*/

int options_Set(pExecuteObject pEo,
                char *name,
                long value);
/*FEDNUF*/
/*FUNDEF*/

long options_Get(pExecuteObject pEo,
                 char *name);
/*FEDNUF*/
/*FUNDEF*/

long *options_GetR(pExecuteObject pEo,
                 char *name);
/*FEDNUF*/
#ifdef __cplusplus
}
#endif
#endif
