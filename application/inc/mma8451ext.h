/*
 * mma8451ext.h
 *
 *  Created on: 18 nov. 2023
 *      Author: sebas
 */

#ifndef PROJECTS_TP_TRACKER_APPLICATION_SRC_MMA8451EXT_H_
#define PROJECTS_TP_TRACKER_APPLICATION_SRC_MMA8451EXT_H_

#include "mma8451.h"
#include "math.h"

typedef struct
{
    float_t accX;
    float_t accY;
    float_t accZ;
}acc3D_t;		// valores con cifra decimal positivos y negativos

#define MMA8451_RESOLUTION_2G_RANGE_14b		0.25 	/1000 //mG
#define MMA8451_RESOLUTION_2G_RANGE_08b		15.6	/1000

#define MMA8451_RESOLUTION_4G_RANGE_14b		0.5		/1000
#define MMA8451_RESOLUTION_4G_RANGE_08b		31.25	/1000

#define MMA8451_RESOLUTION_8G_RANGE_14b		1.0		/1000
#define MMA8451_RESOLUTION_8G_RANGE_08b		62.5	/1000

extern acc3D_t* mma8451ext_accConvertToG(mma8451_accIntCount_t *accIntCount, float_t resolution);

#endif /* PROJECTS_TP_TRACKER_APPLICATION_SRC_MMA8451EXT_H_ */
