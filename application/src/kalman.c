/*
 * kalman.c
 *
 *  Created on: 14 nov. 2023
 *      Author: sebas
 */

#include "kalman.h"

extern void kalman_reset(void)
{

}

extern position3D_t kalman_calcPosition(acc3D_t *acc)
{
	position3D_t p;

	p.X = 0 ;
	p.Y = 0 ;
	p.Z = 0 ;

	return p;
}
