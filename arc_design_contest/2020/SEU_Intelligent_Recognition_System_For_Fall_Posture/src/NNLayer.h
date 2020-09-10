
#ifndef _NNLAYER_H_
#define _NNLAYER_H_
 
//#define LAYER_TOTAL         2
//#define LAYER_0_INDIM       3
//#define LAYER_0_OUTDIM      3
//#define LAYER_1_INDIM       3
//#define LAYER_1_OUTDIM      1

//#define LAYER_TOTAL         3
//#define LAYER_0_INDIM       40
//#define LAYER_0_OUTDIM      128
//#define LAYER_1_INDIM       128
//#define LAYER_1_OUTDIM      64 
//#define LAYER_2_INDIM       64
//#define LAYER_2_OUTDIM      5 

#define LAYER_TOTAL         2
#define LAYER_0_INDIM       20
#define LAYER_0_OUTDIM      32
#define LAYER_1_INDIM       32
#define LAYER_1_OUTDIM      2 

#define EPOCH               5000
#define LR                  0.0000001
#define BATCH_SIZE          10

//#define LAYER_TOTAL         2
//#define LAYER_0_INDIM       2
//#define LAYER_0_OUTDIM      2
//#define LAYER_1_INDIM       2
//#define LAYER_1_OUTDIM      3 
 

//#include "../common.h"
 
/*
*   NNLayer variable definition
*/
typedef struct 
{
    double* weight;             //Weight two-dimensional matrix  INDIM * OUTDIM
    double* bais;               //Offset array      1 * OUTDIM
    int inDim;                  //The input dimension of
    int outDim;                 //The output of dimension
    double* outVal;             //The output array
}NNLayer;
 
 
void NNLayerInit(void);
void NNLayerPredict(NNLayer* model,double* x);

double NNLayerloss(NNLayer* model,double* target);
void cal_dp_outlayer(NNLayer* model,double* target, double(*dp_w1)[LAYER_0_OUTDIM],double* dp_b1);
void cal_dp_hidlayer(NNLayer* model,double* x,double* target, double(*dp_w1)[LAYER_0_OUTDIM],double* dp_b1, double(*dp_w0)[LAYER_0_INDIM],double* dp_b0);
void w_b_upgrate(NNLayer* model, double(*dp_w1)[LAYER_0_OUTDIM],double* dp_b1, double(*dp_w0)[LAYER_0_INDIM],double* dp_b0);
 
#endif
