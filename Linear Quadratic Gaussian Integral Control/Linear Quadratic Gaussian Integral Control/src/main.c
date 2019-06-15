/*
 ============================================================================
 Name        : Linear.c
 Author      : Daniel Mårtensson
 Version     :
 Copyright   : MIT
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "Linear_Quadratic_Gaussian_Integral_Regulator.h"

/*
 * Declare the dimension of square A matrix.
 * Dimension of output vector y.
 * Dimension of reference vector r.
 * Length of integral matrix xn;
 */
#define adim 10
#define ydim 1
#define rdim 1
#define xn 1

/*
 * Print decleration
 */
void print(float* A, int row, int column);

int main() {

	/*
	 * Model A, B, C, D
	 */
	const float A[adim * adim] = { 0.99500201,  -0.00656006,  -0.00324621,   0.00284745,   0.00474606,   0.00299756,   0.00053557,   0.00510325,   0.00195878,   0.00103930,
								   0.00656006,   0.99283815,  -0.02960128,   0.00526632,   0.01514675,   0.00524905,   0.01298864,   0.00695144,   0.01129487,   0.00493022,
								  -0.00324621,   0.02960128,   0.99274160,   0.02925795,   0.03278408,   0.02712715,  -0.01623747,   0.03996273,   0.00092537,   0.00275946,
								  -0.00284745,   0.00526632,  -0.02925795,   0.98010080,  -0.11692287,   0.00548336,  -0.13731362,   0.00838655,  -0.02404120,  -0.00952090,
								   0.00474606,  -0.01514675,   0.03278408,   0.11692287,   0.13819930,  -0.75274525,   0.61524332,  -0.08864197,   0.03148846,   0.01481154,
								  -0.00299756,   0.00524905,  -0.02712715,   0.00548336,   0.75274525,  -0.33361173,  -0.56018800,   0.01121465,  -0.02978725,  -0.00793863,
								   0.00053557,  -0.01298864,  -0.01623747,   0.13731362,   0.61524332,   0.56018800,   0.49924238,  -0.16479061,  -0.02304370,  -0.00928901,
								  -0.00510325,   0.00695144,  -0.03996273,   0.00838655,   0.08864197,   0.01121465,   0.16479061,   0.95896684,  -0.09604525,  -0.03548706,
								   0.00195878,  -0.01129487,   0.00092537,   0.02404120,   0.03148846,   0.02978725,  -0.02304370,   0.09604525,   0.77327414,   0.61147580,
								  -0.00103930,   0.00493022,  -0.00275946,  -0.00952090,  -0.01481154,  -0.00793863,   0.00928901,  -0.03548706,  -0.61147580,   0.78960433};
	const float B[adim * rdim] ={ -0.473079,
								   0.246331,
								  -0.174253,
								  -0.121266,
								   0.218141,
								  -0.127719,
								   0.051346,
								  -0.214983,
								   0.105775,
								  -0.055904};
	const float C[ydim * adim] = {-0.0126102,  -0.0065661,  -0.0046448,   0.0032324,   0.0058147,   0.0034044,   0.0013687,   0.0057305,   0.0028195,   0.0014901};
	const float D[ydim * rdim] = { 0 };

	/*
	 * Kalman steady state matrix K
	 */
	const float K[adim * ydim] = {-0.605326,
								  -0.304048,
								  -0.326261,
								  -0.011849,
								  -0.055601,
								   0.171333,
								   0.392499,
								   0.167388,
								   0.268947,
								   0.014662};
	/*
	 * Control law L
	 */
	const float L[rdim * adim] = {-0.748569,   0.336152,  -0.236959,  -0.072548,  -0.077530,  -0.558029,   0.771664,   0.406676,   0.483614,  -0.698279};

	/*
	 * Integral law Li
	 */
	const float Li[rdim * xn] = {0.39778};

	/*
	 * Initial states - Normally set to zero
	 */
	float x[adim * 1] = {0};
	float xi[xn * 1] = {0};

	/*
	 * Input signal, reference signal, output signal and state vector x and xi
	 */
	float qi = 0.1;		 // <-- Tune this in between 0->1, but qi =/= 1.
	float u[rdim * 1] = {0}; // <-- This is our input value we want to have
	float r[rdim * 1] = {25}; // <-- This is what we want to see.
	float y[ydim * 1] = {0}; // <-- This is our measurement. Set it.

	/*
	 * Anti-windup
	 * 0 = Integrate xi when r < y and r > y
	 * 1 = Delete xi when r < y, else integrate when r > y
	 * 2 = Only integrate xi when r < y, but stop integrate when r > y
	 */
	int anti_windup = 0;

	/*
	 * Initial arrays set - Control LQGI
	 */
	for(int i = 0; i < 1001; i++)
		LQGI(adim, ydim, rdim, xn, A, B, C, D, L, Li, K, u, r, y, x, xi, qi, anti_windup);

	print(u, rdim, 1);


	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	return EXIT_SUCCESS;
}

void print(float* A, int row, int column) {
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			printf("%0.18f ", *(A++));
		}
		printf("\n");
	}
	printf("\n");
}
