/*
 * AppMEF.c
 *
 *  Created on: 28 nov. 2023
 *      Author: sebas
 */

/*==================[inclusions]=============================================*/
#include "appMEF.h"

#include "FreeRTOS.h"
#include "efHal_gpio.h"
#include "task.h"
#include "stdbool.h"

#include "measurePosition.h"
#include "reportPosition.h"
#include "appBoard.h"


/*==================[macros and typedef]=====================================*/

/*==================[internal functions declaration]=========================*/
static void MEF(MEF_EVENTS event);
static void start_acq(void);
static void cancel_acq(void);
static void gpio_callBackInt(efHal_gpio_id_t id);
static MEF_EVENTS getEvent(void);

/*==================[internal data definition]===============================*/
MEF_STATE state = MEF_APP_INIT;
MEF_EVENTS currentEvent = E_NONE;

/*==================[external data definition]===============================*/
/*==================[internal functions definition]==========================*/

static void MEFTask(void *pvParameters)
{
	for(;;)
	{
		MEF(getEvent());
	}
}

static void calib(void)
{
	static uint8_t header[] = "\n\r****** CALIBRANDO ... ******\n\r";
	efHal_uart_send(efHal_dh_UART0, &header, sizeof(header), portMAX_DELAY);

	//Reiniciar las conversiones para comenzar el proceso
	appBoard_accIntEnable(true);

	//Prende ambos
	efHal_gpio_setPin(EF_HAL_GPIO_LED_RED, false);
	efHal_gpio_setPin(EF_HAL_GPIO_LED_GREEN, false);
}

static void start_acq(void)
{
	static uint8_t header[] = "\n\rTP 3D Tracker\n\r\n\r*Presione SW3 para cancelar\n\rX;\tY;\tZ;\n\r";
	efHal_uart_send(efHal_dh_UART0, &header, sizeof(header), portMAX_DELAY);

	//Prende led verde
	efHal_gpio_setPin(EF_HAL_GPIO_LED_RED, true);
	efHal_gpio_setPin(EF_HAL_GPIO_LED_GREEN, false);
}

static void cancel_acq(void)
{
	static uint8_t footer[] = "\n\r********* FIN ********* \n\r Presione SW1 para iniciar\n\r";

	//pausar las conversiones de mma
	appBoard_accIntEnable(false);

	//borrar queue de la tarea de envío
	reportPosition_reset();

	//Prende led rojo
	efHal_gpio_setPin(EF_HAL_GPIO_LED_RED, false);
	efHal_gpio_setPin(EF_HAL_GPIO_LED_GREEN, true);

	efHal_uart_send(efHal_dh_UART0, &footer, sizeof(footer), portMAX_DELAY);
}


extern MEF_STATE appMEF_getState(void)
{
	return state;
}

extern void appMEF_setEvent(MEF_EVENTS event)
{
	currentEvent = event;
}

static void MEF(MEF_EVENTS event)
{
	switch(state)
	{
	case MEF_APP_INIT:
		// Al menos 2 tareas requieren la placa inicializada
		appBoard_init();

		//Para obtener los eventos que hacen evolucionar la MEF
		efHal_gpio_confInt(EF_HAL_GPIO_SW_1, EF_HAL_GPIO_INT_TYPE_FALLING_EDGE);
		efHal_gpio_setCallBackInt(EF_HAL_GPIO_SW_1, gpio_callBackInt);

		efHal_gpio_confInt(EF_HAL_GPIO_SW_3, EF_HAL_GPIO_INT_TYPE_FALLING_EDGE);
		efHal_gpio_setCallBackInt(EF_HAL_GPIO_SW_3, gpio_callBackInt);

		cancel_acq();
		state = MEF_NO_ACQ;
		break;

	case MEF_NO_ACQ:
		if (event == E_SW1) {
			calib();
			state = MEF_CALIB;
		}
		break;

	case MEF_CALIB:
		if (event == E_END_CALIB) {
			start_acq();
			state = MEF_ACQ;
		}
		if (event == E_SW3) {
			cancel_acq();
			state = MEF_NO_ACQ;
		}
		break;

	case MEF_ACQ:
		if (event == E_SW3) {
			cancel_acq();
			state = MEF_NO_ACQ;
		}
		break;

	};

}

static void gpio_callBackInt(efHal_gpio_id_t id)
{
    switch (id)
    {
        case EF_HAL_GPIO_SW_1:
        	currentEvent = E_SW1;
            break;

        case EF_HAL_GPIO_SW_3:
        	currentEvent = E_SW3;
            break;
    }

}

static MEF_EVENTS getEvent(void)
{
	MEF_EVENTS e = currentEvent;
	currentEvent = E_NONE;

	return e;
}

/*==================[external functions definition]==========================*/

extern void appMEF_init(void)
{
	xTaskCreate(MEFTask, "MEF", 100, NULL, 0, NULL);
}

/*==================[end of file]============================================*/
