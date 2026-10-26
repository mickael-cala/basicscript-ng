/*
command.h
*/
#ifndef __COMMAND_H__
#define __COMMAND_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif
#include "errcodes.h"
#include "report.h"
#include "sym.h"
#include "lexer.h"
#include "expression.h"
#include "builder.h"
#include "memory.h"
#include "syntax.h"
#include "execute.h"
#include "syntax.h"
#include "myalloc.h"
#include "filesys.h"
#include "options.h"
#include "hookers.h"

#include "notimp.h"

#define COMMAND(x) void COMMAND_##x(pExecuteObject pEo){\
                     MortalList _ThisCommandMortals=NULL;\
                     pMortalList _pThisCommandMortals = &_ThisCommandMortals;\
                     unsigned long _ActualNode=PROGRAMCOUNTER;\
                     int iErrorCode;

#define IDENTICAL_COMMAND(x) COMMAND_##x(pEo);

#define USE_CALLER_MORTALS (_pThisCommandMortals = pEo->pGlobalMortalList)

#define END goto _FunctionFinishLabel;\
            _FunctionFinishLabel: \
            memory_ReleaseMortals(pEo->pMo,&_ThisCommandMortals);\
            iErrorCode = 0;\
            FINISH;\
            }

#define FINISH

#define RETURN goto _FunctionFinishLabel
#ifdef ERROR
#undef ERROR
#endif
#define ERROR(x) do{ pEo->ErrorCode = x; RETURN; }while(0)

#define NOTIMPLEMENTED ERROR(COMMAND_ERROR_NOTIMP)

#define DEREFERENCE(X)   refcount = pEo->pMo->maxderef;\
  while( *(X) && TYPE( *(X) ) == VTYPE_REF ){\
    (X) = (*(X))->Value.aValue;\
    if( ! refcount -- )ERROR(COMMAND_ERROR_CIRCULAR);\
    }


#define ERRORUNDEF if( (*RaiseError(pEo))&1 ){\
                     ERROR(COMMAND_ERROR_DIV);\
                     }\
                   RESULT = NULL; RETURN;


#define NONULOP(x) ASSERTOKE;if( memory_IsUndef(x) ){if((*RaiseError(pEo))&2 ){\
                     ERROR(COMMAND_ERROR_UNDEFOP);\
                     }\
                   RESULT = NULL; RETURN;}


#define NONULOPE(x) ASSERTOKE;if( memory_IsUndef(x) ){if( (*RaiseError(pEo))&4 )ERROR(COMMAND_ERROR_UNDEFOP);}


#define RETURN_DOUBLE_VALUE_OR_LONG(x) \
                   dResult = (x);\
                   if( dResult == floor(dResult) && fabs(dResult) < LONG_MAX ){\
                     RESULT= NEWMORTALLONG;\
                     ASSERTNULL(RESULT);\
                     LONGVALUE(RESULT) = ((long)dResult);\
                     RETURN;\
                     }\
                   RESULT = NEWMORTALDOUBLE;\
                   ASSERTNULL(RESULT);\
                   DOUBLEVALUE(RESULT) = dResult;\
                   RETURN;

#define RETURN_DOUBLE_VALUE(x) \
                   RESULT = NEWMORTALDOUBLE;\
                   ASSERTNULL(RESULT);\
                   DOUBLEVALUE(RESULT) = (x);\
                   RETURN;


#define RETURN_LONG_VALUE(x) \
                   RESULT= NEWMORTALLONG;\
                   ASSERTNULL(RESULT);\
                   LONGVALUE(RESULT) = (x);\
                   RETURN;

#define RETURN_UNDEF RESULT = NULL; RETURN

#define RETURN_TRUE \
                   RESULT= NEWMORTALLONG;\
                   ASSERTNULL(RESULT);\
                   LONGVALUE(RESULT) = -1L;\
                   RETURN;

#define RETURN_FALSE \
                   RESULT= NEWMORTALLONG;\
                   ASSERTNULL(RESULT);\
                   LONGVALUE(RESULT) = 0L;\
                   RETURN;

#define PROGRAMCOUNTER (pEo->CommandArray[pEo->ProgramCounter-1].Parameter.NodeList.actualm)
#define SETPROGRAMCOUNTER(x) ( pEo->fNextPC=1 , pEo->NextProgramCounter = (x) )

#define NEXTPARAMETER (_ActualNode = pEo->CommandArray[_ActualNode-1].Parameter.CommandArgument.next)
#define PARAMETERNODE (pEo->CommandArray[_ActualNode-1].Parameter.CommandArgument.Argument.pNode)
#define PARAMETERLONG (pEo->CommandArray[_ActualNode-1].Parameter.CommandArgument.Argument.lLongValue)
#define PARAMETERDOUBLE (pEo->CommandArray[_ActualNode-1].Parameter.CommandArgument.Argument.dDoubleValue)
#define PARAMETERSTRING (pEo->StringTable+pEo->CommandArray[_ActualNode-1].Parameter.CommandArgument.Argument.szStringValue)
#define PARAMETERSTRLEN (*((long *)(pEo->StringTable+pEo->CommandArray[_ActualNode-1].Parameter.CommandArgument.Argument.szStringValue-sizeof(long))))
#define PARAMETEREXEC   (pEo->InstructionParameter[_ActualNode-1])

#define OPCODE(x) (pEo->CommandArray[x-1].OpCode)
#define THISPARAMPTR (pEo->CommandParameter[pEo->CommandArray[_ActualNode-1].OpCode - START_CMD])
#define PARAMPTR(x)  (pEo->CommandParameter[(x) - START_CMD])
#define FINALPTR(x)  (pEo->Finaliser[(x) - START_CMD])
#define ALLOC(x) alloc_Alloc((x),pEo->pMemorySegment)
#define FREE(x)  alloc_Free((x),pEo->pMemorySegment)

#define PARAMETERLIST (pEo->CommandArray[pEo->OperatorNode-1].Parameter.Arguments.Argument)

#define NEWLONG   memory_NewLong(pEo->pMo)
#define NEWDOUBLE memory_NewDouble(pEo->pMo)
#define NEWSTRING(length) memory_NewString(pEo->pMo,length)
#define NEWARRAY(low,high) memory_NewArray(pEo->pMo,low,high)

#define NEWMORTALLONG   memory_NewMortalLong(pEo->pMo,_pThisCommandMortals)
#define NEWMORTALDOUBLE memory_NewMortalDouble(pEo->pMo,_pThisCommandMortals)
#define NEWMORTALSTRING(length) memory_NewMortalString(pEo->pMo,length,_pThisCommandMortals)
#define NEWMORTALARRAY(low,high) memory_NewArray(pEo->pMo,_pThisCommandMortals,low,high)

#define CONVERT2DOUBLE(x)  execute_Convert2Double(pEo,x,_pThisCommandMortals)
#define CONVERT2LONG(x)    execute_Convert2Long(pEo,x,_pThisCommandMortals)
#define CONVERT2STRING(x)  execute_Convert2String(pEo,x,_pThisCommandMortals)
#define CONVERT2NUMERIC(x) execute_Convert2Numeric(pEo,x,_pThisCommandMortals)

#define CONVERT2ZCHAR(x,y)   (y) = ALLOC(STRLEN( (x) )+1);\
                             if( (y) == NULL )ERROR(COMMAND_ERROR_MEMORY_LOW);\
                             memcpy((y),STRINGVALUE( (x) ),STRLEN( (x) ));\
                             (y)[STRLEN( (x) )] = (char)0;


#define ISSTRINGINTEGER(x) execute_IsStringInteger(x)
#define ISINTEGER(x) execute_IsInteger(x)

#define RESULT pEo->pOpResult

#define CAR(x) (((x)>0) ? pEo->CommandArray[(x)-1].Parameter.NodeList.actualm : 0)
#define CDR(x) (((x)>0) ? pEo->CommandArray[(x)-1].Parameter.NodeList.rest : 0)

#define EVALUATEEXPRESSION(x) memory_DupMortalize(pEo->pMo,\
                                            execute_Dereference(pEo,\
                                            execute_Evaluate(pEo,x,_pThisCommandMortals,&iErrorCode,0),&iErrorCode),\
                                            _pThisCommandMortals,\
                                            &iErrorCode)
#define _EVALUATEEXPRESSION(x) execute_Dereference(pEo,execute_Evaluate(pEo,x,_pThisCommandMortals,&iErrorCode,0),&iErrorCode)
#define _EVALUATEEXPRESSION_A(x) execute_Dereference(pEo,execute_Evaluate(pEo,x,_pThisCommandMortals,&iErrorCode,1),&iErrorCode)
#define EVALUATELEFTVALUE(x) execute_LeftValue(pEo,x,_pThisCommandMortals,&iErrorCode,0)
#define EVALUATELEFTVALUE_A(x) execute_LeftValue(pEo,x,_pThisCommandMortals,&iErrorCode,1)

#define ASSERTOKE if( iErrorCode )ERROR(iErrorCode); 
#define ASSERTNULL(X) if( (X) == NULL )ERROR(COMMAND_ERROR_MEMORY_LOW);

#define IMMORTALIZE(x) memory_Immortalize(x,_pThisCommandMortals)

typedef unsigned long NODE;
typedef pFixSizeMemoryObject VARIABLE, *LEFTVALUE;
#define TYPE(x) ((x)->vType)

#define OPTION(x) options_Get(pEo,(x))
#define OPTIONR(x) options_GetR(pEo,(x))
#ifdef __cplusplus
}
#endif
#endif
