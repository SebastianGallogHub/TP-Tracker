/*
 * mma8451ext.c
 *
 *  Created on: 18 nov. 2023
 *      Author: sebas
 */

#include "mma8451ext.h"

extern acc3D_t* mma8451ext_accConvertToG(mma8451_accIntCount_t *accIntCount, float_t resolution)
{
	acc3D_t *accG;

	accG->accX = accIntCount->accX * resolution;
	accG->accY = accIntCount->accY * resolution;
	accG->accZ = accIntCount->accZ * resolution;

	return accG;
}

