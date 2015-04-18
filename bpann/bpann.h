/*
 * bpann.h
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
 */

#ifndef BPANN_CLION_BPANN_H
#define BPANN_CLION_BPANN_H

#include"../comm.h"

/*
*	In this neural network data structure,	The network is assumed to
*	be a fully-connected feed-forward n-layer network.
*	If you want create this neural network,	you need input the following
*	four variable:
*			layer_n:		which type is integer,	means there n layers in network.
*			layer:			which type is array,	means the units in each layer.
*			learning_rate:	which type is double,	means...	as the words says.
*			impulses:	   	which type is double,	means...	as the words says.
*
*	such example like:
*			3,	{3,8,2},	0.02,	0.01
*
*/
typedef struct
{
    int layer_n;					// layers in net
    int *layer;						// the units in each layer

    double **layer_units;			// values in each units

    //double *input;					// training input
    //double *target;					// target output

    double **layer_units_errors;	// each units error except input layer

    double ***layer_weights;		// the weights between each adjacent units

    double ***layer_prev_weights;	// the prev weights between each adjacent units

    double learning_rate;
    double impulses;
}Bpann;

void bpann_init();

Bpann *bpann_create(int layer_n,int *layer,double learning_rate,double impulses);
void bpann_free(Bpann **p_bp);

void bpann_train();
void bpann_feed_forward();

void bpann_save(Bpann *bp,char *file_name);
Bpann *bpann_load(char *file_name);

/******************
*	get random number between 0.0 and 1.0
********************/
double Load_Random_0_to_1(){
    return (double)rand()/(double)RAND_MAX;
}

/******************
*	get random number between -1.0 and 1.0
********************/
double Load_Random_Minus_1_to_1()
{
    return Load_Random_0_to_1()*2.0-1.0;
}

/*
*	that is kernel function of Bpann
*/
double logistic(double x)
{
    return (1.0/(1.0+exp(-x)));
}

#define FASTCOPY(to,from,len) \
{\
	register char *_to=(char*)(to);	\
	register char *_from=(char*)(from);	\
	register int _i,_len;	\
	_len=(len);	\
	for(_i=0;_i<_len;++_i)	\
		*(_to++)=*(_from++);	\
}\

double * alloc_1d(int size)
{
    double *tmp=(double*)malloc(size*sizeof(double));
    JUDGE(tmp);
    return tmp;
}

double ** alloc_2d(int *array,int size)
{
    double ** tmp=(double**)malloc(size*sizeof(double*));
    JUDGE(tmp);
    int i=0;
    for(;i<size;++i)
    {
        tmp[i]=alloc_1d(array[i]);
    }
    return tmp;
}

double ** alloc_std_2d(int row,int col)
{
    double **tmp=(double**)malloc(row*sizeof(double*));
    JUDGE(tmp);
    int i=0;
    for(;i<row;++i)
    {
        tmp[i]=alloc_1d(col);
    }
    return tmp;
}

double *** alloc_3d_weight(int *layer,int layer_n)
{
    double ***tmp=(double***)malloc((layer_n-1)*sizeof(double**));
    JUDGE(tmp);
    int i;
    for(i=0;i<layer_n-1;++i)
    {
        tmp[i]=alloc_std_2d(layer[i],layer[i+1]);
    }
    return tmp;
}

Bpann *Bpann_create(int layer_n,int *layer,double learning_rate,double impulses)
{
    Bpann *bp=(Bpann*)malloc(sizeof(Bpann));
    if(NULL==bp){
        printf("Bpann_create: can't alloc memory !\n");
        return 0;
    }
    int i=0,j=0,k=0;

    /*
    *	set learning rate and impulses in Bpann
    */
    bp->learning_rate=learning_rate;
    bp->impulses=impulses;


    bp->layer_n=layer_n;

    bp->layer = (int*)malloc(layer_n*sizeof(int));

    for(i=0;i<layer_n;++i){
        bp->layer[i]=layer[i];
    }

    /*
    *	alloc layer units and init it's value
    */
    bp->layer_units=alloc_2d(layer,layer_n);

    for(i=0;i<layer_n;++i)
    {
        for(j=0;j<layer[i];++j){
            bp->layer_units[i][j]=0;
        }
    }

    /*
    *	this could be unnecessary if needed.
    */
    //	bp->input=alloc_1d(layer[0]);
    //	bp->target=alloc_1d(layer[layer_n-1]);

    /*
    *	alloc each units's error and init it's value;
    *	except input units. so i start with 1
    */
    bp->layer_units_errors=alloc_2d(layer,layer_n);

    for(i=0;i<layer_n;++i)
    {
        for(j=0;j<layer[i];++j){
            bp->layer_units_errors[i][j]=0;
        }
    }

    /*
    *	alloc layer weights
    */
    bp->layer_weights=alloc_3d_weight(layer,layer_n);
    /*
    *	init random layer weight
    */
#ifdef INIT_ZERO_INPUT_WEIGHT
	for(j=0;j<layer[0];++j)
	{
		for(k=0;k<layer[1];++k)
		{
			bp->layer_weights[0][j][k]=0;
		}
	}
	for(i=1;i<layer_n-1;++i)
	{
		for(j=0;j<layer[i];++j)
		{
			for(k=0;k<layer[i+1];++k)
			{
	#ifdef INIT_ZERO_WEIGHT
				bp->layer_weights[i][j][k]=0;
	#else
				bp->layer_weights[i][j][k]=Load_Random_Minus_1_to_1();
	#endif
			}
		}
	}
#else
    for(i=0;i<layer_n-1;++i)
    {
        for(j=0;j<layer[i];++j)
        {
            for(k=0;k<layer[i+1];++k)
            {
#ifdef INIT_ZERO_WEIGHT
				bp->layer_weights[i][j][k]=0;
	#else
                bp->layer_weights[i][j][k]=Load_Random_Minus_1_to_1();
#endif
            }
        }
    }
#endif

    /*
    *	alloc layer prev weights
    */
    bp->layer_prev_weights=alloc_3d_weight(layer,layer_n);
    /*
    *	init layer prev weights
    */
    for(i=0;i<layer_n-1;++i)
    {
        for(j=0;j<layer[i];++j)
        {
            for(k=0;k<layer[i+1];++k)
            {
#ifdef INIT_ZERO_PREV_WEIGHT
				bp->layer_prev_weights[i][j][k]=0;
#else
                bp->layer_prev_weights[i][j][k]=bp->layer_weights[i][j][k];
#endif
            }
        }
    }

    return bp;
}

void bpann_free(Bpann **p_bp)
{
    //free(bp->input);
    //free(bp->target);

    Bpann *bp = *p_bp;

    int i=0,j=0;
    for(i=0;i<bp->layer_n-1;++i)
    {
        free(bp->layer_units[i]);
        free(bp->layer_units_errors[i]);
        for(j=0;j<bp->layer[i];++j)
        {
            free(bp->layer_weights[i][j]);
            free(bp->layer_prev_weights[i][j]);
        }
        free(bp->layer_prev_weights[i]);
        free(bp->layer_weights[i]);
    }

    free(bp->layer_units[bp->layer_n-1]);
    free(bp->layer_units_errors[bp->layer_n-1]);

    free(bp->layer_units);
    free(bp->layer_units_errors);
    free(bp->layer_weights);
    free(bp->layer_prev_weights);

    free(bp->layer);

    free(bp);
    bp = NULL;
}

void feed_forward(Bpann *bp,double *input)
{
    int i=0,j=0,k=0;
    for(;j<bp->layer[0];++j)
    {
        bp->layer_units[0][j]=input[j];
    }

    for(i=1;i<bp->layer_n;++i)
    {
        for(j=0;j<bp->layer[i];++j)
        {
            /*
            *	peek prev layer
            */
            double sum=0.0;
            for(k=0;k<bp->layer[i-1];++k)
            {
                double weights_k_to_j=bp->layer_weights[i-1][k][j];
                double units_prev=bp->layer_units[i-1][k];
                sum+=(weights_k_to_j*units_prev);
            }
            bp->layer_units[i][j]=logistic(sum);
        }
    }
}

void bpann_save(Bpann *bp, char *filepath)
{
#ifdef WINDOWS
    FILE *file;
    fopen_s(&file, filepath, "wb");
#else
    FILE * file=fopen(filepath,"wb");
#endif
    if(NULL==file)
    {
        fprintf(stderr, "Function Bpann_save: can't creat %s file.\n",filepath);
        exit(-1);
    }
    fwrite(&(bp->layer_n),sizeof(int),1,file);
    fwrite(bp->layer,sizeof(int),bp->layer_n,file);

    fwrite(&(bp->learning_rate),sizeof(double),1,file);
    fwrite(&(bp->impulses),sizeof(double),1,file);

    int i,j;
    for(i=0;i<bp->layer_n-1;++i)
    {
        for(j=0;j<bp->layer[i];++j)
        {
            fwrite(bp->layer_weights[i][j],sizeof(double),bp->layer[i+1],file);
        }
    }
    fclose(file);
}

Bpann *bpann_load(char *filepath)
{
#ifdef WINDOWS
    FILE *file;
    fopen_s(&file, filepath, "rb");
#else
    FILE * file=fopen(filepath,"rb");
#endif
    if(NULL==file)
    {
        fprintf(stderr, "Function Bpann_load: can't open %s file.\n",filepath );
        exit(-1);
    }
    int layer_n;
    fread(&layer_n,sizeof(int),1,file);
    int *layer=(int*)malloc(layer_n*sizeof(int));
    JUDGE(layer);
    fread(layer,sizeof(int),layer_n,file);
    double learning_rate,impulses;
    fread(&learning_rate,sizeof(double),1,file);
    fread(&impulses,sizeof(double),1,file);

#define INIT_ZERO_WEIGHT
#define INIT_ZERO_PREV_WEIGHT
    Bpann *new_bp=bpann_create(layer_n,layer,learning_rate,impulses);
#undef INIT_ZERO_PREV_WEIGHT
#undef INIT_ZERO_WEIGHT

    int i,j;
    for(i=0;i<layer_n-1;++i)
    {
        for(j=0;j<layer[i];++j)
        {
            fread(new_bp->layer_weights[i][j],sizeof(double),layer[i+1],file);
        }
    }
    fclose(file);
    return new_bp;
}

void compute_error(Bpann *bp,double *target)
{
    int i=0,j=0,k=0;
    for(;j<bp->layer[bp->layer_n-1];++j)
    {
        double output_j=bp->layer_units[bp->layer_n-1][j];
        bp->layer_units_errors[bp->layer_n-1][j]=output_j*(1-output_j)*(target[j]-output_j);
    }
    for(i=bp->layer_n-2;i>=0;--i)
    {
        for(j=0;j<bp->layer[i];++j)
        {
            double sum=0.0;
            for(k=0;k<bp->layer[i+1];++k)
            {
                double weight_next=bp->layer_weights[i][j][k];
                double units_error_next=bp->layer_units_errors[i+1][k];
                sum+=(weight_next*units_error_next);
            }
            double units_j=bp->layer_units[i][j];
            bp->layer_units_errors[i][j] = units_j*(1 - units_j)*sum;
        }
    }
}

void modify_weights(Bpann *bp)
{
    int i,j,k;
    for(i=0;i<bp->layer_n-1;++i)
    {
        for(j=0;j<bp->layer[i];++j)
        {
            for(k=0;k<bp->layer[i+1];++k)
            {
                double units_prev=bp->layer_units[i][j];
                double units_error_next=bp->layer_units_errors[i+1][k];

                double tmp=bp->layer_weights[i][j][k];
                double prev_weight_delta=tmp-bp->layer_prev_weights[i][j][k];

                bp->layer_weights[i][j][k]+=(bp->learning_rate*units_prev*units_error_next+bp->impulses*prev_weight_delta);
                bp->layer_prev_weights[i][j][k]=tmp;
            }
        }
    }
}

double judge_performance(Bpann *bp,double *input,double *target)
{
    feed_forward(bp,input);

    double sum=0.0;
    int i;
    for(i=0;i<bp->layer[bp->layer_n-1];++i)
    {
        double tmp=target[i]-bp->layer_units[bp->layer_n-1][i];
        sum+=tmp*tmp;
    }
    return exp(-sum);
}

void once_training(Bpann *bp,double *input,double *target)
{
    feed_forward(bp,input);
    compute_error(bp,target);
    modify_weights(bp);
}

#endif //BPANN_CLION_BPANN_H
