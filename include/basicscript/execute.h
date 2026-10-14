/*
execute.h
*/
#ifndef __EXECUTE_H__
#define __EXECUTE_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif

#include "conftree.h"
#include "hookers.h"
#include "thread.h"



typedef struct _Module {
  char *pszModuleName; 
                       
  void *ModulePointer; 
  void *ModuleInternalParameters; 
                      
  int ModuleIsActive; 
  int ModuleIsStatic; 
  struct _Module *next; 
  }Module, *pModule;



typedef struct _SupportTable *pSupportTable;
#define PSUPPORTTABLE 1

typedef struct _ExecuteObject {
  void *(*memory_allocating_function)(size_t);
  void (*memory_releasing_function)(void *);
  void *pMemorySegment; 

  pReportFunction report;
  void *reportptr; 
  unsigned long fErrorFlags;

  ptConfigTree pConfig; 

  char *StringTable; 

  unsigned long cbStringTable; 

  pcNODE CommandArray;
  unsigned long StartNode;
  unsigned long CommandArraySize;

  long cGlobalVariables;
  pFixSizeMemoryObject GlobalVariables;

  long cLocalVariables;
  pFixSizeMemoryObject LocalVariables; 

  unsigned long ProgramCounter;
  unsigned long NextProgramCounter;
  int fNextPC; 

#define fStopRETURN 1
#define fStopSTOP   2
  int fStop;

  unsigned long lStepCounter;  
  unsigned long lGlobalStepCounter; 
  long lFunctionLevel;        

  
  long GlobalStepLimit;       
  long LocalStepLimit;        
  long FunctionLevelLimit;    

  int fWeAreCallingFuction; 

  unsigned long ErrorCode;
  int fErrorGoto; 
#define ONERROR_NOTHING    0
#define ONERROR_GOTO       1
#define ONERROR_RESUME     2
#define ONERROR_RESUMENEXT 3

  unsigned long ErrorGoto;   
  unsigned long ErrorResume; 
  unsigned long LastError;   

  unsigned long OperatorNode;     
  pFixSizeMemoryObject pOpResult; 
  pFixSizeMemoryObject pFunctionResult; 
  pMortalList pGlobalMortalList;   
  unsigned long FunctionArgumentsNode; 

  pMemoryObject pMo;

  CommandFunctionType *pCommandFunction;

  void *CommandParameter[NUM_CMD]; 
  void (*(Finaliser[NUM_CMD]))(struct _ExecuteObject*);
                                   
                                   
                                   
  void **InstructionParameter;     

  VersionInfo Ver;

  void *fpStdinFunction;  
  void *fpStdouFunction;  
  void *fpEnvirFunction;  
  char *CmdLineArgument;  

  SymbolTable OptionsTable; 

  void *pEmbedder; 

  pSupportTable pST; 
  pSupportTable pSTI;
                     
  MUTEX mxModules;   
  pModule modules;   
  struct _ExecuteObject *pEPo;

  char *pszModuleError; 

  pHookFunctions pHookers; 


  LexNASymbol *pCSYMBOLS; 
  int fThreadedCommandTable; 
  char **CSymbolList;
  
  }ExecuteObject
#ifndef PEXECUTEOBJECT
  , *pExecuteObject
#endif
  ;
#define GETDOUBLEVALUE(x) execute_GetDoubleValue(pEo,(x))
#define GETLONGVALUE(x)   execute_GetLongValue(pEo,(x))
/*FUNDEF*/

long execute_GetCommandByName(pExecuteObject pEo,
                              char *pszCommandName,
                              long lCodeHint);
/*FEDNUF*/
/*FUNDEF*/

int execute_CopyCommandTable(pExecuteObject pEo);
/*FEDNUF*/
/*FUNDEF*/

int execute_InitStructure(pExecuteObject pEo,
                          pBuildObject pBo);
/*FEDNUF*/
/*FUNDEF*/

int execute_ReInitStructure(pExecuteObject pEo,
                            pBuildObject pBo);
/*FEDNUF*/
/*FUNDEF*/

void execute_Execute_r(pExecuteObject pEo,
                       int *piErrorCode);
/*FEDNUF*/
/*FUNDEF*/

void execute_InitExecute(pExecuteObject pEo,
                        int *piErrorCode);
/*FEDNUF*/
/*FUNDEF*/

void execute_FinishExecute(pExecuteObject pEo,
                           int *piErrorCode);
/*FEDNUF*/
/*FUNDEF*/

void execute_Execute(pExecuteObject pEo,
                     int *piErrorCode);
/*FEDNUF*/
/*FUNDEF*/

void execute_ExecuteFunction(pExecuteObject pEo,
                             unsigned long StartNode,
                             long cArgs,
                             pFixSizeMemoryObject *pArgs,
                             pFixSizeMemoryObject *pResult,
                             int *piErrorCode);
/*FEDNUF*/
/*FUNDEF*/

pFixSizeMemoryObject execute_Evaluate(pExecuteObject pEo,
                                      unsigned long lExpressionRootNode,
                                      pMortalList pMyMortal,
                                      int *piErrorCode,
                                      int iArrayAccepted);
/*FEDNUF*/
/*FUNDEF*/

pFixSizeMemoryObject *execute_LeftValue(pExecuteObject pEo,
                                        unsigned long lExpressionRootNode,
                                        pMortalList pMyMortal,
                                        int *piErrorCode,
                                        int iArrayAccepted);
/*FEDNUF*/
/*FUNDEF*/

pFixSizeMemoryObject execute_EvaluateArray(pExecuteObject pEo,
                                      unsigned long lExpressionRootNode,
                                      pMortalList pMyMortal,
                                      int *piErrorCode);
/*FEDNUF*/
/*FUNDEF*/

pFixSizeMemoryObject execute_EvaluateSarray(pExecuteObject pEo,
                                      unsigned long lExpressionRootNode,
                                      pMortalList pMyMortal,
                                      int *piErrorCode);
/*FEDNUF*/
/*FUNDEF*/

pFixSizeMemoryObject *execute_LeftValueArray(pExecuteObject pEo,
                                             unsigned long lExpressionRootNode,
                                             pMortalList pMyMortal,
                                             int *piErrorCode);
/*FEDNUF*/
/*FUNDEF*/

pFixSizeMemoryObject *execute_LeftValueSarray(pExecuteObject pEo,
                                              unsigned long lExpressionRootNode,
                                              pMortalList pMyMortal,
                                              int *piErrorCode);
/*FEDNUF*/
/*FUNDEF*/

pFixSizeMemoryObject execute_Convert2String(pExecuteObject pEo,
                                          pFixSizeMemoryObject pVar,
                                          pMortalList pMyMortal);
/*FEDNUF*/
/*FUNDEF*/

pFixSizeMemoryObject execute_Convert2Long(pExecuteObject pEo,
                                          pFixSizeMemoryObject pVar,
                                          pMortalList pMyMortal);
/*FEDNUF*/
/*FUNDEF*/

pFixSizeMemoryObject execute_Convert2LongS(pExecuteObject pEo,
                                           pFixSizeMemoryObject pVar,
                                           pMortalList pMyMortal);
/*FEDNUF*/
/*FUNDEF*/

pFixSizeMemoryObject execute_Convert2Double(pExecuteObject pEo,
                                            pFixSizeMemoryObject pVar,
                                            pMortalList pMyMortal);
/*FEDNUF*/
/*FUNDEF*/

pFixSizeMemoryObject execute_Convert2DoubleS(pExecuteObject pEo,
                                             pFixSizeMemoryObject pVar,
                                             pMortalList pMyMortal);
/*FEDNUF*/
/*FUNDEF*/

pFixSizeMemoryObject execute_Convert2Numeric(pExecuteObject pEo,
                                             pFixSizeMemoryObject pVar,
                                             pMortalList pMyMortal);
/*FEDNUF*/
/*FUNDEF*/

pFixSizeMemoryObject execute_Dereference(pExecuteObject pEo,
                                         pFixSizeMemoryObject p,
                                         int *piErrorCode);
/*FEDNUF*/
/*FUNDEF*/

int execute_DereferenceS(unsigned long refcount,
                         pFixSizeMemoryObject *p);
/*FEDNUF*/
/*FUNDEF*/

double execute_GetDoubleValue(pExecuteObject pEo,
                              pFixSizeMemoryObject pVar);
/*FEDNUF*/
/*FUNDEF*/

long execute_GetLongValue(pExecuteObject pEo,
                          pFixSizeMemoryObject pVar);
/*FEDNUF*/
/*FUNDEF*/

int execute_IsStringInteger(pFixSizeMemoryObject pVar);
/*FEDNUF*/
/*FUNDEF*/

int execute_IsInteger(pFixSizeMemoryObject pVar);
/*FEDNUF*/
#ifdef __cplusplus
}
#endif
#endif
