/*
md5.h
*/
#ifndef __MD5_H__
#define __MD5_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif

typedef unsigned char *POINTER;

typedef unsigned short int UINT2;

typedef unsigned long int UINT4;

#define PROTO_LIST(list) list

#define MAXTO 200



typedef struct {
  UINT4 state[4];                                   
  UINT4 count[2];        
  unsigned char buffer[64];                         
} MD5_CTX;

void MD5Init PROTO_LIST ((MD5_CTX *));
void MD5Update PROTO_LIST
  ((MD5_CTX *, unsigned char *, unsigned int));
void MD5Final PROTO_LIST ((unsigned char [16], MD5_CTX *));


#ifdef __cplusplus
}
#endif
#endif
