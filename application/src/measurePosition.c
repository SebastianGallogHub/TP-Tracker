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
#define DATA_RATE  		1/800 // 800Hz
#define MAX_VALUES  	2
#define MAX_AXIS		3
#define MAX_SAMPLES 	64
#define MAX_SAMPLES_2P	6
#define LOW_LEVEL_NOISE	3
#define T				DATA_RATE * MAX_SAMPLES

/*==================[internal functions declaration]=========================*/
static void measurePositionTask(void *pvParameters);

/*==================[internal data definition]===============================*/


static position3D_t position;
static uint16_t contAcc = 0;
static uint8_t getOffset = 0;
static int16_t OFF[MAX_AXIS];
static int16_t A[MAX_VALUES][MAX_AXIS];
static float_t V[MAX_VALUES][MAX_AXIS],
 	 	 	   P[MAX_VALUES][MAX_AXIS];
/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void measurePositionTask(void *pvParameters)
{
	uint8_t i, j;
	float_t a, v;
	mma8451_accIntCount_t accCAD;

	//Sets
	position.X = 0;
	position.Y = 0;
	position.Z = 0;

	for (i = 0; i < MAX_VALUES; i++) {
		for (j = 0; j < MAX_AXIS; j++) {
			A[i][j] = 0;
			V[i][j] = 0;
			P[i][j] = 0;
		}
	}

	//Delay para esperar que los recursos se inicialicen en otra tarea
	vTaskDelay(500 / portTICK_PERIOD_MS);
	for (;;)
	{
		if (efHal_gpio_waitForInt(EF_HAL_INT1_ACCEL, 100 / portTICK_PERIOD_MS))
		{
			contAcc ++;
			accCAD = mma8451_getAccIntCount();

			if (getOffset) {
				//Primera calibración
				OFF[0] += accCAD.accX;
				OFF[1] += accCAD.accY;
				OFF[2] += 4096 - accCAD.accZ; //Quitar el campo gravitatorio (1G)
			}

			A[1][0] += accCAD.accX;
			A[1][1] += accCAD.accY;
			A[1][2] += 4096 - accCAD.accZ; //Quitar el campo gravitatorio (1G)

			if (contAcc == 3) {
				A[1][0] += accCAD.accX;
			}

			if (contAcc >=64) {
				contAcc = 0;

				for (i = 0; i < MAX_AXIS; i++) {
					//Promediado
					A[1][i] = A[1][i]>>MAX_SAMPLES_2P; //div 64

					//Se quita el offset
					A[1][i] = OFF[i];

					//Filtrado ruido HW
					if ((A[1][i] <= LOW_LEVEL_NOISE) && (A[1][i] >= -LOW_LEVEL_NOISE)) {
						A[1][i] = 0;
					}

					a = (A[0][i] + ((A[1][i]-A[0][i])>>1)) * T; //Aproximación trapezoidal de aceleración inst
					a *= MMA8451_RESOLUTION_2G_RANGE_14b * 9.81;//m/s²

					//Primera integración
					V[1][i] = V[0][i] + a * T;  //m/s

					//Segunda integración
					v = (V[0][i] + ((V[1][i]-V[0][i])/2)) * T; //Aproximación trapezoidal de velocidad inst
					P[1][i] = P[0][i] + v * T;

					//Siguiente paso
					A[0][i] = A[1][i];
					V[0][i] = V[1][i];
					P[0][i] = P[1][i];
				}

				position.X = P[1][0];
				position.Y = P[1][1];
				position.Z = P[1][2];

				reportPosition_addNewPosition(&position);
			}
		}
		else {
			contAcc = 0;
			getOffset = 1;
		}
	}
}

/*==================[external functions definition]==========================*/
extern void measurePosition_init(void)
{
	xTaskCreate(measurePositionTask, "Measure position", 100, NULL, 0, NULL);
}

/*==================[end of file]============================================*/




