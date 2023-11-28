/*
 * measurePositionTask.c
 *
 *  Created on: 18 nov. 2023
 *      Author: sebas
 */

#include "FreeRTOS.h"
#include "task.h"

#include "reportPositionTask.h"
#include "efHal_gpio.h"
#include "mma8451.h"
#include "mma8451ext.h"
#include "kalman.h"

#define DATA_PERIOD_MS 20

extern void measurePositionTask(void *pvParameters)
{
	mma8451_accIntCount_t accCAD;
	acc3D_t *accG;
	position3D_t position;

	for (;;)
	{
		if (efHal_gpio_waitForInt(EF_HAL_INT1_ACCEL, pdMS_TO_TICKS(100)))
		{
			accG = mma8451ext_accConvertToG(&accCAD, MMA8451_RESOLUTION_2G_RANGE_14b);
			position = kalman_calcPosition(accG);

			// promediar / decimar / reducir freq a 10Hz y luego
			reportPositionTask_addNewPosition(position);

			vTaskDelay(DATA_PERIOD_MS / portTICK_PERIOD_MS);
		}
	}
}

