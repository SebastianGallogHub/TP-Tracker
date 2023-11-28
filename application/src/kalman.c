/*
 * kalman.c
 *
 *  Created on: 14 nov. 2023
 *      Author: sebas
 */

#include "kalman.h"
#include "math.h"

//#define MAX_STEP 4294967295	//max de uint16_t ~6000 días
#define MAX_STEP 5

#define ACC_BIAS 0.03 		//g
#define PERIOD_S 0.00125 	// muestreo cada 800Hz

static uint64_t currentStep;
static float_t Sx[MAX_STEP], Sy[MAX_STEP], Sz[MAX_STEP]; //Posición
static float_t Vx[MAX_STEP], Vy[MAX_STEP], Vz[MAX_STEP]; //Velocidad

extern void kalman_reset(void)
{
	currentStep = 0;

	for (uint64_t j = 0; j < MAX_STEP; ++j)
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

	currentStep ++;

	Sx[currentStep] = Sx[currentStep-1] + Vx[currentStep-1] * PERIOD_S + ((acc->accX - ACC_BIAS) * PERIOD_S)/2 ;
	Sy[currentStep] = Sy[currentStep-1] + Vy[currentStep-1] * PERIOD_S + ((acc->accY - ACC_BIAS) * PERIOD_S)/2 ;
	Sz[currentStep] = Sz[currentStep-1] + Vz[currentStep-1] * PERIOD_S + ((acc->accZ - ACC_BIAS) * PERIOD_S)/2 ;

	Vx[currentStep] = Vx[currentStep-1] + (acc->accX - ACC_BIAS) * PERIOD_S;
	Vy[currentStep] = Vy[currentStep-1] + (acc->accY - ACC_BIAS) * PERIOD_S;
	Vy[currentStep] = Vz[currentStep-1] + (acc->accZ - ACC_BIAS) * PERIOD_S;

	p.X = Sx[currentStep];
	p.Y = Sy[currentStep];
	p.Z = Sz[currentStep];

	return p;
}



