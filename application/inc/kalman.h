/*
 * kalman.h
 *
 *  Created on: 14 nov. 2023
 *      Author: sebas
 */

/*                                                                      */

#ifndef EXAMPLES_TP_TRACKER_KALMANFILTER_INC_KALMAN_H_
#define EXAMPLES_TP_TRACKER_KALMANFILTER_INC_KALMAN_H_

/*==================[inclusions]=============================================*/

#include "appBoard.h"
#include "mma8451ext.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros and typedef]=====================================*/

typedef struct
{
	int16_t X;
	int16_t Y;
	int16_t Z;
}position3D_t;

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

extern void kalman_reset(void);
extern position3D_t kalman_calcPosition(acc3D_t acc);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* EXAMPLES_TP_TRACKER_KALMANFILTER_INC_KALMAN_H_ */
