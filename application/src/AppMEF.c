/*
 * AppMEF.c
 *
 *  Created on: 28 nov. 2023
 *      Author: sebas
 */
#include "AppMEF.h"
#include "FreeRTOS.h"
#include "appBoard.h"
#include "efHal_gpio.h"

#include "kalman.h"

static void gpio_callBackInt(efHal_gpio_id_t id);

void MEF(MEF_EVENTS event)
{
	MEF_STATE state = MEF_APP_INIT;

	switch(state)
	{
	case MEF_APP_INIT:
		// Al menos 2 tareas requieren la placa inicializada
		appBoard_init();

		//El sistema empieza sin registrar nada
		//Desactivo la fuente de interrupciones en el acelerómetro
		appBoard_accIntEnable(false);

		efHal_gpio_setCallBackInt(EF_HAL_GPIO_SW_1, gpio_callBackInt);
		efHal_gpio_setCallBackInt(EF_HAL_GPIO_SW_3, gpio_callBackInt);

		state = MEF_CANCEL_ACQ;
		break;

	case MEF_CANCEL_ACQ:
		if (event == E_SW1) {
			//Reiniciar las conversiones para comenzar el proceso
//			appBoard_accIntEnable(true);

			efHal_gpio_setPin(EF_HAL_GPIO_LED_RED, false);
			efHal_gpio_setPin(EF_HAL_GPIO_LED_GREEN, true);

			state = MEF_START_ACQ;
		}
		break;

	case MEF_START_ACQ:
		if (event == E_SW3) {
			//pausar las conversiones de mma
			//	appBoard_accIntEnable(false);

				//Resetear el filtro
			//	kalman_reset();

				//borrar queue mma8451_accIntCount_t
			//	xQueueReset(xAccReceiveQueue);

				//borrar queue position3D_t
			//	xQueueReset(xUartSendQueue);

			efHal_gpio_setPin(EF_HAL_GPIO_LED_RED, true);
			efHal_gpio_setPin(EF_HAL_GPIO_LED_GREEN, false);

			state = MEF_CANCEL_ACQ;
		}
		break;

	};

}

static void gpio_callBackInt(efHal_gpio_id_t id)
{
    switch (id)
    {
        case EF_HAL_GPIO_SW_1:
			MEF(E_SW1);
            break;

        case EF_HAL_GPIO_SW_3:
        	MEF(E_SW3);
            break;
    }

}

