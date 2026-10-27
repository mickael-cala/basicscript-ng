/*
builder.h
*/
#ifndef __BUILDER_H__
#define __BUILDER_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif

#ifdef WIN32
#pragma warning( disable : 4103 ) 


#pragma pack(push,4)
#endif
typedef struct _cNODE {
  long OpCode; 
  union {

    
    struct {
      unsigned long next;
      union {
        unsigned long pNode;  
        long lLongValue;
        double dDoubleValue;
        unsigned long szStringValue; 
        }Argument;
      }CommandArgument;

    
    struct {
      unsigned long Argument; 
      }Arguments;

    
    union {
      double dValue;        
      long long lValue;     /* phase 8.2 (E44) */
      unsigned long sValue; 
      }Constant;

    
    struct {
      unsigned long Serial; 
      }Variable;

    
    struct {
      unsigned long NodeId; 
      unsigned long Argument; 
      }UserFunction;

    
    struct {
      unsigned long actualm; 
      unsigned long rest;    
      }NodeList;


    }Parameter;
  } cNODE,*pcNODE;

#ifdef WIN32
#pragma pack(pop)
#endif




typedef struct _SymbolLongTable {
  long value;
  char symbol[1];
  } SymbolLongTable, *pSymbolLongTable;

typedef struct _BuildObject {
  void *(*memory_allocating_function)(size_t);
  void (*memory_releasing_function)(void *);
  void *pMemorySegment; 
                        
  char *StringTable; 
  unsigned long cbStringTable; 
  unsigned long cbCollectedStrings; 

  int iErrorCounter;

  long cGlobalVariables;

  pcNODE CommandArray;
  unsigned long NodeCounter; 
  unsigned long StartNode;

  unsigned long cbFTable; 
  unsigned long cbVTable; 
  pSymbolLongTable FTable; 
  pSymbolLongTable VTable; 

  peXobject pEx; 
  pReportFunction report;
  void *reportptr; 
  unsigned long fErrorFlags;
  char *FirstUNIXline;
  struct _PreprocObject *pPREP;
  } BuildObject, *pBuildObject;

#define MAGIC_CODE   0x1A534142
#define VERSION_HIGH 0x00000002
#define VERSION_LOW  0x00000000
#define MYVERSION_HIGH 0x00000000
#define MYVERSION_LOW  0x00000000
#define VARIATION "STANDARD"
typedef struct _VersionInfo {
  unsigned long MagicCode;
  unsigned long VersionHigh, VersionLow;
  unsigned long MyVersionHigh,MyVersionLow;
  unsigned long Build;
  unsigned long Date;
  unsigned char Variation[9];
  } VersionInfo,*pVersionInfo;

#define BU_SAVE_FTABLE 0x00000001
#define BU_SAVE_VTABLE 0x00000002
/*FUNDEF*/

void build_AllocateStringTable(pBuildObject pBuild,
                          int *piFailure);
/*FEDNUF*/
/*FUNDEF*/

unsigned long build_StringIndex(pBuildObject pBuild,
                                char *s,
                                long sLen);
/*FEDNUF*/
/*FUNDEF*/

int build_Build_l(pBuildObject pBuild,
                  peNODE_l Result);
/*FEDNUF*/
/*FUNDEF*/

int build_Build_r(pBuildObject pBuild,
                  peNODE Result);
/*FEDNUF*/
/*FUNDEF*/

int build_Build(pBuildObject pBuild);
/*FEDNUF*/
/*FUNDEF*/

unsigned long build_MagicCode(pVersionInfo p);
/*FEDNUF*/
/*FUNDEF*/

void build_SaveCCode(pBuildObject pBuild,
                    char *szFileName);
/*FEDNUF*/
/*FUNDEF*/

int build_SaveCorePart(pBuildObject pBuild,
                       FILE *fp,
                       unsigned long fFlag);
/*FEDNUF*/
/*FUNDEF*/

int build_SaveCore(pBuildObject pBuild,
                   FILE *fp);
/*FEDNUF*/
/*FUNDEF*/

int build_SaveCode(pBuildObject pBuild,
                   char *szFileName);
/*FEDNUF*/
/*FUNDEF*/

void build_SaveECode(pBuildObject pBuild,
                     char *pszInterpreter,
                     char *szFileName);
/*FEDNUF*/
/*FUNDEF*/

int build_GetExeCodeOffset(char *pszInterpreter,
                            long *plOffset,
                            long *plEOFfset);
/*FEDNUF*/
/*FUNDEF*/

void build_LoadCore(pBuildObject pBuild,
                    char *szFileName,
                    FILE *fp,
                    long lEOFfset);
/*FEDNUF*/
/*FUNDEF*/

void build_LoadCodeWithOffset(pBuildObject pBuild,
                              char *szFileName,
                              long lOffset,
                              long lEOFfset);
/*FEDNUF*/
/*FUNDEF*/

void build_LoadCode(pBuildObject pBuild,
                    char *szFileName);
/*FEDNUF*/
/*FUNDEF*/

int build_IsFileBinaryFormat(char *szFileName);
/*FEDNUF*/
/*FUNDEF*/

void build_pprint(pBuildObject pBuild,
                  FILE *f);
/*FEDNUF*/
/*FUNDEF*/

int build_CreateFTable(pBuildObject pBuild);
/*FEDNUF*/
/*FUNDEF*/

int build_CreateVTable(pBuildObject pBuild);
/*FEDNUF*/
/*FUNDEF*/

long build_LookupFunctionByName(pBuildObject pBuild,
                          char *s);
/*FEDNUF*/
/*FUNDEF*/

long build_LookupVariableByName(pBuildObject pBuild,
                          char *s);
/*FEDNUF*/
#ifdef __cplusplus
}
#endif
#endif
