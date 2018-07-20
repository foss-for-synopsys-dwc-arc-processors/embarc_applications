#ifndef __PID_H__
#define __PID_H__

typedef struct
{
	float p, i, d, n, ts;
    float accI, accD, accIMax, accIMin, accDMax, accDMin;
}Pid;

void pidParamCorr(Pid *pid, float p, float i, float d, float n);
float Tick(Pid *pid, float diff);
void pidReset(Pid *pid);

#endif