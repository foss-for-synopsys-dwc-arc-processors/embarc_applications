#include <math.h>

/** \brief quarternion normalization 
*   \param q: quarternion pointer with the order of x, y, z, w
*/
void quaternion_norm(float* q);

/** \brief quarternion interporlation
*          The method are reference with this paper http://run.usc.edu/cs520-s15/assign2/p245-shoemake.pdf
*   \param q1: the input quarternion 1 pointer with the order of x, y, z, w
*   \param q2: the input quarternion 2 pointer with the order of x, y, z, w
*   \param q: the output quarternion pointer with the order of x, y, z, w
*   \param lambda: the interporlation ratio, which must between 0.0 ~ 1.0
*/
void quaternion_slerp(const float* q1, const float* q2, float* q, float lambda);