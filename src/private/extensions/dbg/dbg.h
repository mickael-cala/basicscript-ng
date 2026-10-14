/*
dbg.h
*/
#ifndef __DBG_H__
#define __DBG_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif

typedef struct _UserFunction_t {
  long cLocalVariables;
  char *pszFunctionName;
  char **ppszLocalVariables;
  long NodeId; 
  } UserFunction_t, *pUserFunction_t;


typedef struct _DebugNode_t {
  char *pszFileName; 
  long lLineNumber;  
  long lNodeId;      
  long lSourceLine;  
                     
  } DebugNode_t, *pDebugNode_t;


typedef struct _SourceLine_t {
  char *line;
  long lLineNumber;
  char *szFileName;
  int BreakPoint;
  } SourceLine_t, *pSourceLine_t;


typedef struct _DebugCallStack_t {
  long Node;
  pUserFunction_t pUF;
  pFixSizeMemoryObject LocalVariables;
  struct _DebugCallStack_t *up,*down;
  } DebugCallStack_t, *pDebugCallStack_t;






typedef struct _DebuggerObject {
  pPrepext pEXT;
  pExecuteObject pEo;
  long cGlobalVariables;
  char **ppszGlobalVariables;
  long cUserFunctions;
  pUserFunction_t pUserFunctions;
  long cFileNames;
  char **ppszFileNames;
  long cNodes;
  pDebugNode_t Nodes;
  long cSourceLines;
  pSourceLine_t SourceLines;
  pDebugCallStack_t DbgStack;
  pDebugCallStack_t StackTop;
  pDebugCallStack_t StackListPointer;
  long CallStackDepth;
  long Run2CallStack;
  long Run2Line;
  int bLocalStart;
  long FunctionNode;
  long lPrevPC,lPC;
  } DebuggerObject, *pDebuggerObject;
/*FUNDEF*/

int SPrintVariable(pDebuggerObject pDO,
                   VARIABLE v,
                   char *pszBuffer,
                   unsigned long *cbBuffer);
/*FEDNUF*/
/*FUNDEF*/

int SPrintVarByName(pDebuggerObject pDO,
                    pExecuteObject pEo,
                    char *pszName,
                    char *pszBuffer,
                    unsigned long *cbBuffer);
/*FEDNUF*/
/*FUNDEF*/

long GetSourceLineNumber(pDebuggerObject pDO,
                         long PC);
/*FEDNUF*/
/*FUNDEF*/

long GetCurrentDebugLine(pDebuggerObject pDO);
/*FEDNUF*/
#ifdef __cplusplus
}
#endif
#endif
