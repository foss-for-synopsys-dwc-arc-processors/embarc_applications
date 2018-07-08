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
	int num;	//向量的维数
	double  data[MAX_DIM];
};

typedef struct VECTOR  mvector;
typedef struct VECTOR* pmvector;

/*****************************************************************************/
/*VectorMul(mvector *A,double mul) 向量数乘
/* 
/*****************************************************************************/
extern void VectorMul(mvector *A,double mul);


/*****************************************************************************/
/*VectorInit(mvector *A,int dim) 向量初始化
/* dim 维数
/******************************************************************************/

extern void VectorInit(mvector *A,int dim);

/*****************************************************************************/
/* VectorAdd(mvector *A,mvector *B,mvector *C)
/* 向量加法，结果存放在C中
/*****************************************************************************/
extern void VectorAdd(mvector *A,mvector *B,mvector *C);
/*****************************************************************************/
/* VectorDistance,用于求两个向量之间的距离，欧氏距离
/* A和B是待求距离的两个向量
/*****************************************************************************/
extern double VectorDistance(mvector *A,mvector *B);

/*****************************************************************************/
/* DTWDistance，求两个数组之间的匹配距离
/* A,B分别为第一第二个数组，I，J为其数组长度，r为匹配窗口的大小
/* r的大小一般取为数组长度的1/10到1/30
/* 返回两个数组之间的匹配距离,如果返回－1.0，表明数组长度太大了
/*****************************************************************************/
extern double VDTWDistanceFun(mvector *A,int I,mvector *B,int J,int r);

/*****************************************************************************/
/* DTWTemplate，进行建立模板的工作
/* 其中A为已经建立好的模板，我们在以后加入训练样本的时候，
/* 以已建立好的模板作为第一个参数，I为模板的长度，在这个模板中不再改变
/* B为新加入的训练样本，J为B的长度，turn为训练的次数，在第一次
/* 用两个数组建立模板时，r为1，这是出于权值的考虑
/* temp保存匹配最新训练后的模板，建议temp[DTWMAXNUM]，函数返回最新训练后模板的长度
/* 如果函数返回-1，表明训练样本之间距离过大，需要重新选择训练样本，
/* tt为样本之间距离的阈值，自行定义
/*****************************************************************************/
extern int VDTWTemplate(mvector *A,int I,mvector *B,int J,mvector *temp,int turn,double tt,double *rltdistance);
extern uint8_t  tst_beg(double test_buffer[13],double aValue[3],double wValue[3],double AValue[3],int adc[4]);
extern int VDTWD(mvector *TEST,int J,int r);
extern int judge(double aValue[3],double wValue[3],double AValue[3],int adc[4],int cmd);
#endif