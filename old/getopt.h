/*
getopt.h
*/
#ifndef __GETOPT_H__
#define __GETOPT_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif
/*FUNDEF*/

char getoptt(int argc, 
            char **argv,
            char *optstring,
            char **optarg,
            int *poptind);
/*FEDNUF*/
#ifdef __cplusplus
}
#endif
#endif
