/*
 * reportPositionTask.h
 *
 *  Created on: 18 nov. 2023
 *      Author: sebas
 */

#ifndef PROJECTS_TP_TRACKER_APPLICATION_SRC_REPORTPOSITIONTASK_H_
#define PROJECTS_TP_TRACKER_APPLICATION_SRC_REPORTPOSITIONTASK_H_


/*==================[inclusions]=============================================*/
#include <math.h>

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros and typedef]=====================================*/
typedef struct
{
	float_t X;
	float_t Y;
	float_t Z;
}position3D_t;

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

extern void reportPosition_init(void);
extern void reportPosition_reset(void);
extern void reportPosition_addNewPosition(position3D_t *position);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* PROJECTS_TP_TRACKER_APPLICATION_SRC_REPORTPOSITIONTASK_H_ */
