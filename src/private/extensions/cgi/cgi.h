/*
cgi.h
*/
#ifndef __CGI_H__
#define __CGI_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif

typedef struct _SymbolList {
  char *symbol;
  FILE *fp;   
  char *file; 
  char *value;
  long len;   
  struct _SymbolList *pHeaders; 
  struct _SymbolList *next;
  } SymbolList, *pSymbolList;

typedef struct _DebugStore {
  char *ServerSoftware;
  char *ServerName;
  char *GatewayInterface;
  char *ServerProtocol;
  char *ServerPort;
  char *RequestMethod;
  char *PathInfo;
  char *PathTranslated;
  char *ScriptName;
  char *QueryString;
  char *RemoteHost;
  char *RemoteAddress;
  char *AuthType;
  char *RemoteUser;
  char *RemoteIdent;
  char *ContentType;
  char *ContentLength;
  char *UserAgent;
  char *Cookie;

  FILE *fpDebugInput;
  } DebugStore, *pDebugStore;

typedef struct _CgiObject {
  void *(*maf)(long size, void *pSegment);
  void (*mrf)(void *MemoryToFree, void *pSegment);
  void *pSegment;

#define CGI_INTERFACE_CGI   0x00000000 
#define CGI_INTERFACE_ISAPI 0x00000001
#define CGI_INTERFACE_NSAPI 0x00000002
#define CGI_INTERFACE_FCGI  0x00000003
#define CGI_INTERFACE_DEBUG 0x00000004 
  long fInterface;   

#ifdef WIN32
  LPEXTENSION_CONTROL_BLOCK lpECB;
  char *pszNextChar;
  char *pszLocalBuffer;
  DWORD cbAvailable;
  DWORD dwIsapiBufferSize;
#else
#define LPEXTENSION_CONTROL_BLOCK void *
#endif

  void *pEmbed; 
  int (*pfStdIn)(void *); 
  void (*pfStdOut)(int, void*); 
  char *(*pfEnv)(void *,char *,long); 

  char *pszDebugFile; 
  pDebugStore pDebugInfo; 

  char *pszBoundary; 
  unsigned long cbBoundary;   
  unsigned char *pszBuffer;   
  unsigned long cbBuffer;     
  unsigned long cbFill;       
  unsigned long lBufferPosition; 
  int (*CharacterInput)(struct _CgiObject *p);
  void *pInputParameter; 
  unsigned long lContentLength;  

  pSymbolList pGetParameters; 
  pSymbolList pPostParameters; 

  unsigned long lBufferIncrease; 
  unsigned long lBufferMax;      
  unsigned long lContentMax;     
  unsigned long lFileMax;        

#define CGI_METHOD_NONE 0x00000000
#define CGI_METHOD_GET  0x00000001
#define CGI_METHOD_POST 0x00000002
#define CGI_METHOD_UPL  0x00000004 
                                   

#define CGI_METHOD_PUT  0x00000008
#define CGI_METHOD_DEL  0x00000010 
#define CGI_METHOD_COPY 0x00000020
#define CGI_METHOD_MOVE 0x00000040
#define CGI_METHOD_HEAD 0x00000080

  long fMethods;        
  } CgiObject, *pCgiObject;


#define CGI_ERROR_BUFFER_OVERFLOW 0x00080001
#define CGI_ERROR_BIG_CONTENT     0x00080002
#define CGI_ERROR_INVALID_METHOD  0x00080003
#define CGI_ERROR_NO_DEBUG_FILE   0x00080004
#define CGI_ERROR_NOTIMP          0x00080005
#define CGI_ERROR_EOF             0x00080006
#define CGI_ERROR_ILLF_MULTI      0x00080007
#define CGI_ERROR_FILEMAX         0x00080008
#define CGI_ERROR_MEMORY_LOW      0x00080009
#define CGI_ERROR_METHOD_NOTALL   0x0008000A
#define CGI_ERROR_ILLF_MULTI1     0x00080017
#define CGI_ERROR_ILLF_MULTI2     0x00080027
#define CGI_ERROR_ILLF_MULTI3     0x00080037
#define CGI_ERROR_ILLF_MULTI4     0x00080047
#define CGI_ERROR_ILLF_MULTI5     0x00080057
#define CGI_ERROR_ILLF_MULTI6     0x00080067
#define CGI_ERROR_ILLF_MULTI7     0x00080077


#define cgi_EmptyBuffer(x) ((x)->cbFill=0)
#define cgi_BufferFull(x)  ((x)->cbFill == (x)->cbBuffer)
/*FUNDEF*/

void cgi_InitCgi(pCgiObject pCO);
/*FEDNUF*/
/*FUNDEF*/

void cgi_InitIsapi(pCgiObject pCO,
                   LPEXTENSION_CONTROL_BLOCK lpECB);
/*FEDNUF*/
/*FUNDEF*/

long cgi_ReadHttpRequest(pCgiObject pCO);
/*FEDNUF*/
/*FUNDEF*/

char *cgi_PostParam(pCgiObject pCO,
                    char *pszParam);
/*FEDNUF*/
/*FUNDEF*/

char *cgi_GetParam(pCgiObject pCO,
                   char *pszParam);
/*FEDNUF*/
/*FUNDEF*/

char *cgi_PostParamEx(pCgiObject pCO,
                      char *pszParam,
                      pSymbolList *p);
/*FEDNUF*/
/*FUNDEF*/

char *cgi_GetParamEx(pCgiObject pCO,
                     char *pszParam,
                     pSymbolList *p);
/*FEDNUF*/
/*FUNDEF*/

FILE *cgi_FILEp(pCgiObject pCO,
                char *pszParam);
/*FEDNUF*/
/*FUNDEF*/

char *cgi_OriginalFileName(pCgiObject pCO,
                char *pszParam);
/*FEDNUF*/
/*FUNDEF*/

long cgi_FileLength(pCgiObject pCO,
                char *pszParam);
/*FEDNUF*/
/*FUNDEF*/

pSymbolList cgi_PartHeader(pCgiObject pCO,
                     char *pszParam);
/*FEDNUF*/
/*FUNDEF*/

char *cgi_Header(pCgiObject pCO,
                 char *symbol,
                 pSymbolList pHeader);
/*FEDNUF*/
/*FUNDEF*/

char *cgi_Referer(pCgiObject pCO);
/*FEDNUF*/
/*FUNDEF*/

char *cgi_Cookie(pCgiObject pCO);
/*FEDNUF*/
/*FUNDEF*/

char *cgi_ServerSoftware(pCgiObject pCO);
/*FEDNUF*/
/*FUNDEF*/

char *cgi_ServerName(pCgiObject pCO);
/*FEDNUF*/
/*FUNDEF*/

char *cgi_GatewayInterface(pCgiObject pCO);
/*FEDNUF*/
/*FUNDEF*/

char *cgi_ServerProtocol(pCgiObject pCO);
/*FEDNUF*/
/*FUNDEF*/

char *cgi_ServerPort(pCgiObject pCO);
/*FEDNUF*/
/*FUNDEF*/

char *cgi_RequestMethod(pCgiObject pCO);
/*FEDNUF*/
/*FUNDEF*/

char *cgi_PathInfo(pCgiObject pCO);
/*FEDNUF*/
/*FUNDEF*/

char *cgi_PathTranslated(pCgiObject pCO);
/*FEDNUF*/
/*FUNDEF*/

char *cgi_ScriptName(pCgiObject pCO);
/*FEDNUF*/
/*FUNDEF*/

char *cgi_QueryString(pCgiObject pCO);
/*FEDNUF*/
/*FUNDEF*/

char *cgi_RemoteHost(pCgiObject pCO);
/*FEDNUF*/
/*FUNDEF*/

char *cgi_RemoteAddress(pCgiObject pCO);
/*FEDNUF*/
/*FUNDEF*/

char *cgi_AuthType(pCgiObject pCO);
/*FEDNUF*/
/*FUNDEF*/

char *cgi_RemoteUser(pCgiObject pCO);
/*FEDNUF*/
/*FUNDEF*/

char *cgi_RemoteIdent(pCgiObject pCO);
/*FEDNUF*/
/*FUNDEF*/

char *cgi_ContentType(pCgiObject pCO);
/*FEDNUF*/
/*FUNDEF*/

char *cgi_ContentLength(pCgiObject pCO);
/*FEDNUF*/
/*FUNDEF*/

char *cgi_UserAgent(pCgiObject pCO);
/*FEDNUF*/
/*FUNDEF*/

int cgi_ResizeBuffer(pCgiObject pCO,
                     unsigned long lNewSize);
/*FEDNUF*/
/*FUNDEF*/

long cgi_FillBuffer(pCgiObject pCO);
/*FEDNUF*/
/*FUNDEF*/

void cgi_ShiftBuffer(pCgiObject pCO,
                     unsigned long nch);
/*FEDNUF*/
/*FUNDEF*/

void cgi_NormalizeBuffer(pCgiObject pCO);
/*FEDNUF*/
/*FUNDEF*/

long cgi_SkipAfterBoundary(pCgiObject pCO);
/*FEDNUF*/
/*FUNDEF*/

int cgi_GetNextByte(pCgiObject pCO);
/*FEDNUF*/
/*FUNDEF*/

int cgi_GetNextChar(pCgiObject pCO);
/*FEDNUF*/
/*FUNDEF*/

long cgi_ReadHeader(pCgiObject pCO,
                   pSymbolList *pHeader);
/*FEDNUF*/
/*FUNDEF*/

int cgi_ResizeThisBuffer(pCgiObject pCO,
                         char **ppszBuffer,
                         long *plOldSize,
                         long lNewSize);
/*FEDNUF*/
/*FUNDEF*/

void cgi_FillSymbolAndFile(pCgiObject pCO,
                           char *pszContentDisposition,
                           pSymbolList pHeader);
/*FEDNUF*/
/*FUNDEF*/

long cgi_GetMultipartParameters(pCgiObject pCO);
/*FEDNUF*/
/*FUNDEF*/

long cgi_GetGetParameters(pCgiObject pCO);
/*FEDNUF*/
/*FUNDEF*/

long cgi_GetPostParameters(pCgiObject pCO);
/*FEDNUF*/
#ifdef __cplusplus
}
#endif
#endif
