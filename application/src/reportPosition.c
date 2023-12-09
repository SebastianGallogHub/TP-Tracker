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
#include "kalman.h"
#include <stdio.h>

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
	position3D_t position;
	char header[] = "\n\rTP 3D Tracker\n\r\n\r*Presione \n\r\t->SW1 para iniciar\n\r\t->SW3 para cancelar\n\rX;Y;Z\n\r";
	char dataFmt[] = "%3.2f;%3.2f;%3.2f;\n\r";
	char strOut[50];
	int32_t len;

	//Delay para esperar que los recursos se inicialicen en otra tarea
	vTaskDelay(500 / portTICK_PERIOD_MS);

	//Encabezado y aviso de tarea inicializada
	efHal_uart_send(efHal_dh_UART0, &header, sizeof(header), portMAX_DELAY);
	for (;;)
	{
		//Leer Queue position3D_t
		if(xQueueReceive(xUartSendQueue, &position, portMAX_DELAY) == pdPASS )
		{
			//todo agregar info del sistema
			len = sprintf(strOut, dataFmt, position.X, position.Y, position.Z);

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

extern void reportPosition_addNewPosition(void *position)
{
	xQueueSend(xUartSendQueue, position, 500 / portTICK_PERIOD_MS );
}

extern void reportPosition_reset(void)
{
	xQueueReset(xUartSendQueue);
}

/*==================[end of file]============================================*/
