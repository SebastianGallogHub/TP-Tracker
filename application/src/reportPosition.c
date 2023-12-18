/*
 * reportPositionTask.c
 *
 *  Created on: 18 nov. 2023
 *      Author: sebas
 */


/*==================[inclusions]=============================================*/
#include "reportPosition.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include "bsp_frdmkl46z.h"

/*==================[macros and typedef]=====================================*/
#define UART_SND_QUEUE_LENGTH		50
#define UART_SND_QUEUE_ITEM_SIZE 	sizeof(position3D_t)

/*==================[internal functions declaration]=========================*/
extern void reportPositionTask(void *pvParameters);

/*==================[internal data definition]===============================*/
QueueHandle_t xUartSendQueue = NULL;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
extern void reportPositionTask(void *pvParameters)
{
	position3D_t pos;
	char fmt[] = "%10.2f;\t%10.2f;\t%10.2f;\n\r";
	char strOut[50];
	int32_t len;

	//Delay para esperar que los recursos se inicialicen en otra tarea
	vTaskDelay(500 / portTICK_PERIOD_MS);
	for (;;)
	{
		//Leer Queue position3D_t
		if(xQueueReceive(xUartSendQueue, &pos, portMAX_DELAY) == pdPASS )
		{
			//todo agregar info del sistema
			len = sprintf(strOut, fmt, pos.X, pos.Y, pos.Z);

			efHal_uart_send(efHal_dh_UART0, &strOut, len, portMAX_DELAY);
		}
	}
}

/*==================[external functions definition]==========================*/

extern void reportPosition_init(void)
{
	xUartSendQueue = xQueueCreate(UART_SND_QUEUE_LENGTH, UART_SND_QUEUE_ITEM_SIZE);

	xTaskCreate(reportPositionTask, "Report position", 300, NULL, 0, NULL);
}

extern void reportPosition_addNewPosition(position3D_t *position)
{
	xQueueSend(xUartSendQueue, position, 500 / portTICK_PERIOD_MS );
}

extern void reportPosition_reset(void)
{
	xQueueReset(xUartSendQueue);
}

/*==================[end of file]============================================*/
