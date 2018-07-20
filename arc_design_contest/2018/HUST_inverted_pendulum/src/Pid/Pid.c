#include "Pid.h"

#define USEI
#define USED

void pidParamCorr(Pid *pid, float p, float i, float d, float n)
{
    pid->p = p;
    pid->i = i;
    pid->d = d;
    pid->n = n;
    pid->accI = .0f;
    pid->accD = .0f;
}

void pidReset(Pid *pid)
{
    pid->accI = .0f;
    pid->accD = .0f;
}

float Tick(Pid *pid, float diff)
{
    float pout;
#ifdef USED
    float dout;
#endif
    
    pout = diff * pid->p;

#ifdef USEI    
    pid->accI += diff * pid->i * pid->ts;
    if(pid->accI > pid->accIMax)
        pid->accI = pid->accIMax;
    else if(pid->accI < pid->accIMin)
        pid->accI = pid->accIMin;
#endif

#ifdef USED
    dout = (diff * pid->d - pid->accD) * pid->n;    
    pid->accD += dout * pid->ts;
    if(pid->accD > pid->accDMax)
        pid->accD = pid->accDMax;
    else if(pid->accD < pid->accDMin)
        pid->accD = pid->accDMin;
#endif
    
#ifdef USEI
    pout += pid->accI;
#endif
#ifdef USED
    pout += dout;
#endif

    if(pout > pid->accIMax)
        pout = pid->accIMax;
    else if(pout < pid->accIMin)
        pout = pid->accIMin;
    
    return pout;
}