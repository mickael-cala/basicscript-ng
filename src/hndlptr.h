/*
hndlptr.h
*/
#ifndef __HNDLPTR_H__
#define __HNDLPTR_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif

/*FUNDEF*/

unsigned long handle_GetHandle(void **pHandle,
                               void *pMEM,
                               void *pointer);
/*FEDNUF*/
/*FUNDEF*/

void *handle_GetPointer(void **pHandle,
                        unsigned long handle);
/*FEDNUF*/
/*FUNDEF*/

void handle_FreeHandle(void **pHandle,
                       unsigned long handle);
/*FEDNUF*/
/*FUNDEF*/

void handle_DestroyHandleArray(void **pHandle,
                               void *pMEM);
/*FEDNUF*/
#ifdef __cplusplus
}
#endif
#endif
