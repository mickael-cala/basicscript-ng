/*
report.h
*/
#ifndef __REPORT_H__
#define __REPORT_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif

typedef char *ReportMessage(char *Language, int ErrorCode);

typedef void ReportFunction(void *, 
                            char *, 
                            long,   
                            unsigned int,    
                            int,    
                            int *,  
                            char *,  
                            unsigned long * 
                            );

typedef ReportFunction *pReportFunction;



enum {
  REPORT_INFO = 0,
  REPORT_WARNING,
  REPORT_ERROR,
  REPORT_FATAL,
  REPORT_INTERNAL
  };

#define REPORT_F_CGI  0x00000001
#define REPORT_F_FRST 0x00000002 

/*FUNDEF*/

void report_report(void *filepointer,
                   char *FileName,
                   long LineNumber,
                   unsigned int iErrorCode,
                   int iErrorSeverity,
                   int *piErrorCounter,
                   char *szErrorString,
                   unsigned long *fFlags);
/*FEDNUF*/
#ifdef __cplusplus
}
#endif
#endif
