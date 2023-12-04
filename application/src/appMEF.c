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

#include "kalman.h"
#include "measurePosition.h"
#include "reportPosition.h"
#include "appBoard.h"


/*==================[macros and typedef]=====================================*/

typedef enum
{
	MEF_APP_INIT = 0,
	MEF_NO_ACQ,
	MEF_ACQ,
} MEF_STATE;

typedef enum
{
	E_NONE = 0,
	E_SW1,
	E_SW3,
} MEF_EVENTS;

/*==================[internal functions declaration]=========================*/

static void MEF(MEF_EVENTS event);
static void start_acq(void);
static void cancel_acq(void);
static void gpio_callBackInt(efHal_gpio_id_t id);
static void setEvent(MEF_EVENTS event);
static MEF_EVENTS getEvent(void);

/*==================[internal data definition]===============================*/
MEF_EVENTS currentEvent;

/*==================[external data definition]===============================*/
/*==================[internal functions definition]==========================*/

static void MEFTask(void *pvParameters)
{
	for(;;)
	{
		MEF(getEvent());
	}
}

static void start_acq(void)
{
	//Reiniciar las conversiones para comenzar el proceso
	appBoard_accIntEnable(true);

	//Prende led verde
	efHal_gpio_setPin(EF_HAL_GPIO_LED_RED, true);
	efHal_gpio_setPin(EF_HAL_GPIO_LED_GREEN, false);
}

static void cancel_acq(void)
{
	//pausar las conversiones de mma
	appBoard_accIntEnable(false);

	//Resetear el filtro
	kalman_reset();

	//borrar queue de la tarea de envío
	reportPosition_reset();

	//Prende led rojo
	efHal_gpio_setPin(EF_HAL_GPIO_LED_RED, false);
	efHal_gpio_setPin(EF_HAL_GPIO_LED_GREEN, true);
}

static void MEF(MEF_EVENTS event)
{
	static MEF_STATE state = MEF_APP_INIT;

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
			start_acq();
			state = MEF_ACQ;
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
			setEvent(E_SW1);
            break;

        case EF_HAL_GPIO_SW_3:
        	setEvent(E_SW3);
            break;
    }

}

static void setEvent(MEF_EVENTS event)
{
	currentEvent = event;
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
