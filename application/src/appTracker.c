/*
 * appTracker.c
 *
 *  Created on: 18 nov. 2023
 *      Author: sebas
 */

/*==================[inclusions]=============================================*/
#include "FreeRTOS.h"
#include "task.h"
#include "efHal_gpio.h"
#include "appBoard.h"

#include "AppMEF.h"
#include "measurePositionTask.h"
#include "reportPositionTask.h"

/*==================[macros and typedef]=====================================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

int main(void)
{
//	xTaskCreate(reportPositionTask, "ReportPosition", 100, NULL, 0, NULL);
//	xTaskCreate(measurePositionTask, "MeasurePosition", 100, NULL, 2, NULL);

    vTaskStartScheduler();
    for (;;);
}

extern void vApplicationDaemonTaskStartupHook()
{
	MEF(E_NONE);
}

extern void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
    while (1);
}

/*==================[end of file]============================================*/
