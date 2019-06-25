#ifndef GyroSensor_H
#define GyroSensor_H

#include "ev3api.h"
#include "kernel.h"

// 属性
typedef struct GyroSensor
{
	sensor_port_t inputPort;
} GyroSensor;

// 公開操作
void GyroSensor_init(GyroSensor* this, sensor_port_t inputPort);
int16_t GyroSensor_getAngle(GyroSensor* this);
int16_t GyroSensor_getRate(GyroSensor* this);
ER GyroSensor_Reset(GyroSensor* this);
unsigned int GyroSensor_getOffset(GyroSensor* this);

#endif // GyroSensor_H
