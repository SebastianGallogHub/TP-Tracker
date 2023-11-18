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
	xTaskCreate(reportPositionTask, "ReportPosition", 100, NULL, 0, NULL);
	xTaskCreate(measurePositionTask, "MeasurePosition", 100, NULL, 2, NULL);

    vTaskStartScheduler();
    for (;;);
}

extern void vApplicationDaemonTaskStartupHook()
{
	appBoard_init(); // Al menos 2 tareas requieren la placa inicializada

	appBoard_accIntEnable(false); //El sistema empieza sin registrar nada
}

void cancelAquisition(efHal_gpio_id_t id)
{
	//pausar las conversiones de mma
//	appBoard_accIntEnable(false);

	//Resetear el filtro
//	kalman_reset();

	//borrar queue mma8451_accIntCount_t
//	xQueueReset(xAccReceiveQueue);

	//borrar queue position3D_t
//	xQueueReset(xUartSendQueue);

	//Señalización
	efHal_gpio_setPin(EF_HAL_GPIO_LED_RED, true);
	efHal_gpio_setPin(EF_HAL_GPIO_LED_GREEN, false);
}

void setAquisition(efHal_gpio_id_t id) //Set
{
	//Reiniciar las conversiones para comenzar el proceso
//	appBoard_accIntEnable(true);

	//Señalización
	efHal_gpio_setPin(EF_HAL_GPIO_LED_RED, false);
	efHal_gpio_setPin(EF_HAL_GPIO_LED_GREEN, true);
}

extern void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
    while (1);
}

/*==================[end of file]============================================*/
