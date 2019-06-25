/*
 * Prep_Arm.h
 *
 *  Created on: 2016/07/14
 *      Author: abeco
 */

#ifndef Prep_Arm_H_
#define Prep_Arm_H_

#include "D_Motor.h"
#include "ev3api.h"
#include "kernel.h"

typedef struct Prep_Arm{
	int count;
	int state;
	Motor* arm_motor;
}Prep_Arm;

void arm_init(Prep_Arm* this, Motor* arm_motor);
int arm_rize(Prep_Arm* this);


#endif /* Prep_Arm_H_ */
