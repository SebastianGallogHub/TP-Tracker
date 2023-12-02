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

/*==================[macros and typedef]=====================================*/
#define UART_SND_QUEUE_LENGTH		100
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
	uint8_t header[] = "Hola Mundo...\n\rX\tY\tZ\t ";
	uint8_t charTemp;

	//Delay para esperar que los recursos se inicialicen en otra tarea
	vTaskDelay(500 / portTICK_PERIOD_MS);

	//Encabezado y aviso de tarea inicializada
	efHal_uart_send(efHal_dh_UART1, &header, sizeof(header), portMAX_DELAY);
	for (;;)
	{
		//Leer Queue position3D_t
		if(xQueueReceive(xUartSendQueue, &position, portMAX_DELAY) == pdPASS )
		{
//			//todo obtener info del sistema
//
//			//composición
//			//todo componer el string separado por ";"
//			//todo agregar info del sistema
//
//			efHal_uart_send(efHal_dh_UART0, &charTemp, sizeof(charTemp), portMAX_DELAY);
		}
	}
}

/*==================[external functions definition]==========================*/

extern void reportPosition_init(void)
{
	xUartSendQueue = xQueueCreate(UART_SND_QUEUE_LENGTH, UART_SND_QUEUE_ITEM_SIZE);

	xTaskCreate(reportPositionTask, "Report position", 100, NULL, 0, NULL);
}

extern void reportPosition_addNewPosition(position3D_t position)
{
	xQueueSend(xUartSendQueue, &position, (TickType_t) 1 );
}

extern void reportPosition_reset(void)
{
	xQueueReset(xUartSendQueue);
}

/*==================[end of file]============================================*/
