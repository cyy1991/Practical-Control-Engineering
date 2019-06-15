/*
 * Linear_Quadratic_Gaussian_Integral_Regulator.c
 *
 *  Created on: 4 juni 2019
 *      Author: Daniel Mårtensson
 */

#include "Linear_Quadratic_Gaussian_Integral_Regulator.h"

/*
 * Parameters
 */
int adim;
int ydim;
int rdim;
int xn;

/*
 * Deceleration
 */
static void integral_law(float* xi, float* r, float* y, int anti_windup);
static void control_law(float* u, float qi, float* r, const float* L, const float* Li, float* x, float* xi);
static void kalman(float* x, const float* A, const float* B, float* u, const float* K, float* y, const float* C);
static void mul(float* A, float* B, float* C, int row_a, int column_a, int column_b);
//static void print(const float* A, int row, int column);

/*
 * Constructor
 */
void LQGI(int adim_, int ydim_, int rdim_, int xn_, const float* A, const float* B, const float* C, const float* D, const float* L, const float* Li, const float* K, float* u, float* r, float* y, float* x, float* xi, float qi, int anti_windup){
	/*
	 * Set the dimensions
	 */
	adim = adim_;
	ydim = ydim_;
	rdim = rdim_;
	xn = xn_;

	/*
	 * Find the output u
	 */
	control_law(u, qi, r, L, Li, x, xi);

	/*
	 * Update the integral xi state
	 */
	integral_law(xi, r, y, anti_windup);

	/*
	 * Estimate the state vector
	 */
	kalman(x, A, B, u, K, y, C);

}

/*
 * xi = xi + r - y;
 */
static void integral_law(float* xi, float* r, float* y, int anti_windup) {
	for(int i = 0; i < rdim; i++){
		/*
		 * Anti-windup
		 */
		if(anti_windup == 0){
			*(xi + i) = *(xi + i) + *(r + i) - *(y + i); // Always integrate
		}else if(anti_windup == 1){
			if(*(r + i) > *(y + i)){
				*(xi + i) = *(xi + i) + *(r + i) - *(y + i); // Only integrate when r > y, else delete
			}else{
				for(int j = 0; j < rdim; j++){
					*(xi + j) = 0; // Delete xi
				}
			}
		}else if(anti_windup == 2){
			if(*(r + i) > *(y + i)){
				*(xi + i) = *(xi + i) + *(r + i) - *(y + i); // Only integrate r > y, else stop
			}
		}else{
			*(xi + i) = *(xi + i) + *(r + i) - *(y + i); // Always integrate if nothing else selected
		}
	}
}

/*
 * u = Li/(1-qi)*r - (L*x - Li*xi);
 * Li/(1-qi) = Feed forward reference gain for discrete system. qi = integral speed
 */
static void control_law(float* u, float qi, float* r, const float* L, const float* Li, float* x, float* xi) {
	/*
	 * Compute vectors L_vec = L*x and Li_vec = Li*xi
	 */
	float L_vec[rdim*1];
	memset(L_vec, 0, rdim*sizeof(float));
	mul((float*) L, x, L_vec, rdim, adim, 1);
	float Li_vec[rdim*1];
	memset(Li_vec, 0, rdim*sizeof(float));
	mul((float*) Li, xi, Li_vec, rdim, xn, 1);

	for(int i = 0; i < rdim; i++){
		*(u + i) = *(Li + i)/(1-qi) * *(r + i) - (*(L_vec + i) - *(Li_vec + i));
	}

}

/*
 *   x = Ax - KCx + Bu + Ky % Kalman filter
 */
static void kalman(float* x, const float* A, const float* B, float* u, const float* K, float* y, const float* C) {
	/*
	 * Compute the vector A_vec = A*x
	 */
	float A_vec[adim*1];
	memset(A_vec, 0, adim*sizeof(float));
	mul((float*) A, x, A_vec, adim, adim, 1);

	/*
	 * Compute the vector B_vec = B*u
	 */
	float B_vec[adim*1];
	memset(B_vec, 0, adim*sizeof(float));
	mul((float*) B, u, B_vec, adim, rdim, 1);

	/*
	 * Compute the vector C_vec = C*x
	 */
	float C_vec[ydim*1];
	memset(C_vec, 0, ydim*sizeof(float));
	mul((float*) C, x, C_vec, ydim, adim, 1);

	/*
	 * Compute the vector KC_vec = K*C_vec
	 */
	float KC_vec[adim*1];
	memset(KC_vec, 0, adim*sizeof(float));
	mul((float*) K, C_vec, KC_vec, adim, ydim, 1);

	/*
	 * Compute the vector Ky_vec = K*y
	 */
	float Ky_vec[adim*1];
	memset(Ky_vec, 0, adim*sizeof(float));
	mul((float*) K, y, Ky_vec, adim, ydim, 1);

	/*
	 * Now add x = A_vec - KC_vec + B_vec + Ky_vec
	 */
	for(int i = 0; i < adim; i++){
		*(x + i) = *(A_vec + i) - *(KC_vec + i) + *(B_vec + i) + *(Ky_vec + i);
	}

}

/*
 * C = A*B
 */
static void mul(float* A, float* B, float* C, int row_a, int column_a, int column_b) {

	// Data matrix
	float* data_a = A;
	float* data_b = B;

	for (int i = 0; i < row_a; i++) {
		// Then we go through every column of b
		for (int j = 0; j < column_b; j++) {
			data_a = &A[i * column_a];
			data_b = &B[j];

			*C = 0; // Reset
			// And we multiply rows from a with columns of b
			for (int k = 0; k < column_a; k++) {
				*C += *data_a * *data_b;
				data_a++;
				data_b += column_b;
			}
			C++; // ;)
		}
	}
}

/*
 * Print matrix or vector - Just for error check

static void print(const float* A, int row, int column) {
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			printf("%0.18f ", *(A++));
		}
		printf("\n");
	}
	printf("\n");
}
*/

