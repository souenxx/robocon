/****************************************************
 *  UltrasonicSensor.c                                         
 *  Created on: 2014/09/09 11:38:53                      
 *  Implementation of the Class UltrasonicSensor       
 *  Original author: aritsugi_kitasuka                     
 ****************************************************/

#include "D_SonarSensor.h"

void SonarSensor_init(SonarSensor* this, sensor_port_t inputPort)
{
	this->inputPort=inputPort;
	ev3_sensor_config(this->inputPort, ULTRASONIC_SENSOR);
}

int16_t SonarSensor_getDistance(SonarSensor* this){
	return ev3_ultrasonic_sensor_get_distance(this->inputPort);
}
