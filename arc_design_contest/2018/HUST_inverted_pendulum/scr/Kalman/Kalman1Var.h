
/*
 * Kalman1Var.h
 *
 *  Created on: Aug 4, 2016
 *      Author: loywong
 */


#ifndef KALMAN_KALMAN1VAR_H_
#define KALMAN_KALMAN1VAR_H_

typedef struct
{
    float A, B, H, Q, R;
    float xpri, Ppri;
    float K, xpos, Ppos;
}Kalman1Var;

void Predict(Kalman1Var *kalVal, float u);
float Correct(Kalman1Var *kalVal, float z);
void kalReset(Kalman1Var *kalVal);

#endif /* KALMAN_KALMAN1VAR_H_ */