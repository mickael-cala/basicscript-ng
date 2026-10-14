/*
lexer.h
*/
#ifndef __LEXER_H__
#define __LEXER_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif

enum LexemeType {
  LEX_T_DOUBLE = 1,
  LEX_T_LONG,
  LEX_T_STRING,
  LEX_T_ASYMBOL,
  LEX_T_NSYMBOL,
  LEX_T_CHARACTER,

  LEX_T_SKIP,   
                
  LEX_T_SKIP_SYMBOL,

  LEX_T_DUMMY
  };


typedef struct _Lexeme {
  enum LexemeType type;   
  union {
    double dValue;        
    long   lValue;        
    char  *sValue;        
    } value;
  long sLen;              
  char *szFileName;       
  long lLineNumber;       
  struct _Lexeme *next;   
  }Lexeme, *pLexeme;

typedef struct _LexNASymbol {
  char *Symbol;
  int Code;
  } LexNASymbol, *pLexNASymbol;

typedef struct _LexObject {
  int (*pfGetCharacter)(void *); 
  char * (*pfFileName)(void *);  
  long (*pfLineNumber)(void *);  
  void *pvInput;
  void *(*memory_allocating_function)(size_t, void *);
  void (*memory_releasing_function)(void *, void *);
  void *pMemorySegment; 

  char *SSC;  
  char *SCC;  
  char *SFC;  
  char *SStC; 
  char *SKIP; 
              
              


  char *ESCS;
  long fFlag;

  pReportFunction report;
  void *reportptr; 
  int iErrorCounter;
  unsigned long fErrorFlags;

  char *buffer;  
  long cbBuffer; 

  pLexNASymbol pNASymbols; 
  int cbNASymbolLength;    

  pLexNASymbol pASymbols;  

  pLexNASymbol pCSymbols;  

  pLexeme pLexResult;      
  pLexeme pLexCurrentLexeme; 
  struct _PreprocObject *pPREP;
  }LexObject, *pLexObject;

#define LEX_PROCESS_STRING_NUMBER         0x01
#define LEX_PROCESS_STRING_OCTAL_NUMBER   0x02
#define LEX_ASYMBOL_CASE_SENSITIVE        0x04
#define LEX_PROCESS_STRING_HEX_NUMBER     0x08
/*FUNDEF*/

char *lex_SymbolicName(pLexObject pLex,
                       long OpCode);
/*FEDNUF*/
/*FUNDEF*/

void lex_HandleContinuationLines(pLexObject pLex);
/*FEDNUF*/
/*FUNDEF*/

void lex_RemoveSkipSymbols(pLexObject pLex);
/*FEDNUF*/
/*FUNDEF*/

void lex_RemoveComments(pLexObject pLex);
/*FEDNUF*/
/*FUNDEF*/

void lex_NextLexeme(pLexObject pLex);
/*FEDNUF*/
/*FUNDEF*/

void lex_SavePosition(pLexObject pLex,
                      pLexeme *ppPosition);
/*FEDNUF*/
/*FUNDEF*/

void lex_RestorePosition(pLexObject pLex,
                         pLexeme *ppPosition);
/*FEDNUF*/
/*FUNDEF*/

void lex_StartIteration(pLexObject pLex);
/*FEDNUF*/
/*FUNDEF*/

int lex_EOF(pLexObject pLex);
/*FEDNUF*/
/*FUNDEF*/

int lex_Type(pLexObject pLex);
/*FEDNUF*/
/*FUNDEF*/

double lex_Double(pLexObject pLex
/*noverbatim
CUT*/);
/*FEDNUF*/
/*FUNDEF*/

char *lex_String(pLexObject pLex);
/*FEDNUF*/
/*FUNDEF*/

long lex_StrLen(pLexObject pLex);
/*FEDNUF*/
/*FUNDEF*/

long lex_Long(pLexObject pLex);
/*FEDNUF*/
/*FUNDEF*/

long lex_LineNumber(pLexObject pLex);
/*FEDNUF*/
/*FUNDEF*/

char *lex_FileName(pLexObject pLex);
/*FEDNUF*/
#define lex_Int(x) lex_Long(x)
#define lex_Symbol(x) lex_String(x)
#define lex_Float(x) lex_Double(x)
#define lex_Char(x) lex_Long(x)
#define lex_Token(x) lex_Long(x)
#define lex_Code(x) lex_Long(x)
/*FUNDEF*/

void lex_Finish(pLexObject pLex);
/*FEDNUF*/
/*FUNDEF*/

void lex_DumpLexemes(pLexObject pLex,
                     FILE *psDump);
/*FEDNUF*/
/*FUNDEF*/

int lex_ReadInput(pLexObject pLex);
/*FEDNUF*/
/*FUNDEF*/

void lex_InitStructure(pLexObject pLex);
/*FEDNUF*/
#ifdef __cplusplus
}
#endif
#endif
