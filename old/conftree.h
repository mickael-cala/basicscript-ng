/*
conftree.h
*/
#ifndef __CONFTREE_H__
#define __CONFTREE_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif

typedef long CFT_NODE;

#define CFT_NODE_LEAF    0x00
#define CFT_NODE_BRANCH  0x01
#define CFT_NODE_MASK    0x01

#define CFT_TYPE_STRING  0x02
#define CFT_TYPE_INTEGER 0x04
#define CFT_TYPE_REAL    0x06
#define CFT_TYPE_MASK    0x06

#define CFT_ERROR_FILE   0x00000001
#define CFT_ERROR_SYNTAX 0x00000002
#define CFT_ERROR_MEMORY 0x00000003
#define CFT_ERROR_EMPTY  0x00000004
#define CFT_ERROR_NOTYPE 0x00000005




typedef struct _tConfigNode {
  long lKey;  
  long lNext; 
  union {
    long lVal;  
                
                
    double dVal;
    }Val;
  unsigned char fFlag; 
  } tConfigNode, *tpConfigNode;

typedef struct _tConfigTree {
  tpConfigNode Root;
  long cNode; 
  char *StringTable;
  unsigned long cbStringTable; 
  void *(*memory_allocating_function)(size_t, void *);
  void (*memory_releasing_function)(void *, void *);
  void *pMemorySegment;
  char *pszConfigFileName;
  char TC;
  } tConfigTree, *ptConfigTree;

#define CFT_ROOT_NODE 1

#define CFT_ERROR_SUCCESS   0x00000000
#define CFT_ERROR_NOT_FOUND 0x00000001

/*FUNDEF*/

int cft_init(ptConfigTree pCT,
              void *(*memory_allocating_function)(size_t, void *),
              void (*memory_releasing_function)(void *, void *),
              void *pMemorySegment);
/*FEDNUF*/
/*FUNDEF*/

int cft_GetConfigFileName(ptConfigTree pCT,
                          char **ppszConfigFile,
                          char *env,/* environment variable or registry key on win32 */
                          char *DefaultFileName);
/*FEDNUF*/
/*FUNDEF*/

int cft_start(ptConfigTree pCT,
              void *(*memory_allocating_function)(size_t, void *),
              void (*memory_releasing_function)(void *, void *),
              void *pMemorySegment,
              char *Envir,
              char *pszDefaultFileName,
              char *pszForcedFileName);
/*FEDNUF*/
/*FUNDEF*/

CFT_NODE cft_FindNode(ptConfigTree pCT,
                      CFT_NODE lStartNode,
                      char *key);
/*FEDNUF*/
/*FUNDEF*/

int cft_GetEx(ptConfigTree pCT,
              char *key,
              CFT_NODE *plNodeId,
              char **ppszValue,
              long *plValue,
              double *pdValue,
              int *type);
/*FEDNUF*/
/*FUNDEF*/

char *cft_GetString(ptConfigTree pCT,
                    char *key);
/*FEDNUF*/
/*FUNDEF*/

CFT_NODE cft_EnumFirst(ptConfigTree pCT,
                       CFT_NODE lNodeId);
/*FEDNUF*/
/*FUNDEF*/

long cft_EnumNext(ptConfigTree pCT,
                  long lNodeId);
/*FEDNUF*/
/*FUNDEF*/

char *cft_GetKey(ptConfigTree pCT,
                 CFT_NODE lNodeId);
/*FEDNUF*/
/*FUNDEF*/

int cft_ReadConfig(ptConfigTree pCT,
                   char *pszFileName);
/*FEDNUF*/
/*FUNDEF*/

int cft_WriteConfig(ptConfigTree pCT,
                    char *pszFileName);
/*FEDNUF*/
/*FUNDEF*/

void cft_DropConfig(ptConfigTree pCT);
/*FEDNUF*/
#ifdef __cplusplus
}
#endif
#endif
