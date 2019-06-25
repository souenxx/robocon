/*
 * LineTracer.c
 *
 *  Created on: 2016/07/17
 *      Author: ennyutakumi
 */

#include "Linetracer.h"


void Linetracer_init(Linetracer* this, PID* pid, Light_Lightdetector* lightdetector, Driver* driver){
	this->pid = pid;
	this->driver = driver;
	this->lightdetector = lightdetector;
}

//ライントレースを行う
//edge == 0:左エッジ 	1:右エッジ
//save == 0:韋駄天		1:低速用 2:超低速駐車用
void Linetracer_do(Linetracer* this, int edge, int save){
	/* Open Bluetooth file */
	int normal_light;
	int lpf_light;
	int pid = 0;

	Lightdetector_get(this->lightdetector, &normal_light, &lpf_light);
	pid = PID_calc(this->pid,lpf_light, edge);
	Driver_calcPower(this->driver, pid, edge, save);
}
