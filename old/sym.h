/*
sym.h
*/
#ifndef __SYM_H__
#define __SYM_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif
#include <stddef.h>
typedef struct _symbol {
  char  *name;
  void  *value;
  struct _symbol *small_son, *big_son;
  } Symbol, *pSymbol, **SymbolTable;

#define SymbolValue(x) (x->value)

#define SymbolName(x) ((char *)*((char **)( ((unsigned char *)(x)) - offsetof(struct _symbol,value) + offsetof(struct _symbol,name) )))

/*FUNDEF*/

SymbolTable sym_NewSymbolTable(
  void* (*memory_allocating_function)(size_t,void *),
  void *pMemorySegment);
/*FEDNUF*/
/*FUNDEF*/

void sym_FreeSymbolTable(
  SymbolTable table,
  void (*memory_releasing_function)(void *,void *),
  void *pMemorySegment);
/*FEDNUF*/
/*FUNDEF*/

void sym_TraverseSymbolTable(
  SymbolTable table,
  void (*call_back_function)(char *SymbolName, void *SymbolValue, void *f),
  void *f);
/*FEDNUF*/
/*FUNDEF*/

void **sym_LookupSymbol(
  char *s,                 /* zero terminated string containing the symbol                 */
  SymbolTable hashtable,   /* the symbol table                                             */
  int insert,              /* should a new empty symbol inserted, or return NULL instead   */
  void* (*memory_allocating_function)(size_t, void *),
  void (*memory_releasing_function)(void *, void *),
  void *pMemorySegment);
/*FEDNUF*/
/*FUNDEF*/

int sym_DeleteSymbol(
  char *s,                 /* zero terminated string containing the symbol                 */
  SymbolTable hashtable,   /* the symbol table                                             */
  void (*memory_releasing_function)(void *, void *),
  void *pMemorySegment);
/*FEDNUF*/
#ifdef __cplusplus
}
#endif
#endif
