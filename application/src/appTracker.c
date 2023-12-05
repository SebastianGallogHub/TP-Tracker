/*
 * appTracker.c
 *
 *  Created on: 18 nov. 2023
 *      Author: sebas
 */

/*==================[inclusions]=============================================*/
#include "FreeRTOS.h"
#include "task.h"

#include "appMEF.h"
#include "measurePosition.h"
#include "reportPosition.h"


/*==================[macros and typedef]=====================================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

static void test(void *pvParameters)
{
	position3D_t pp;
	position3D_t *p;
	pp.X = 1.2;
	pp.Y = 2.2;
	pp.Z = 3.2;
	p = &pp;

	while(1)
	{
		vTaskDelay(500 / portTICK_PERIOD_MS);
		reportPosition_addNewPosition(p);
	}
}

int main(void)
{
    vTaskStartScheduler();
    for (;;);
}

extern void vApplicationDaemonTaskStartupHook()
{
//	appMEF_init();
	xTaskCreate(test, "test", 100, NULL, 0, NULL);
	reportPosition_init();
//	measurePosition_init();
}

extern void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
    while (1);
}

/*==================[end of file]============================================*/
