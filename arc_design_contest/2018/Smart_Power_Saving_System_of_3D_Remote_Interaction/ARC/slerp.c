#include "slerp.h"
#include "embARC_debug.h"
void quaternion_norm(float* q)
{
  float* x = q;
  float* y = q + 1;
  float* z = q + 2;
  float* w = q + 3;
  float norm = sqrt((*x)*(*x) + (*y)*(*y) + (*z)*(*z) + (*w)*(*w));
  EMBARC_PRINTF("norm: %d", (int)(norm*10));
  *x = *x / norm;
  *y = *y / norm;
  *z = *z / norm;
  *w = *w / norm;
}


void quaternion_slerp(const float* q1, const float* q2, float* q, float lambda)
{
  float theta = 0 , st = 0, sut = 0, sout = 0, coeff1 = 0, coeff2 = 0;
  const float* x1 = q1;
  const float* y1 = q1 + 1;
  const float* z1 = q1 + 2;
  const float* w1 = q1 + 3;
  
  const float* x2 = q2;
  const float* y2 = q2 + 1;
  const float* z2 = q2 + 2;
  const float* w2 = q2 + 3;

  float dotproduct = (*x1) * (*x2) + (*y1) * (*y2) + (*z1) * (*z2) + (*w1) * (*w2);

  theta = (float)acos(dotproduct);

  if (theta < 0)
  {
    theta = -theta;
  }

  st = (float)sin(theta);
  sut = (float)sin(lambda*theta);
  sout = (float)sin((1 - lambda)*theta);
  coeff1 = sout / st;
  coeff2 = sut / st;

  float* x = q;
  float* y = q + 1;
  float* z = q + 2;
  float* w = q + 3;

  *(x) = coeff1 * *(x1)+coeff2 * *(x2);
  *(y) = coeff1 * *(y1)+coeff2 * *(y2);
  *(z) = coeff1 * *(z1)+coeff2 * *(z2);
  *(w) = coeff1 * *(w1)+coeff2 * *(w2);

  quaternion_norm(q);
}
