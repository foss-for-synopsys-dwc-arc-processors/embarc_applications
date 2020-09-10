
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

//#define LAYER_TOTAL         2
//#define LAYER_0_INDIM       2
//#define LAYER_0_OUTDIM      2
//#define LAYER_1_INDIM       2
//#define LAYER_1_OUTDIM      3 
 

//#include "../common.h"
 
/*
*   NNLayer��������
*/
typedef struct 
{
    const double* weight;             //Ȩ�ض�ά����  INDIM * OUTDIM
    const double* bais;               //ƫ������      1 * OUTDIM
    int inDim;                  //����ά��
    int outDim;                 //���ά��
    double* outVal;             //�������
}NNLayer;
 
 
void NNLayerInit(void);
void NNLayerPredict(NNLayer* model,double* x);
 

#endif
