/*
httpd.h
*/
#ifndef __HTTPD_H__
#define __HTTPD_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif

#ifdef WIN32
typedef u_short port_t;
#else
#ifndef __DARWIN__
typedef int port_t;
#endif
#define closesocket close
#endif


#define HTTPD_PORT 80

#define HTTPD_IP INADDR_ANY


#define LISTEN_BACKLOG 200


#define CONCURRENT_HITS 100



#define HIT_MAX_SIZE 32768

#define MAX_HEADER_LINES 100

#define MAX_QUERY_LENGTH 256


struct _ThreadData;
struct _HttpdThread;

struct _fun {
  char *(*pGetServerVariable)(struct _ThreadData *, char *);
  int (*pWriteClient)(struct _ThreadData *, char *, int);
  int (*pReadClient)(struct _ThreadData *, char *, int);
  int (*pWriteClientText)(struct _ThreadData *, char *);
  int (*pState)(struct _ThreadData *, int);
  int (*pContentType)(struct _ThreadData *, char *);
  int (*pHeader)(struct _ThreadData *, char *, char *);
  int (*pStartBody)(struct _ThreadData *);
  char *(*pGetParam)(struct _ThreadData *, char *);
  char *(*pPostParam)(struct _ThreadData *, char *);
  char *(*pScriptName)(struct _ThreadData *);
  void (*pCloseClient)(struct _ThreadData *);
  void (*HttpProc)(struct _HttpdThread *, struct _ThreadData *);
  int  (*FtpProc)(struct _HttpdThread *, struct _ThreadData *, char *);
  };



#define GetServerVariable(X) (pT->pFunctions->pGetServerVariable(pT,(X)))
#define WriteClient(X,Y)     (pT->pFunctions->pWriteClient(pT,(X),(Y)))
#define ReadClient(X,Y)      (pT->pFunctions->pReadClient(pT,(X),(Y)))
#define WriteClientText(X)   (pT->pFunctions->pWriteClientText(pT,(X)))
#define State(X)             (pT->pFunctions->pState(pT,(X)))
#define ContentType(X)       (pT->pFunctions->pContentType(pT,(X)))
#define Header(X,Y)          (pT->pFunctions->pHeader(pT,(X),(Y)))
#define StartBody()          (pT->pFunctions->pStartBody(pT))
#define GetParam(X)          (pT->pFunctions->pGetParam(pT,(X)))
#define PostParam(X)         (pT->pFunctions->pPostParam(pT,(X)))
#define ScriptName()         (pT->pFunctions->pScriptName(pT))
#define CloseClient()        (pT->pFunctions->pCloseClient(pT))


typedef struct _ThreadData {
  int                ThreadIndex;      
  int                SocketOpened;     
  int                server_index;     
  SOCKET             msgsock;          
  int                NextFree;         
  void              *pThreadLocalData; 
  THREADHANDLE       T;
  struct sockaddr_in addr;


  int                  iHeaderLines;            
  char                *HeaderKey[MAX_HEADER_LINES],
                      *HeaderString[MAX_HEADER_LINES];


  char                *pszMethod;             

  char                *pszQueryString;        

  unsigned char        ClientIP[4];

  unsigned int         cbAvailable;   
  unsigned char       *pszData;      
  struct _fun         *pFunctions;          
  char                 getparams[MAX_QUERY_LENGTH];
  int                  getparlen;
  char                 script[MAX_QUERY_LENGTH];
  int                  scriptlen;
  char                 buffer[HIT_MAX_SIZE];
  struct _HttpdThread *pHT;  
  
  void                *AppThreadData;
  } ThreadData, *pThreadData;

  
#define SERVER_NONE      0
#define SERVER_HTTP      1
#define SERVER_FTP       2
typedef struct _ServerData {
  port_t        port;
  unsigned long ip;
  int           type;
  char          *salute;
  char          *codebase; 
  
  SOCKET         sock;
  unsigned long cAllowed;
  unsigned long *plAllowedIP;
  unsigned long *plAllowedMask;

  unsigned long cDenied;
  unsigned long *plDeniedIP;
  unsigned long *plDeniedMask;

  
  char          *USER;
  char          *PASS;
  char          *ACCT;
  char          *CWD;
  char          *CDUP;
  char          *SMNT;
  char          *REIN;
  char          *QUIT;
  char          *PORT;
  char          *PASV;
  char          *TYPE;
  char          *STRU;
  char          *MODE;
  char          *RETR;
  char          *STOR;
  char          *STOU;
  char          *APPE;
  char          *ALLO;
  char          *REST;
  char          *RNFR;
  char          *RNTO;
  char          *ABOR;
  char          *DELE;
  char          *MKD;
  char          *PWD;
  char          *LIST;
  char          *NLST;
  char          *SITE;
  char          *SYST;
  char          *STAT;
  char          *HELP;
  char          *NOOP;

  } ServerData, *pServerData;

typedef struct _HttpdThread {

  
  pThreadData    threads;

  
  int            FirstFreeThread;

  
  MUTEX          mxFirstFreeThread;

  
  int            cThread;

  
  unsigned long  lWaitSec;

  
  unsigned long  lWaitCount;

  
#define MAX_SERVERS 100
  ServerData    server[MAX_SERVERS];
  
  int            c_servers;

  
  int            threadmax;

  
  int            listenmax;

  int            iState; 
#define STATE_NORMAL 0
#define STATE_SHUT   1 

  MUTEX          mxState; 

  
  void          *AppData;
  } HttpdThread, *pHttpdThread;

/*FUNDEF*/

void InitSignalHandlers(void);
/*FEDNUF*/
/*FUNDEF*/

int httpd(int argc,
          char *argv[],
          int (*AppInit)(int argc,char *argv[],pHttpdThread pHT,void **AppData),
          int (*AppStart)(void **AppData),
          void (*HttpProc)(pHttpdThread pHT,pThreadData ThisThread),
          int (*FtpProc)(pHttpdThread pHT,pThreadData ThisThread, char *pszCommand));
/*FEDNUF*/
/*FUNDEF*/

pThreadData GetFreeThread(pHttpdThread pHT);
/*FEDNUF*/
/*FUNDEF*/

void ReleaseThreadData(pHttpdThread pHT,
                       int index);
/*FEDNUF*/
/*FUNDEF*/

int CheckAllowDeny(pThreadData ThisThread);
/*FEDNUF*/
/*FUNDEF*/

void FinishConnection(pThreadData ThisThread);
/*FEDNUF*/
/*FUNDEF*/

void HitHandler(void *t);
/*FEDNUF*/
/*FUNDEF*/

void HandleFtpHit(pThreadData ThisThread);
/*FEDNUF*/
/*FUNDEF*/

void HandleHttpHit(pThreadData ThisThread);
/*FEDNUF*/
#ifdef __cplusplus
}
#endif
#endif
