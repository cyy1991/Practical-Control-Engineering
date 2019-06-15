/*
 * Linear_Quadratic_Gaussian_Integral_Regulator.h
 *
 *  Created on: 4 juni 2019
 *      Author: Daniel Mårtensson
 */

#include <string.h>

#ifndef LINEAR_QUADRATIC_GAUSSIAN__INTEGRAL_REGULATOR_H_
#define LINEAR_QUADRATIC_GAUSSIAN__INTEGRAL_REGULATOR_H_

void LQGI(int adim_, int ydim_, int rdim_, int xn_, const float* A, const float* B, const float* C, const float* D, const float* L, const float* Li, const float* K, float* u, float* r, float* y, float* x, float* xi, float qi, int anti_windup);

#endif /* LINEAR_QUADRATIC_GAUSSIAN__INTEGRAL_REGULATOR_H_ */
