/*
 * reportPositionTask.h
 *
 *  Created on: 18 nov. 2023
 *      Author: sebas
 */

#ifndef PROJECTS_TP_TRACKER_APPLICATION_SRC_REPORTPOSITIONTASK_H_
#define PROJECTS_TP_TRACKER_APPLICATION_SRC_REPORTPOSITIONTASK_H_


/*==================[inclusions]=============================================*/

#include "kalman.h"
/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros and typedef]=====================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

extern void reportPosition_init(void);
extern void reportPositionTask(void *pvParameters);
extern void reportPositionTask_addNewPosition(position3D_t position);
extern void reportPositionTask_reset(void);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* PROJECTS_TP_TRACKER_APPLICATION_SRC_REPORTPOSITIONTASK_H_ */
