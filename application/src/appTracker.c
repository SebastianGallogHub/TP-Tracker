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

int main(void)
{
    vTaskStartScheduler();
    for (;;);
}

extern void vApplicationDaemonTaskStartupHook()
{
	appMEF_init();
	reportPosition_init();
	measurePosition_init();
}

extern void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
    while (1);
}

/*==================[end of file]============================================*/
