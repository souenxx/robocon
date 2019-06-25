/*
 * Prep_Arm.c
 *
 *  Created on: 2016/07/14
 *      Author: abeco
 */


#include "Prep_Arm.h"

void arm_init(Prep_Arm* this, Motor* arm_motor){
	this->count = 0;
	this->arm_motor = arm_motor;
	this->state = 0;
}

int arm_rize(Prep_Arm* this){
	if(this->state == 0 && this->count < 100){
		Motor_setPower(this->arm_motor, -10);
		this->count++;
		return false;
	}else if(this->state == 0 && this->count >= 100){
//		Motor_setPower(this->arm_motor, 0);
		Motor_stop(this->arm_motor,true);
		this->state = 1;
		return false;
	}else{
		Motor_setRotatedegree(this->arm_motor, 30 ,30,true);
		return true;
	}
}