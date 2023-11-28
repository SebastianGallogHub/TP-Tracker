/*
 * AppMEF.h
 *
 *  Created on: 28 nov. 2023
 *      Author: sebas
 */

#ifndef PROJECTS_TP_TRACKER_APPLICATION_SRC_APPMEF_H_
#define PROJECTS_TP_TRACKER_APPLICATION_SRC_APPMEF_H_

typedef enum
{
	MEF_APP_INIT = 0,
	MEF_APP_IDLE,
	MEF_CANCEL_ACQ,
	MEF_START_ACQ,
} MEF_STATE;

typedef enum
{
	E_NONE = 0,
	E_SW1,
	E_SW3,
} MEF_EVENTS;

extern void MEFTask(void *pvParameters);
extern void MEF(MEF_EVENTS event);

#endif /* PROJECTS_TP_TRACKER_APPLICATION_SRC_APPMEF_H_ */
