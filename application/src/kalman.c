/*
 * kalman.c
 *
 *  Created on: 14 nov. 2023
 *      Author: sebas
 */

#include "kalman.h"
#include "math.h"

#define MAX_SIZE 2

static float_t S[MAX_SIZE][3], V[MAX_SIZE][3];
static float_t DIFF[3];

extern void kalman_reset(void)
{
	for (uint8_t i = 0; i < 3; i++)
	{
		DIFF[i] = 0;
		for (uint8_t j = 0; j < MAX_SIZE; j++)
		{
			S[j][i] = 0;
			V[j][i] = 0;
		}
	}
}

float_t roundff(float_t v, float_t n)
{
	static float_t nn;
	nn = powf(10.0, n);

	return ((int32_t)(v * nn))/nn;
}

extern position3D_t kalman_calcPosition(acc3D_t acc, float_t periodo)
{
	static position3D_t pos;
	static float_t p2;
	p2 = periodo * periodo;

	DIFF[0] = (acc.accX - DIFF[0]);
	DIFF[1] = (acc.accY - DIFF[1]);
	DIFF[2] = (acc.accZ - DIFF[2]);

	for (uint8_t i = 0; i < 3; i++)
	{
		V[0][i] = V[1][i];
		V[1][i] = V[0][i] + DIFF[i] * periodo;
		V[1][i] = roundff(V[1][i], 2);

		S[0][i] = S[1][i];
		S[1][i] = S[0][i]  + V[0][i] * periodo + (DIFF[i] * p2)/2 ;
		S[1][i] = roundff(S[1][i], 2);
	}

	pos.X = S[1][0];
	pos.Y = S[1][1];
	pos.Z = S[1][2];

	DIFF[0] = acc.accX;
	DIFF[1] = acc.accY;
	DIFF[2] = acc.accZ;

	return pos;
}





