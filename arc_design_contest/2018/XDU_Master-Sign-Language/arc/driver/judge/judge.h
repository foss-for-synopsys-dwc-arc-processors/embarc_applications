/***vdtwrecoge.h*********************************************************************/
#ifndef _judge_H_
#define _judge_H_

#include<stdio.h>
#include<math.h>

#define DTWMAXNUM 2000
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))
#define ABS(a) ((a)>0?(a):(-(a)))
#define DTWVERYBIG 100000000.0
#define MAX_DIM 13
struct VECTOR
{
	int num;	
	double  data[MAX_DIM];
};

typedef struct VECTOR  mvector;
typedef struct VECTOR* pmvector;

/*****************************************************************************/
/*VectorMul(mvector *A,double mul) 
/* 
/*****************************************************************************/
extern void VectorMul(mvector *A,double mul);


/*****************************************************************************/
/*VectorInit(mvector *A,int dim) 
/* dim 
/******************************************************************************/

extern void VectorInit(mvector *A,int dim);

/*****************************************************************************/
/* VectorAdd(mvector *A,mvector *B,mvector *C)
/* 
/*****************************************************************************/
extern void VectorAdd(mvector *A,mvector *B,mvector *C);
/*****************************************************************************/
/* VectorDistance,
/* 
/*****************************************************************************/
extern double VectorDistance(mvector *A,mvector *B);


extern double VDTWDistanceFun(mvector *A,int I,mvector *B,int J,int r);

/*****************************************************************************/

/*****************************************************************************/
extern int VDTWTemplate(mvector *A,int I,mvector *B,int J,mvector *temp,int turn,double tt,double *rltdistance);
extern uint8_t  tst_beg(double test_buffer[13],double aValue[3],double wValue[3],double AValue[3],int adc[4]);
extern int VDTWD(mvector *TEST,int J,int r);
extern int judge(double aValue[3],double wValue[3],double AValue[3],int adc[4],int cmd);
#endif
