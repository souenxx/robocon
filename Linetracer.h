/*
 * LineTracer.h
 *
 *  Created on: 2016/07/17
 *      Author: ennyutakumi
 */

#ifndef LINETRACER_H_
#define LINETRACER_H_

#include "ev3api.h"
#include "kernel.h"

#include "Driver.h"
#include "Light_Lightdetector.h"
#include "PID_PID.h"

typedef struct Linetracer
{
	PID* pid;
	Light_Lightdetector* lightdetector;
	Driver* driver;
}Linetracer;

void Linetracer_init(Linetracer* this, PID* pid, Light_Lightdetector* lightdetector, Driver* driver);
void Linetracer_do(Linetracer* this, int edge, int save);
//void Linetracer_set_boundly(Linetracer* this,int boundly);
#endif /* LINETRACER_H_ */
