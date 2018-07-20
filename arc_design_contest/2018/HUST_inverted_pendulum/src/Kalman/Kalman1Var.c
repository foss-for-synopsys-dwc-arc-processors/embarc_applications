/*
 * Kalman1Var.cpp
 *
 *  Created on: Aug 4, 2016
 *      Author: loywong
 */


#include <Kalman/Kalman1Var.h>

void kalReset(Kalman1Var *kalVal)
{
    kalVal->xpos = 0.f;
    kalVal->Ppos = 0.f;
}

void kalParamCorr(Kalman1Var *kalVal, float r, float q, float x0, float P0)
{
    kalVal->Q = q;
    kalVal->R = r;
    kalVal->xpos = x0;
    kalVal->Ppos = P0;
}

void Predict(Kalman1Var *kalVal, float u)
{
//    xˆ -k = A xˆ k – 1 + B u k – 1
    kalVal->xpri = kalVal->A * kalVal->xpos + kalVal->B * u;
//    P -k = A P k – 1 A T + Q
    kalVal->Ppri = kalVal->A * kalVal->Ppos * kalVal->A + kalVal->Q;
}

float Correct(Kalman1Var *kalVal, float z)
{
//    K k = P -k H T ( H P -k H T + R ) – 1
    kalVal->K = kalVal->Ppri * kalVal->H /                                  \
                    (kalVal->H * kalVal->Ppri * kalVal->H + kalVal->R);
//    xˆ k = xˆ -k + K k ( z k – H xˆ -k )
    kalVal->xpos = kalVal->xpri + kalVal->K * (z - kalVal->H * kalVal->xpri);
//    P k = ( I – K k H ) P -k
    kalVal->Ppos = (1 - kalVal->K * kalVal->H) * kalVal->Ppri;

    return kalVal->xpos;
}