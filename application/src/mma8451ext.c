/*
 * mma8451ext.c
 *
 *  Created on: 18 nov. 2023
 *      Author: sebas
 */
/*==================[inclusions]=============================================*/
#include "mma8451ext.h"

/*==================[macros and typedef]=====================================*/
#define G 		 9.8	//m/s²
#define ACC_BIAS 0	//g

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
float_t off[3];

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
void init(mma8451_accIntCount_t a);

/*==================[external functions definition]==========================*/

extern acc3D_t mma8451ext_CADToAcc(mma8451_accIntCount_t accIntCount, float_t resolution)
{
	if (off[0] == 0) init(accIntCount);
	acc3D_t accG;

	accG.accX = (((accIntCount.accX + off[0]) * resolution) - ACC_BIAS) * G;
	accG.accY = (((accIntCount.accY + off[1]) * resolution) - ACC_BIAS) * G;
	accG.accZ = (((accIntCount.accZ + off[2]) * resolution) - ACC_BIAS) * G;

	return accG;
}

void init(mma8451_accIntCount_t a)
{
	off[0] = -a.accX;
	off[1] = -a.accY;
	off[2] = 4096 -a.accZ;
}
/*==================[end of file]============================================*/


