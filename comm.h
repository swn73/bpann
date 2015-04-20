/*
 * comm.h
 *
 * Copyright 2015 Hualin Xia <hua_lin@live.cn>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Only one thing was needed,	which is the author's info was not allowed
 * been DELETED.
 *
 */

#ifndef BPANN_COMM_H
#define BPANN_COMM_H

//#define WINDOWS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ERROR_ALLOC() {\
    printf("can't alloc memory !\n"); \
    exit(-1); \
}

#define JUDGE(x) {\
if (NULL == (x)){ ERROR_ALLOC(); }\
}

/******************
*	get random number between 0.0 and 1.0
********************/
#define RANDOM_0_to_1()  (double) rand() / (double) RAND_MAX

/******************
*	get random number between -1.0 and 1.0
********************/
#define RANDOM_minus_1_to_1()   RANDOM_0_to_1() * 2.0 - 1.0

#define ABS(x)    (((x)>0.0)?(x):(-(x)))

#define FASTCOPY(to, from, len) \
{\
    register char *_to=(char*)(to);    \
    register char *_from=(char*)(from);    \
    register int _i,_len;    \
    _len=(len);    \
    for(_i=0;_i<_len;++_i)    \
        *(_to++)=*(_from++);    \
}\

#endif //BPANN_COMM_H
