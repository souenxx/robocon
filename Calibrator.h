/*
 * Calibrator_Calibrate.h
 *
 *  Created on: 2016/07/14
 *      Author: ennyutakumi
 */

#ifndef CALIBRATOR_H
#define CALIBRATOR_H

#include "ev3api.h"
#include "kernel.h"
#include "D_ColorSensor.h"
#include "D_Motor.h"


#define PARA 0.5

typedef struct Calibrator
{
	int brack;
	int white;
	double boundary;
	int state;
	ColorSensor* colorsensor;
	Motor* left_motor;
	Motor* right_motor;

}Calibrator;

void Calibrator_init(Calibrator* this,ColorSensor* colorsensor,Motor* left_motor,Motor* right_motor);
int Calibrator_calibrate(Calibrator* this);
double Calibrator_get_boundary(Calibrator* this);
double Calibrator_get_white(Calibrator* this);
#endif /* CALIBRATOR_H */
