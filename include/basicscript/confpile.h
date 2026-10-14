/*
confpile.h
*/
#ifndef __CONFPILE_H__
#define __CONFPILE_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif

/*FUNDEF*/

int cft_ReadTextConfig(ptConfigTree pCT,
                       char *pszFileName);
/*FEDNUF*/
/*FUNDEF*/

static int DumpTree(ptConfigTree pCT,
                    FILE *fp,
                    CFT_NODE hNode,
                    int offset);
/*FEDNUF*/
/*FUNDEF*/

int cft_DumpConfig(ptConfigTree pCT,
                   char *pszFileName);
/*FEDNUF*/
/*FUNDEF*/

int cft_DumbConfig(ptConfigTree pCT,
                   char *pszFileName);
/*FEDNUF*/
#ifdef __cplusplus
}
#endif
#endif
