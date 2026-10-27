/*
memory.h
*/
#ifndef __MEMORY_H__
#define __MEMORY_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif

typedef unsigned char BYTE, *PBYTE;

typedef struct _FixSizeMemoryObject {

  union _fsmoval{
    PBYTE  pValue; 
    long   lValue; 
    double dValue; 
    struct _FixSizeMemoryObject **aValue;
    } Value;
  unsigned long Size; 
  BYTE sType; 
              
  BYTE vType;
  BYTE State;
  BYTE fArrayFixed; /* phase 8.1 : pose par DIM, refuse les indices hors bornes */

  struct _FixSizeMemoryObject *next;
  union {
    struct _FixSizeMemoryObject *prev;   
    struct _FixSizeMemoryObject **rprev; 
    }link;
  long ArrayLowLimit, ArrayHighLimit;    
                                         
  } FixSizeMemoryObject, *pFixSizeMemoryObject, 
    *MortalList, **pMortalList;


#define DOUBLEVALUE(x) ((x)->Value.dValue)
#define LONGVALUE(x)   ((x)->Value.lValue)
#define STRINGVALUE(x) ((char *)((x)->Value.pValue))
#define ARRAYVALUE(x,i)  (((x)->Value.aValue)[(i)-(x)->ArrayLowLimit])
#define ARRAYLOW(x)    ((x)->ArrayLowLimit)
#define ARRAYHIGH(x)   ((x)->ArrayHighLimit)
#define STRLEN(x)      ((x)->Size)

#define MAX_NUMBER_OF_FIX_TYPES 254
#define LARGE_BLOCK_TYPE 255

typedef struct _MemoryObject {
  unsigned long SizeOfType[MAX_NUMBER_OF_FIX_TYPES]; 
  BYTE iNumberOfFixTypes;    
                             
                             
  BYTE FirstStringIndex,LastStringIndex;
  pFixSizeMemoryObject MemoryObjectsFreeList[MAX_NUMBER_OF_FIX_TYPES];
  void *(*memory_allocating_function)(size_t);
  void (*memory_releasing_function)(void *);
  void *pMemorySegment; 
                        
  unsigned long maxderef;
  } MemoryObject, *pMemoryObject;

#define TYPESIZE(pMo,X) (pMo->SizeOfType(X))
#define IsMortal(x) ((x)->State == STATE_MORTAL)



#define FIX_TYPE_LONG    0
#define FIX_TYPE_DOUBLE  1
#define FIX_TYPE_CSTRING 2 
#define MAX_FIX_TYPE     3
#define FIX_TYPE_ALLOC FIX_TYPE_LONG 

#define STATE_UNKNOWN  0
#define STATE_FREE     1
#define STATE_MORTAL   2
#define STATE_IMMORTAL 3



#define VTYPE_LONG   0 
#define VTYPE_DOUBLE 1 
#define VTYPE_STRING 2 
#define VTYPE_ARRAY  3 
#define VTYPE_REF    4 
#define VTYPE_UNDEF  5 
/*FUNDEF*/

int memory_InitStructure(pMemoryObject pMo);
/*FEDNUF*/
/*FUNDEF*/

int memory_RegisterType(pMemoryObject pMo,
                        unsigned long SizeOfThisType);
/*FEDNUF*/
/*FUNDEF*/

void memory_RegisterTypes(pMemoryObject pMo);
/*FEDNUF*/
/*FUNDEF*/

void memory_DebugDump(pMemoryObject pMo);
/*FEDNUF*/
/*FUNDEF*/

pFixSizeMemoryObject memory_NewVariable(pMemoryObject pMo,
                                        int type,
                                        unsigned long LargeBlockSize);
/*FEDNUF*/
/*FUNDEF*/

int memory_ReleaseVariable(pMemoryObject pMo,
                           pFixSizeMemoryObject p);
/*FEDNUF*/
/*FUNDEF*/

pFixSizeMemoryObject memory_NewString(pMemoryObject pMo,
                                      unsigned long StringSize);
/*FEDNUF*/
/*FUNDEF*/

pFixSizeMemoryObject memory_NewCString(pMemoryObject pMo,
                                       unsigned long StringSize);
/*FEDNUF*/
/*FUNDEF*/

int memory_SetRef(pMemoryObject pMo,
                   pFixSizeMemoryObject *ppVar,
                   pFixSizeMemoryObject *ppVal);
/*FEDNUF*/
/*FUNDEF*/

pFixSizeMemoryObject memory_NewRef(pMemoryObject pMo);
/*FEDNUF*/
/*FUNDEF*/

int memory_IsUndef(pFixSizeMemoryObject pVar);
/*FEDNUF*/
/*FUNDEF*/

int memory_Type(pFixSizeMemoryObject pVar);
/*FEDNUF*/
/*FUNDEF*/

pFixSizeMemoryObject memory_SelfOrRealUndef(pFixSizeMemoryObject pVar);
/*FEDNUF*/
/*FUNDEF*/

pFixSizeMemoryObject memory_NewUndef(pMemoryObject pMo);
/*FEDNUF*/
/*FUNDEF*/

int memory_ReplaceVariable(pMemoryObject pMo,
                           pFixSizeMemoryObject *Lval,
                           pFixSizeMemoryObject NewValue,
                           pMortalList pMortal,
                           int iDupFlag);
/*FEDNUF*/
/*FUNDEF*/

pFixSizeMemoryObject memory_NewLong(pMemoryObject pMo);
/*FEDNUF*/
/*FUNDEF*/

pFixSizeMemoryObject memory_NewDouble(pMemoryObject pMo);
/*FEDNUF*/
/*FUNDEF*/

pFixSizeMemoryObject memory_CopyArray(pMemoryObject pMo,
                                      pFixSizeMemoryObject p);
/*FEDNUF*/
/*FUNDEF*/

pFixSizeMemoryObject memory_NewArray(pMemoryObject pMo,
                                     long LowIndex,
                                     long HighIndex);
/*FEDNUF*/
/*FUNDEF*/

pFixSizeMemoryObject memory_ReDimArray(pMemoryObject pMo,
                                       pFixSizeMemoryObject p,
                                       long LowIndex,
                                       long HighIndex);
/*FEDNUF*/
/*FUNDEF*/

int memory_ArrayIndexOutOfBounds(pFixSizeMemoryObject p,
                                 long Index);
pFixSizeMemoryObject memory_CheckArrayIndex(pMemoryObject pMo,
                                            pFixSizeMemoryObject p,
                                            long Index);
/*FEDNUF*/
/*FUNDEF*/

void memory_Mortalize(pFixSizeMemoryObject p,
                      pMortalList pMortal);
/*FEDNUF*/
/*FUNDEF*/

void memory_Immortalize(pFixSizeMemoryObject p,
                        pMortalList pMortal);
/*FEDNUF*/
/*FUNDEF*/

pFixSizeMemoryObject memory_NewMortal(pMemoryObject pMo,
                                      BYTE type,
                                      unsigned long LargeBlockSize,
                                      pMortalList pMortal);
/*FEDNUF*/
/*FUNDEF*/

pFixSizeMemoryObject memory_DupImmortal(pMemoryObject pMo,
                                        pFixSizeMemoryObject pVar,
                                        int *piErrorCode);
/*FEDNUF*/
/*FUNDEF*/

pFixSizeMemoryObject memory_DupVar(pMemoryObject pMo,
                                   pFixSizeMemoryObject pVar,
                                   pMortalList pMyMortal,
                                   int *piErrorCode);
/*FEDNUF*/
/*FUNDEF*/

pFixSizeMemoryObject memory_DupMortalize(pMemoryObject pMo,
                                         pFixSizeMemoryObject pVar,
                                         pMortalList pMyMortal,
                                         int *piErrorCode);
/*FEDNUF*/
/*FUNDEF*/

void memory_ReleaseMortals(pMemoryObject pMo,
                           pMortalList pMortal);
/*FEDNUF*/
/*FUNDEF*/

void memory_DebugDumpVariable(pMemoryObject pMo,
                              pFixSizeMemoryObject pVar);
/*FEDNUF*/
/*FUNDEF*/

void memory_DebugDumpMortals(pMemoryObject pMo,
                             pMortalList pMortal);
/*FEDNUF*/
/*FUNDEF*/

pFixSizeMemoryObject memory_NewMortalString(pMemoryObject pMo,
                                            unsigned long StringSize,
                                            pMortalList pMortal);
/*FEDNUF*/
/*FUNDEF*/

pFixSizeMemoryObject memory_NewMortalCString(pMemoryObject pMo,
                                             unsigned long StringSize,
                                             pMortalList pMortal);
/*FEDNUF*/
/*FUNDEF*/

pFixSizeMemoryObject memory_NewMortalLong(pMemoryObject pMo,
                                            pMortalList pMortal);
/*FEDNUF*/
/*FUNDEF*/

pFixSizeMemoryObject memory_NewMortalRef(pMemoryObject pMo,
                                         pMortalList pMortal);
/*FEDNUF*/
/*FUNDEF*/

pFixSizeMemoryObject memory_NewMortalDouble(pMemoryObject pMo,
                                            pMortalList pMortal);
/*FEDNUF*/
/*FUNDEF*/

pFixSizeMemoryObject memory_NewMortalArray(pMemoryObject pMo,
                                           pMortalList pMortal,
                                           long IndexLow,
                                           long IndexHigh);
/*FEDNUF*/
#ifdef __cplusplus
}
#endif
#endif
