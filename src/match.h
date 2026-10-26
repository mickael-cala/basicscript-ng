/*
match.h
*/
#ifndef __MATCH_H__
#define __MATCH_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif

#define JOKERNR 13

#define NOJOKER     0
#define SINGLEJOKER 1
#define MULTIJOKER  3

typedef struct _MatchSets {
  unsigned char SetType[JOKERNR];
  unsigned char set[JOKERNR][32];
  } MatchSets, *pMatchSets;

/*FUNDEF*/

unsigned long match_index(char ch);
/*FEDNUF*/
/*FUNDEF*/

void match_InitSets(pMatchSets pMS);
/*FEDNUF*/

#define MATCH_ADDC 0x0001 
#define MATCH_REMC 0x0002 
#define MATCH_INVC 0x0004 
#define MATCH_SNOJ 0x0008 
#define MATCH_SSIJ 0x0010 
#define MATCH_SMUJ 0x0020 
#define MATCH_NULS 0x0040 
#define MATCH_FULS 0x0080 

/*FUNDEF*/

void match_ModifySet(pMatchSets pMS,
                     char JokerCharacter,
                     int nChars,
                     unsigned char *pch,
                     int fAction);
/*FEDNUF*/
/*FUNDEF*/

int match_match(char *pszPattern,
                unsigned long cbPattern,
                char *pszString,
                unsigned long cbString,
                char **ParameterArray,
                unsigned long *pcbParameterArray,
                char *pszBuffer,
                int cArraySize,
                int cbBufferSize,
                int fCase,
                pMatchSets pThisMatchSets,
                int *iResult);
/*FEDNUF*/
/*FUNDEF*/

int match_count(char *pszPattern,
                unsigned long cbPattern);
/*FEDNUF*/
/*FUNDEF*/

int match_parameter(char *pszFormat,
                    unsigned long cbFormat,
                    char **ParameterArray,
                    unsigned long *pcbParameterArray,
                    char *pszBuffer,
                    int cArraySize,
                    unsigned long *pcbBufferSize);
/*FEDNUF*/
/*FUNDEF*/

int match_size(char *pszFormat,
               unsigned long cbFormat,
               unsigned long *pcbParameterArray,
               int cArraySize,
               int *cbBufferSize);
/*FEDNUF*/
#ifdef __cplusplus
}
#endif
#endif
