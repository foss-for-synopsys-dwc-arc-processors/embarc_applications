#ifndef _LQR_H_
#define _LQR_H_

#include "physParams.h"

typedef struct 
{
	float k1, k2, k3, k4;
}lqr;

//void lqrParamCorr(lqr *lqr, lqrParam *K);
float calc(lqr *lqr, float pos, float vel, float ang, float av);

#endif /* LQR_H_ */