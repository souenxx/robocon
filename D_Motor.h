/*

担当:明石　啓司郎  */

#ifndef Motor_H
#define Motor_H

#include "ev3api.h"
#include "kernel.h"

typedef struct Motor{
	int outputport;
} Motor;

void Motor_init(Motor* this, int outputport,motor_type_t type);
int32_t Motor_getAngle(Motor* this);
int Motor_getPower(Motor* this);
ER Motor_resetAnglecount(Motor* this);
ER Motor_setRotatedegree(Motor* this, int degrees,uint32_t speed_abs,bool_t blocking);
ER Motor_setPower(Motor* this, int power);
ER Motor_stop(Motor* this,bool_t brake);



#endif
