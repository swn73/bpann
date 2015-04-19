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


#define Abs(x)    (((x)>0.0)?(x):(-(x)))


#endif //BPANN_COMM_H
