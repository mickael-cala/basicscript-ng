/*
reader.h
*/
#ifndef __READER_H__
#define __READER_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif

typedef struct _SourceLine {
  char *line;
  long lLineNumber;
  long LineLength;
  char *szFileName;
  struct _SourceLine *next;
  } SourceLine, *pSourceLine;

typedef struct _ImportedFileList {
  char *pszFileName;
  struct _ImportedFileList *next;
  }ImportedFileList,*pImportedFileList;

typedef struct _ReadObject {
  void * (*fpOpenFile)(char *, void *);  
  int (*fpGetCharacter)(void *, void *); 
  void (*fpCloseFile)(void *, void *);   
  void *pFileHandleClass;

  void *(*memory_allocating_function)(size_t, void *);
  void (*memory_releasing_function)(void *, void *);
  void *pMemorySegment; 

  ptConfigTree pConfig;

#define BUFFER_INITIAL_SIZE 1024 
#define BUFFER_INCREMENT 1024 
  char *Buffer;  
  long dwBuffer; 
  long cBuffer;  

  pSourceLine Result; 


  pSourceLine CurrentLine; 
  long NextCharacterPosition; 
  char fForceFinalNL; 
                      

  pReportFunction report;
  void *reportptr; 
  int iErrorCounter;
  unsigned long fErrorFlags;

  pImportedFileList pImportList;

  char *FirstUNIXline;
  struct _PreprocObject *pPREP;
  } ReadObject, *pReadObject;

/*FUNDEF*/

int reader_IncreaseBuffer(pReadObject pRo);
/*FEDNUF*/
/*FUNDEF*/

int reader_gets(pReadObject pRo,
                 void *fp);
/*FEDNUF*/
/*FUNDEF*/

int reader_ReadLines(pReadObject pRo,
                     char *szFileName);
/*FEDNUF*/
/*FUNDEF*/

int reader_ReadLines_r(pReadObject pRo,
                       char *szFileName,
                       pSourceLine *pLine);
/*FEDNUF*/
/*FUNDEF*/

void reader_ProcessIncludeFiles(pReadObject pRo,
                                pSourceLine *pLine);
/*FEDNUF*/
/*FUNDEF*/

void reader_LoadPreprocessors(pReadObject pRo,
                                 pSourceLine *pLine);
/*FEDNUF*/
/*FUNDEF*/

void reader_StartIteration(pReadObject pRo);
/*FEDNUF*/
/*FUNDEF*/

char *reader_NextLine(pReadObject pRo);
/*FEDNUF*/
/*FUNDEF*/

int reader_NextCharacter(void *p);
/*FEDNUF*/
/*FUNDEF*/

char *reader_FileName(void *p);
/*FEDNUF*/
/*FUNDEF*/

long reader_LineNumber(void *p);
/*FEDNUF*/
/*FUNDEF*/

void reader_InitStructure(pReadObject pRo);
/*FEDNUF*/
/*FUNDEF*/

char *reader_RelateFile(pReadObject pRo,
                       char *pszBaseFile,
                       char *pszRelativeFile);
/*FEDNUF*/
/*FUNDEF*/

void reader_DumpLines(pReadObject pRo,
                      FILE *fp);
/*FEDNUF*/
#ifdef __cplusplus
}
#endif
#endif
