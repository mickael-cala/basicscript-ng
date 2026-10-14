/*
expression.h
*/
#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif

#define MAX_LEXES_PER_LINE 14 
                              
#define MAX_GO_CONSTANTS 3    
                              
                              
#define MAX_SAME_LABELS 10    

typedef struct _SymbolUF { 
  unsigned long FunId; 
  long Argc; 
  unsigned long node; 
  char *FunctionName; 
  struct _SymbolUF *next;
  } SymbolUF, *pSymbolUF;


typedef struct _eNODE {
  long OpCode; 
  unsigned long NodeId; 
  char *szFileName;
  long lLineNumber;
  union {

    
    struct {
      union {
        struct _SymbolLABEL *pLabel;
        struct _eNODE *pNode;
        struct _eNODE_l *pNodeList;
        long lLongValue;
        double dDoubleValue;
        char *szStringValue;
        }Argument;
      long sLen;
      struct _eNODE *next;
      }CommandArgument;

    
    struct {
      struct _eNODE_l *Argument;
      }Arguments;

    
    struct {
      union {
        double dValue;        
        long   lValue;        
        char  *sValue;
        }Value;
      long sLen; 
      }Constant;

    
    struct {
      unsigned long Serial; 
      }Variable;

    
    struct {
      pSymbolUF pFunction; 
      struct _eNODE_l *Argument;
      }UserFunction;

    }Parameter;

  } eNODE,*peNODE;



enum {
  eNTYPE_ARR=1, 
  eNTYPE_SAR,   
  eNTYPE_FUN,   
  eNTYPE_LVR,   
  eNTYPE_GVR,   
  eNTYPE_DBL,   
  eNTYPE_LNG,   
  eNTYPE_STR,   
  eNTYPE_LST,   
  eNTYPE_CRG,   

  __eNTYPE_DUMMY__
  };


typedef struct _eNODE_l{
  unsigned long NodeId; 
  char *szFileName;
  long lLineNumber;
  peNODE actualm;
  struct _eNODE_l *rest;
  } eNODE_l, *peNODE_l;

typedef struct _SymbolLABEL { 
  long Serial; 
  unsigned long node; 
  } SymbolLABEL, *pSymbolLABEL;

typedef struct _SymbolVAR { 
  long Serial; 
  } SymbolVAR, *pSymbolVAR;

typedef struct _LabelStack {
  pSymbolLABEL pLabel;
  struct _LabelStack *Flink;
  long Type;
  } LabelStack, *pLabelStack;

typedef struct _BFun {   
  long OpCode;  
  long MinArgs; 
  long MaxArgs; 
  } BFun, *pBFun;

typedef struct _PredLConst {
  char *name;
  long value;
  }PredLConst,*pPredLConst;


enum {
  EX_LEX_EXP = 1,  
  EX_LEX_EXPL,     
  EX_LEX_LVAL,     
  EX_LEX_LVALL,    
  EX_LEX_NSYMBOL,  
  EX_LEX_SYMBOL,   
  EX_LEX_ASYMBOL,  
  EX_LEX_PRAGMA,   
  EX_LEX_SET_NAME_SPACE, 
  EX_LEX_RESET_NAME_SPACE, 
  EX_LEX_CHARACTER,
  EX_LEX_LONG,     
  EX_LEX_DOUBLE,   
  EX_LEX_STRING,   
  EX_LEX_LOCAL,    
  EX_LEX_LOCALL,   
  EX_LEX_GLOBAL,   
  EX_LEX_GLOBALL,  
  EX_LEX_FUNCTION, 
  EX_LEX_THIS_FUNCTION, 
  EX_LEX_LABEL_DEFINITION, 
  EX_LEX_LABEL,    
  EX_LEX_STAR,     
  EX_LEX_NOEXEC,   
  EX_LEX_ARG_NUM,  

  EX_LEX_GO_FORWARD,
  EX_LEX_GO_BACK,
  EX_LEX_COME_FORWARD,
  EX_LEX_COME_BACK,







  EX_LEX_LOCAL_START, 
  EX_LEX_LOCAL_END,   

  EX_LEX_CONST_NAME,  
  EX_LEX_GCONST_NAME, 
  EX_LEX_CONST_VALUE, 

  EX_LEX_DUMMY
  };

typedef struct _LineSyntaxUnit {
  int type;          
  long OpCode;       
  long GoConstant[MAX_GO_CONSTANTS];
  } LineSyntaxUnit, *pLineSyntaxUnit;

typedef struct _LineSyntax {
  long CommandOpCode;
  peNODE (*pfAnalyzeFunction)();
  LineSyntaxUnit lexes[MAX_LEXES_PER_LINE];
  } LineSyntax, *pLineSyntax;

typedef struct _NameSpaceStack {
  struct _NameSpaceStack *next;
  char *ThisNameSpace;
  }NameSpaceStack, *pNameSpaceStack;

typedef struct _eXobject {
  void *(*memory_allocating_function)(size_t);
  void (*memory_releasing_function)(void *);
  void *pMemorySegment; 
                        
  void *pLocalVarMemorySegment; 
                                
  void *pSymbolTableMemorySegment; 

  pLexObject pLex;    

  SymbolTable GlobalVariables;
  SymbolTable UserFunctions;
  SymbolTable LocalVariables;
  SymbolTable LocallyDeclaredGlobalVariables;
  SymbolTable GlobalLabels;  
  SymbolTable GlobalConstants; 
  long *plNrLocalVariables; 
                            
                            
                            
                            
  pPredLConst PredeclaredLongConstants;
  long cGlobalVariables;
  long cGlobalLabels;
  long cLocalVariables;
  long cUserFunctions;

  pSymbolUF FirstUF; 

  int iWeAreLocal; 
  int iDeclareVars; 
  int iDefaultLocal; 
















  char *CurrentNameSpace;
  long cbCurrentNameSpace;
  pNameSpaceStack pOldNameSpace;

  unsigned long *Unaries; 
  
  
  
  unsigned long *Binaries;
  unsigned long MAXPREC; 

  pBFun BuiltInFunctions;

  pReportFunction report;
  void *reportptr; 
  int iErrorCounter;
  unsigned long fErrorFlags;

  char *Buffer;
  size_t cbBuffer;

  pLineSyntax Command;

  peNODE_l pCommandList;

  unsigned long NodeCounter; 

  pSymbolUF ThisFunction; 

  
  pLabelStack pComeAndGoStack;
  pLabelStack pFreeComeAndGoStack; 
                                   
  pSymbolLABEL LabelsWaiting[MAX_SAME_LABELS];
  unsigned long cLabelsWaiting;
  pSymbolUF pFunctionWaiting;

  unsigned long cbStringTable; 
  struct _PreprocObject *pPREP;
  } eXobject, *peXobject;

typedef void (*CommandFunctionType)();

/*FUNDEF*/

void ex_DumpVariables(SymbolTable q,
                      FILE *fp);
/*FEDNUF*/
/*FUNDEF*/

int expression_PushNameSpace(peXobject pEx);
/*FEDNUF*/
/*FUNDEF*/

void ex_CheckUndefinedLabels(peXobject pEx);
/*FEDNUF*/
/*FUNDEF*/

void ex_CleanNameSpaceStack(peXobject pEx);
/*FEDNUF*/
/*FUNDEF*/

int expression_PopNameSpace(peXobject pEx);
/*FEDNUF*/
/*FUNDEF*/

int ex_PushWaitingLabel(peXobject pEx,
                         pSymbolLABEL pLbl);
/*FEDNUF*/
/*FUNDEF*/

pSymbolLABEL ex_PopWaitingLabel(peXobject pEx);
/*FEDNUF*/
/*FUNDEF*/

int _ex_PushLabel(peXobject pEx,
                  pSymbolLABEL pLbl,
                  long Type,
                  void *pMemorySegment);
/*FEDNUF*/
/*FUNDEF*/

pSymbolLABEL _ex_PopLabel(peXobject pEx,
                          long *pAcceptedType);
/*FEDNUF*/
/*FUNDEF*/

void _ex_CleanLabelStack(peXobject pEx);
/*FEDNUF*/
/*FUNDEF*/

pSymbolLABEL _new_SymbolLABEL(peXobject pEx);
/*FEDNUF*/
/*FUNDEF*/

pSymbolVAR _new_SymbolVAR(peXobject pEx,
                          int iLocal);
/*FEDNUF*/
/*FUNDEF*/

pSymbolUF _new_SymbolUF(peXobject pEx);
/*FEDNUF*/
/*FUNDEF*/

peNODE _new_eNODE(peXobject pEx);
/*FEDNUF*/
/*FUNDEF*/

peNODE_l _new_eNODE_l(peXobject pEx,
                      char *pszFileName,
                      long lLineNumber);
/*FEDNUF*/
/*FUNDEF*/

void ex_free(peXobject pEx);
/*FEDNUF*/
/*FUNDEF*/

int ex_init(peXobject pEx);
/*FEDNUF*/
/*FUNDEF*/

void ex_CleanNamePath(char *s);
/*FEDNUF*/
/*FUNDEF*/

int ex_ConvertName(char *s,          /* name to convert            */
                   char *Buffer,     /* buffer to store the result */
                   size_t cbBuffer,  /* size of the buffer         */
                   peXobject pEx     /* current expression object  */);
/*FEDNUF*/
/*FUNDEF*/

pBFun ex_IsBFun(peXobject pEx);
/*FEDNUF*/
/*FUNDEF*/

unsigned long ex_IsUnop(peXobject pEx);
/*FEDNUF*/
/*FUNDEF*/

unsigned long ex_IsBinop(peXobject pEx,
               unsigned long precedence);
/*FEDNUF*/
/*FUNDEF*/

peNODE_l ex_LeftValueList(peXobject pEx);
/*FEDNUF*/
/*FUNDEF*/

peNODE_l ex_ExpressionList(peXobject pEx);
/*FEDNUF*/
/*FUNDEF*/

int ex_Local(peXobject pEx);
/*FEDNUF*/
/*FUNDEF*/

int ex_LocalList(peXobject pEx);
/*FEDNUF*/
/*FUNDEF*/

int ex_Global(peXobject pEx);
/*FEDNUF*/
/*FUNDEF*/

int ex_GlobalList(peXobject pEx);
/*FEDNUF*/
/*FUNDEF*/

void **ex_LookupUserFunction(peXobject pEx,
                             int iInsert);
/*FEDNUF*/
/*FUNDEF*/

void **ex_LookupGlobalVariable(peXobject pEx,
                               int iInsert);
/*FEDNUF*/
/*FUNDEF*/

void **ex_LookupLocallyDeclaredGlobalVariable(peXobject pEx);
/*FEDNUF*/
/*FUNDEF*/

void **ex_LookupLocalVariable(peXobject pEx,
                              int iInsert);
/*FEDNUF*/
/*FUNDEF*/

peNODE ex_Tag(peXobject pEx);
/*FEDNUF*/
/*FUNDEF*/

peNODE ex_Expression_i(peXobject pEx,
                       int i);
/*FEDNUF*/
/*FUNDEF*/

void ex_Expression_r(peXobject pEx,
                     peNODE *Result);
/*FEDNUF*/
/*FUNDEF*/

int ex_IsSymbolValidLval(peXobject pEx);
/*FEDNUF*/
/*FUNDEF*/

peNODE ex_LeftValue(peXobject pEx);
/*FEDNUF*/
/*FUNDEF*/

int ex_PredeclareGlobalLongConst(peXobject pEx,
                                 char *pszConstName,
                                 long lConstValue);
/*FEDNUF*/
/*FUNDEF*/

peNODE ex_IsCommandThis(peXobject pEx,
                        pLineSyntax p,
                        int *piFailure);
/*FEDNUF*/
/*FUNDEF*/

void ex_Command_r(peXobject pEx,
                  peNODE *Result,
                  int *piFailure);
/*FEDNUF*/
/*FUNDEF*/

int ex_Command_l(peXobject pEx,
                  peNODE_l *Result);
/*FEDNUF*/
/*FUNDEF*/

void ex_pprint(FILE *f,
               peXobject pEx);
/*FEDNUF*/
/*FUNDEF*/

int ex_Pragma(peXobject pEx,
              char *pszPragma);
/*FEDNUF*/
/*FUNDEF*/

peNODE ex_IsCommandCALL(peXobject pEx,
                        pLineSyntax p,
                        int *piFailure);
/*FEDNUF*/
/*FUNDEF*/

peNODE ex_IsCommandOPEN(peXobject pEx,
                        pLineSyntax p,
                        int *piFailure);
/*FEDNUF*/
/*FUNDEF*/

peNODE ex_IsCommandSLIF(peXobject pEx,
                        pLineSyntax p,
                        int *piFailure);
/*FEDNUF*/
/*FUNDEF*/

peNODE ex_IsCommandIF(peXobject pEx,
                        pLineSyntax p,
                        int *piFailure);
/*FEDNUF*/
/*FUNDEF*/

peNODE ex_IsCommandLET(peXobject pEx,
                       pLineSyntax p,
                       int *piFailure);
/*FEDNUF*/
#ifdef __cplusplus
}
#endif
#endif
