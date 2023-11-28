/*
 * reportPositionTask.c
 *
 *  Created on: 18 nov. 2023
 *      Author: sebas
 */

#include "reportPositionTask.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "kalman.h"

#define UART_SND_QUEUE_LENGTH		100
#define UART_SND_QUEUE_ITEM_SIZE 	sizeof(position3D_t)

QueueHandle_t xUartSendQueue = NULL;

extern void reportPositionTask(void *pvParameters)
{
	position3D_t position;
	uint8_t header[] = "Hola Mundo...\n\rX\tY\tZ\t ";
	uint8_t charTemp;

	xUartSendQueue = xQueueCreate(UART_SND_QUEUE_LENGTH, UART_SND_QUEUE_ITEM_SIZE);

	//Encabezado y aviso de tarea inicializada
	efHal_uart_send(efHal_dh_UART0, &header, sizeof(header), portMAX_DELAY);

	for (;;)
	{
		//Leer Queue position3D_t
		if(xQueueReceive(xUartSendQueue, &position, portMAX_DELAY) == pdPASS )
		{
			//todo obtener info del sistema

			//composición
			//todo componer el string separado por ";"
			//todo agregar info del sistema

			efHal_uart_send(efHal_dh_UART0, &charTemp, sizeof(charTemp), portMAX_DELAY);
		}
	}
}

extern void reportPositionTask_addNewPosition(position3D_t position)
{
	xQueueSend(xUartSendQueue, &position, (TickType_t) 1 );
}

extern void reportPositionTask_reset(void)
{
	xQueueReset(xUartSendQueue);
}

