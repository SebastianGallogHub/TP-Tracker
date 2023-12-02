/*
 * AppMEF.h
 *
 *  Created on: 28 nov. 2023
 *      Author: sebas
 */

#ifndef PROJECTS_TP_TRACKER_APPLICATION_SRC_APPMEF_H_
#define PROJECTS_TP_TRACKER_APPLICATION_SRC_APPMEF_H_

/*==================[inclusions]=============================================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros and typedef]=====================================*/

typedef enum
{
	MEF_APP_INIT = 0,
	MEF_CANCEL_ACQ,
	MEF_START_ACQ,
} MEF_STATE;

typedef enum
{
	E_NONE = 0,
	E_SW1,
	E_SW3,
} MEF_EVENTS;

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

extern void MEF_init(void);
extern void MEFTask(void *pvParameters);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/

#endif /* PROJECTS_TP_TRACKER_APPLICATION_SRC_APPMEF_H_ */
