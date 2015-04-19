/*
 * image.h
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

#ifndef BPANN_CLION_IMAGE_H
#define BPANN_CLION_IMAGE_H

#include"../comm.h"

typedef struct {
    char *name;
    int row, col;
    int *data;
} Image;

/***	user accessible macros	***/
#define ROW(img)    ((img)->row)
#define COL(img)    ((img)->col)
#define NAME(img)    ((img)->name)

/***	user accessible functions	***/
Image *img_open();

Image *img_creat();

void img_setpixel();

int img_getpixel();

void img_free();


#define RGB_TO_GREY(r, g, b)    ((r)+(g)+(b))/3

Image *Image_alloc(int cols, int rows) {
    Image *img = (Image *) malloc(sizeof(Image));
    img->name = NULL;
    img->col = cols;
    img->row = rows;
    img->data = (int *) malloc(sizeof(int) * (ROW(img) * COL(img)));
    return img;
}


char *Get_name(char *filepath);

Image *Imread_ppm_P6(char *filepath);

Image *Imread_ppm_P3(char *filepath);

Image *Imread_pgm_P5(char *filrname);

Image *Imread_pgm_P2(char *filepath);

Image *Imread_pbm_P4(char *filepath);

Image *Imread_pbm_P1(char *filepath);

#define SET_PIXEL(img, rw, cl, e)    \
{\
    (img)->data[(img)->col*((rw)-1) + (cl)] = e; \
}

#define GET_PIXEL(img, rw, cl)    (img)->data[(img)->col*((rw)-1)+(cl)]

/*
    Type	            Magic number    	Magic number	Extension	Colors
    Portable BitMap	    P1	ASCII	        P4	binary	    .pbm	    0–1 (black & white)
    Portable GrayMap    P2	ASCII	        P5	binary	    .pgm	    0–255 (gray scale)
    Portable PixMap     P3	ASCII	        P6	binary	    .ppm	    0–255 (RGB)

*/

Image *Imread_PNM(char *filepath) {
#define BUF_SIZE 16
    char buf[BUF_SIZE];
    int type;

    FILE *file = fopen(filepath, "r");

    if (NULL == file) {
        fprintf(stderr, "Imread_PNM: can't open file %s.\n", filepath);
        exit(-1);
    }
    fgets(buf, BUF_SIZE, file);
    sscanf(buf, "P%d", &type);
    if (type > 6 || type < 1) {
        fprintf(stderr, "Imread_PNM: bad type:P%d", type);
        return NULL;
    }
    Image *img = NULL;
    switch (type) {
        case 1:
            img = Imread_pbm_P1(filepath);
            break;
        case 2:
            img = Imread_pgm_P2(filepath);
            break;
        case 3:
            img = Imread_ppm_P3(filepath);
            break;
        case 4:
            img = Imread_pbm_P4(filepath);
            break;
        case 5:
            img = Imread_pgm_P5(filepath);
            break;
        case 6:
            img = Imread_ppm_P6(filepath);
            break;
        default :
            // nothing to do
            break;
    }
#undef BUF_SIZE
    return img;
}

char *Get_name(char *filepath) {
#ifdef WINDOWS
    char seq = '\\';
#else
    char seq = '/';
#endif
    char *part = filepath;
    while (*part != '\0') {
        part++;
    }
    int len = part - filepath;
    while (part >= filepath && *part != seq) {
        part--;
    }
    part = part + 1;
    char *filename = (char *) malloc((len + 1) * sizeof(char));
    JUDGE(filename);
    int i = 0;
    while (*(part + i) != '\0') {
        *(filename + i) = *(part + i);
        i++;
    }
    *(filename + i) = '\0';
    return filename;
}

Image *Imread_ppm_P6(char *filepath) {
#ifdef WINDOWS
    FILE *file;
    fopen_s(&file, filepath, "rb");
#else
    FILE *file = fopen(filepath, "rb");
#endif
    if (NULL == file) {
        fprintf(stderr, "Imread_ppm: can't open file %s.\n", filepath);
        exit(-1);
    }
    char buf[128];
    fgets(buf, 128, file);
    fgets(buf, 128, file);
    int cl, rl;

#ifdef WINDOWS
    sscanf_s(buf, "%d %d", &cl, &rl);
#else
    sscanf(buf, "%d %d", &cl, &rl);
#endif

    Image *img = Image_alloc(cl, rl);
    NAME(img) = Get_name(filepath);
    fgets(buf, 128, file);

    int val_max;

#ifdef WINDOWS
    sscanf_s(buf, "%d", &val_max);
#else
    sscanf(buf, "%d", &val_max);
#endif
    if (val_max > 255) {
        fprintf(stderr, "only support pixel value lowwer 255. here is %d\n", val_max);
        return NULL;
    }
    int i, j;
    for (i = 0; i < rl; ++i) {
        for (j = 0; j < cl; ++j) {
            int r, g, b;
            r = fgetc(file);
            g = fgetc(file);
            b = fgetc(file);
            SET_PIXEL(img, j, i, RGB_TO_GREY(r, g, b));
        }
    }
    return img;
}


#endif //BPANN_CLION_IMAGE_H
