#include <math.h>

void quaternion_norm(float* q);

void quaternion_slerp(const float* q1, const float* q2, float* q, float lambda);