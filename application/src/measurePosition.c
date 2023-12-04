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
#define DATA_PERIOD_MS 20

/*==================[internal functions declaration]=========================*/
static void measurePositionTask(void *pvParameters);

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void measurePositionTask(void *pvParameters)
{
	mma8451_accIntCount_t accCAD;
	acc3D_t accG;
	position3D_t position;

	//Delay para esperar que los recursos se inicialicen en otra tarea
	vTaskDelay(500 / portTICK_PERIOD_MS);

	for (;;)
	{
		if (efHal_gpio_waitForInt(EF_HAL_INT1_ACCEL, 100 / portTICK_PERIOD_MS))
		{
			accCAD = mma8451_getAccIntCount();
			accG = mma8451ext_accConvertToG(accCAD, MMA8451_RESOLUTION_2G_RANGE_14b);
			position = kalman_calcPosition(accG);

			// promediar / decimar / reducir freq a 10Hz y luego
			reportPosition_addNewPosition(position);

			vTaskDelay(DATA_PERIOD_MS / portTICK_PERIOD_MS);
		}
	}
}

/*==================[external functions definition]==========================*/
extern void measurePosition_init(void)
{
	xTaskCreate(measurePositionTask, "Measure position", 100, NULL, 0, NULL);
}

/*==================[end of file]============================================*/




