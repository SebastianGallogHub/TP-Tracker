/*
 * measurePositionTask.c
 *
 *  Created on: 18 nov. 2023
 *      Author: sebas
 */

/*==================[inclusions]=============================================*/
#include "measurePosition.h"
#include "reportPosition.h"
#include "FreeRTOS.h"
#include "task.h"

#include "efHal_gpio.h"
#include "mma8451.h"
#include "mma8451ext.h"
#include "kalman.h"

/*==================[macros and typedef]=====================================*/
typedef enum
{
	CALI_ACC,
	CALC_POS,
} STATE;

#define DATA_RATE  		200 //Hz
#define SAMPLE_TIME		1/DATA_RATE
#define MAX_VALUES  	3
#define MAX_AXIS		3
#define SAMPLES 		64
#define T				DATA_RATE * SAMPLES
#define LOW_LEVEL_NOISE 3
#define END_OF_MOVEMENT 5

/*==================[internal functions declaration]=========================*/
static void measurePositionTask(void *pvParameters);

/*==================[internal data definition]===============================*/

//salida
static position3D_t position;
static STATE st = CALI_ACC;
//contaddores
static uint16_t contAcc = 0;//, cCali = 0;
////banderas
//static uint8_t freset = 1, cali = 1;

static int8_t END_OF_MOVEMENT_CT[MAX_AXIS];
static int32_t OF[MAX_AXIS];

static int16_t A[MAX_VALUES][MAX_AXIS],
			   Af[MAX_VALUES][MAX_AXIS];

static float_t V[MAX_VALUES][MAX_AXIS],
 	 	 	   P[MAX_VALUES][MAX_AXIS];

//butterworth
static float_t sampling_rate;
static float_t corr_cutoff_f;
static float_t K1,K2,K3;
static float_t A0,A1,A2;
static float_t B1,B2;
/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void butterworth_init(float_t cutoff_f, float_t sample_t)
{
	sampling_rate = 1/sample_t;
	corr_cutoff_f = tanf(3.14 * cutoff_f/ sampling_rate);

	K1 = sqrtf(2) * corr_cutoff_f;
	K2 = corr_cutoff_f * corr_cutoff_f;

	A0 = K2 / (1 + K1 + K2);
	A1 = 2 * A0;
	A2 = A0;

	K3 = A1/K2;

	B1 = -A1 + K3;
	B2 = 1 - A1 - K3;
}


static float_t butterworth_2_lpf(float_t x0, float_t x1, float_t x2, float_t y1, float_t y2 )
{
	/*
	 * x0: actual
	 * x1: actual-1
	 * x2: actual-2
	 *
	 * y1: filtrada-1
	 * y2: filtrada-2
	 *
	 * */

	return A0 * x0 + A1 * x1 + A2 * x2 + B1 * y1 + B2 * y2;
}


static void reset()
{
	static uint8_t i, j;

	position.X = 0;
	position.Y = 0;
	position.Z = 0;
	for (i = 0; i < MAX_VALUES; i++) {
		for (j = 0; j < MAX_AXIS; j++) {
			OF[j] = 0;

			A[i][j] = 0;
			Af[i][j] = 0;

			V[i][j] = 0;
			P[i][j] = 0;
		}
	}
}

static uint8_t filter_acc()
{
	static uint8_t i;

	contAcc++;
	if (contAcc >= SAMPLES) {
		contAcc = 0;

		for (i = 0; i < MAX_AXIS; i++) {
			//Promediado
			A[0][i] = A[0][i] / SAMPLES;

			//Filtro de aceleración
			Af[0][i] = butterworth_2_lpf(A[0][i], A[1][i], A[2][i], Af[1][i], Af[2][i]);

			//Avanzar un paso del filtro
			A[2][i] = A[1][i];	Af[2][i] = Af[1][i];
			A[1][i] = A[0][i];	Af[1][i] = Af[0][i];
		}

		return 1;
	}

	return 0;
}

static uint8_t calibrate_acc()
{
	static uint32_t c = 0;
	static uint8_t i;

	c++;
	for (i = 0; i < MAX_AXIS; ++i) {
		OF[i] += Af[0][i];
	}

	if (c>=1024)
	{
		c =0;
		for (i = 0; i < MAX_AXIS; ++i) {
			OF[i] = OF[i]/1024;
		}
		return 1;
	}

	return 0;
}

static void calculate_pos()
{
	static float_t a,v;
	static uint8_t i;

	for (i = 0; i < MAX_AXIS; ++i) {
		//Filtrado ruido HW
		if ((Af[0][i] <= LOW_LEVEL_NOISE) && (Af[0][i] >= -LOW_LEVEL_NOISE)) {
			Af[0][i] = 0;
		}

		a = (Af[0][i] + ((Af[1][i]-Af[0][i])/2)) * T; //Aproximación trapezoidal de aceleración inst
		a = a * MMA8451_RESOLUTION_2G_RANGE_14b * 9.81; //m/s²

		//Primera integración
		V[1][i] = V[0][i] + a * T;  //m/s
		v = (V[0][i] + ((V[1][i]-V[0][i])/2)) * T; //Aproximación trapezoidal de velocidad inst

		//Segunda integración
		P[1][i] = P[0][i] + v * T; 	//m

		//Siguiente paso
		V[0][i] = V[1][i];
		P[0][i] = P[1][i];

		//Verificación de final de movimiento para minimizar el error de velocidad
		if (A[0][i] == 0) {
			END_OF_MOVEMENT_CT[i]++;
		}

		if (END_OF_MOVEMENT_CT[i] >= END_OF_MOVEMENT) {
			V[0][i] = 0;
			V[1][i] = 0;
		}
	}

	position.X = P[1][0];
	position.Y = P[1][1];
	position.Z = P[1][2];

	//Enviar valor de posición
	reportPosition_addNewPosition(&position);
}

static void measurePositionTask(void *pvParameters)
{
	static uint8_t i, c;
	static uint8_t calibrated = 0;
	mma8451_accIntCount_t accCAD;

	//Delay para esperar que los recursos se inicialicen en otra tarea
//	vTaskDelay(500 / portTICK_PERIOD_MS);
	reset();
	for (;;)
	{
		if (efHal_gpio_waitForInt(EF_HAL_INT1_ACCEL, 400 / portTICK_PERIOD_MS)) {
			accCAD = mma8451_getAccIntCount();
			A[0][0] += accCAD.accX;
			A[0][1] += accCAD.accY;
			A[0][2] += 4096 - accCAD.accZ;

			//Filtro Butterworth
			if (filter_acc()) {

				//Calibración
				if (!calibrated) {
					c++;
					for (i = 0; i < MAX_AXIS; ++i) {
						OF[i] += Af[0][i];
					}

					if (c>=64)
					{
						c =0;
						calibrated = 1;
						for (i = 0; i < MAX_AXIS; ++i) {
							OF[i] = OF[i]/64;
						}
					}
				}
				else{
//					position.X = Af[0][0];
//					position.Y = Af[0][1];
//					position.Z = Af[0][2];

					position.X = Af[0][0]-OF[0];
					position.Y = Af[0][1]-OF[1];
					position.Z = Af[0][2]-OF[2];

					//Enviar valor de posición
					reportPosition_addNewPosition(&position);
				}

			}
		}
		else{
			calibrated = 0;
		}

//		if (filter_acc()) {
//			switch(st)
//			{
//			case CALI_ACC:
//				c++;
//				for (i = 0; i < MAX_AXIS; ++i) {
//					OF[i] += Af[0][i];
//				}
//
//				if (c>=1024)
//				{
//					c =0;
//					for (i = 0; i < MAX_AXIS; ++i) {
//						OF[i] = OF[i]/1024;
//					}
//					st = CALC_POS;
//				}
//
//				break;
//
//			case CALC_POS:
//				calculate_pos();
//
//				if(accShutDown)
//				{
//					reset();
//					st = CALI_ACC;
//					reportPosition_addNewPosition(&position);
//				}
//				break;
//			}
		}

//		if(cali)
//		{
//			if(getacc())
//			{
//				cCali++;
//
//				if(cCAli)
//			}
//		}
//		else{
//			if (getAcc()) {
//
//				for (i = 0; i < MAX_AXIS; ++i) {
//
//					//Filtrado ruido HW
//					if ((Af[0][i] <= LOW_LEVEL_NOISE) && (Af[0][i] >= -LOW_LEVEL_NOISE)) {
//						Af[0][i] = 0;
//					}
//
//					a = (Af[0][i]) MMA8451_RESOLUTION_2G_RANGE_14b * 9.81;//m/s²
//
//					//Primera integración
//					V[1][i] = V[0][i] + a * T;  //m/s
//
//					//Segunda integración
//					v = (V[0][i] + ((V[1][i]-V[0][i])/2)) * T; //Aproximación trapezoidal de velocidad inst
//					P[1][i] = P[0][i] + v * T;
//
//					//Siguiente paso
//					V[0][i] = V[1][i];
//					P[0][i] = P[1][i];
//
//					V[1][i] = 0;
//					P[1][i] = 0;
//
//					if (A[0][i] == 0) {
//						END_OF_MOVEMENT_CT[i]++;
//					}
//
//					if (END_OF_MOVEMENT_CT[i] >= END_OF_MOVEMENT) {
//						V[0][i] = 0;
//						V[1][i] = 0;
//					}
//				}
//
//				position.X = P[1][0];
//				position.Y = P[1][1];
//				position.Z = P[1][2];
//
//				reportPosition_addNewPosition(&position);
//			}
//		}
//	}
}

/*==================[external functions definition]==========================*/
extern void measurePosition_init(void)
{
	butterworth_init((float_t)(DATA_RATE / SAMPLES) / 4, (float_t)SAMPLE_TIME * SAMPLES);

	xTaskCreate(measurePositionTask, "Measure position", 100, NULL, 0, NULL);
}

/*==================[end of file]============================================*/




