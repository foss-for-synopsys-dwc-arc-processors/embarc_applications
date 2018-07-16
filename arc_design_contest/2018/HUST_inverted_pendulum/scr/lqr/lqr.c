#include "lqr.h"
#include "physParams.h"

// void lqrParamCorr(lqr *lqr, lqrParam *K)
// {
// 	lqr->k1 = K->k1;
// 	lqr->k2 = K->k2;
// 	lqr->k3 = K->k3;
// 	lqr->k4 = K->k4;
// }

float calc(lqr *lqr, float pos, float vel, float ang, float av)
{
	float vol;
	vol = lqr->k1 * pos + lqr->k2 * vel + lqr->k3 * ang + lqr->k4 * av;
	return vol;
}