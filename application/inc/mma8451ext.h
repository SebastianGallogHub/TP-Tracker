/*
 * mma8451ext.h
 *
 *  Created on: 18 nov. 2023
 *      Author: sebas
 */

#ifndef PROJECTS_TP_TRACKER_APPLICATION_SRC_MMA8451EXT_H_
#define PROJECTS_TP_TRACKER_APPLICATION_SRC_MMA8451EXT_H_

/*==================[inclusions]=============================================*/
#include "mma8451.h"
#include "math.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros and typedef]=====================================*/
typedef struct
{
    float_t accX;
    float_t accY;
    float_t accZ;
}acc3D_t;		// valores con cifra decimal positivos y negativos

/*==================[external data declaration]==============================*/
#define MMA8451_RESOLUTION_2G_RANGE_14b	 0.25/1000 //mG
#define MMA8451_RESOLUTION_2G_RANGE_08b	 15.6/1000

#define MMA8451_RESOLUTION_4G_RANGE_14b	 0.5/1000
#define MMA8451_RESOLUTION_4G_RANGE_08b	 31.25/1000

#define MMA8451_RESOLUTION_8G_RANGE_14b	 1.0/1000
#define MMA8451_RESOLUTION_8G_RANGE_08b	 62.5/1000

/*==================[external functions declaration]=========================*/

extern acc3D_t* mma8451ext_accConvertToG(mma8451_accIntCount_t *accIntCount, float_t resolution);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/

#endif /* PROJECTS_TP_TRACKER_APPLICATION_SRC_MMA8451EXT_H_ */
