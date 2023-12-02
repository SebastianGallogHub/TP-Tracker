/*
 * measurePositionTask.h
 *
 *  Created on: 18 nov. 2023
 *      Author: sebas
 */

#ifndef PROJECTS_TP_TRACKER_APPLICATION_SRC_MEASUREPOSITIONTASK_H_
#define PROJECTS_TP_TRACKER_APPLICATION_SRC_MEASUREPOSITIONTASK_H_

/*==================[inclusions]=============================================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros and typedef]=====================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

extern void measurePosition_init(void *pvParameters);
extern void measurePositionTask(void *pvParameters);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* PROJECTS_TP_TRACKER_APPLICATION_SRC_MEASUREPOSITIONTASK_H_ */
