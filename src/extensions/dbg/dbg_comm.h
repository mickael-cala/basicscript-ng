/*
dbg_comm.h
*/
#ifndef __DBG_COMM_H__
#define __DBG_COMM_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif
/*FUNDEF*/

void comm_Init(pDebuggerObject pDO);
/*FEDNUF*/
/*FUNDEF*/

void comm_WeAreAt(pDebuggerObject pDO,
                  long i);
/*FEDNUF*/
/*FUNDEF*/

void comm_List(pDebuggerObject pDO,
               long lStart,
               long lEnd,
               long lThis);
/*FEDNUF*/
/*FUNDEF*/

void GetRange(char *pszBuffer,
              long *plStart,
              long *plEnd);
/*FEDNUF*/
/*FUNDEF*/

void comm_Message(pDebuggerObject pDO,
                  char *pszMessage);
/*FEDNUF*/
/*FUNDEF*/

int comm_GetCommand(pDebuggerObject pDO,
                    char *pszBuffer,
                    long cbBuffer);
/*FEDNUF*/
#ifdef __cplusplus
}
#endif
#endif
