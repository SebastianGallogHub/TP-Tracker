/*
 * reportPositionTask.h
 *
 *  Created on: 18 nov. 2023
 *      Author: sebas
 */

#ifndef PROJECTS_TP_TRACKER_APPLICATION_SRC_REPORTPOSITIONTASK_H_
#define PROJECTS_TP_TRACKER_APPLICATION_SRC_REPORTPOSITIONTASK_H_

#include "kalman.h"

extern void reportPositionTask(void *pvParameters);
extern void reportPositionTask_addNewPosition(position3D_t position);

#endif /* PROJECTS_TP_TRACKER_APPLICATION_SRC_REPORTPOSITIONTASK_H_ */
