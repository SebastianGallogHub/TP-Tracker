/*
 * kalman.c
 *
 *  Created on: 14 nov. 2023
 *      Author: sebas
 */

#include "kalman.h"
#include "math.h"

//#define MAX_STEP 4294967295	//max de uint16_t ~6000 días
//#define MAX_STEP 2147483647	//max de uint16_t/2 ~3000 días
//#define MAX_STEP 4294967		//max de uint16_t/1000 ~6 días
//#define MAX_STEP 5

#define MAX_SIZE 2

#define ACC_BIAS 0.03 		//g
#define PERIOD_S 0.00125 	// muestreo cada 800Hz

static float_t Sx[MAX_SIZE], Sy[MAX_SIZE], Sz[MAX_SIZE]; //Posición
static float_t Vx[MAX_SIZE], Vy[MAX_SIZE], Vz[MAX_SIZE]; //Velocidad

extern void kalman_reset(void)
{
	for (uint64_t j = 0; j < MAX_SIZE; ++j)
	{
		Sx[j] = 0;
		Sy[j] = 0;
		Sz[j] = 0;

		Vx[j] = 0;
		Vy[j] = 0;
		Vz[j] = 0;
	}
}

extern position3D_t kalman_calcPosition(acc3D_t *acc)
{
	position3D_t p;

	Sx[0] = Sx[1];
	Sy[0] = Sy[1];
	Sz[0] = Sz[1];

	Vx[0] = Vx[1];
	Vy[0] = Vy[1];
	Vz[0] = Vz[1];

	Vx[1] = Vx[0] + (acc->accX - ACC_BIAS) * PERIOD_S;
	Vy[1] = Vy[0] + (acc->accY - ACC_BIAS) * PERIOD_S;
	Vy[1] = Vz[0] + (acc->accZ - ACC_BIAS) * PERIOD_S;

	Sx[1] = Sx[0] + Vx[0] * PERIOD_S + ((acc->accX - ACC_BIAS) * PERIOD_S)/2 ;
	Sy[1] = Sy[0] + Vy[0] * PERIOD_S + ((acc->accY - ACC_BIAS) * PERIOD_S)/2 ;
	Sz[1] = Sz[0] + Vz[0] * PERIOD_S + ((acc->accZ - ACC_BIAS) * PERIOD_S)/2 ;

	p.X = Sx[1];
	p.Y = Sy[1];
	p.Z = Sz[1];

	return p;
}



