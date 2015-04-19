/*
 * train_set.h
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

#ifndef BPANN_CLION_TRAIN_SET_H
#define BPANN_CLION_TRAIN_SET_H

#include"../comm.h"

typedef struct {
    int test_flag;
    double *input;
    double *target;
} Data;

typedef struct {
    Data *_list;
    int dim_input;
    int dim_target;
    int num_list;
} TrainingSet;

TrainingSet *training_set_creat(int num_list, int dim_input, int dim_target) {
    TrainingSet *set = (TrainingSet *) malloc(sizeof(TrainingSet));
    JUDGE(set);
    set->dim_input = dim_input;
    set->dim_target = dim_target;
    set->num_list = num_list;
    set->_list = (Data *) malloc(num_list * sizeof(Data));
    JUDGE(set->_list);
    int i = 0;
    while (i < num_list) {
        set->_list[i].test_flag = 0;
        set->_list[i].input = (double *) malloc(sizeof(double) * dim_input);
        JUDGE(set->_list[i].input);
        set->_list[i].target = (double *) malloc(sizeof(double) * dim_target);
        JUDGE(set->_list[i].target);
        i++;
    }
    return set;
}

void training_set_free(TrainingSet **p_set) {
    TrainingSet *set = *p_set;
    int i = 0;
    while (i < set->num_list) {
        free(set->_list[i].input);
        free(set->_list[i].target);
        i++;
    }
    free(set->_list);
    free(set);
    set = NULL;
}


#endif //BPANN_CLION_TRAIN_SET_H
