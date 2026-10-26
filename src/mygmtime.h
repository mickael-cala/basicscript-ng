/*
mygmtime.h
*/
#ifndef __MYGMTIME_H__
#define __MYGMTIME_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif
#define _DAY_SEC           (24L * 60L * 60L)
#define _YEAR_SEC          (365L * _DAY_SEC)
#define _FOUR_YEAR_SEC     (1461L * _DAY_SEC)
#define _BASE_DOW          4                
#define _BASE_YEAR         70L              
#define _MAX_YEAR          138L             
#define _LEAP_YEAR_ADJUST  17L              

/*FUNDEF*/

long mygmktime(struct tm *tb);
/*FEDNUF*/
/*FUNDEF*/

struct tm * mygmtime (time_t *timp, struct tm *ptb);
/*FEDNUF*/
#ifdef __cplusplus
}
#endif
#endif
