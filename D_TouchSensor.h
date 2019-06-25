#ifndef D_TOUCHSENSOR_H_
#define D_TOUCHSENSOR_H_

#include "ev3api.h"
#include "kernel.h"


// 属性を保持するための構造体の定義
typedef struct TouchSensor
{
	sensor_port_t inputPort;
} TouchSensor;

// 公開操作
void TouchSensor_init(TouchSensor* this, sensor_port_t inputPort);
bool_t TouchSensor_getState(TouchSensor* this);

#endif
