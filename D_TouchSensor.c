#include "D_TouchSensor.h"
// 初期化する
void TouchSensor_init(TouchSensor* this, sensor_port_t inputPort)
{
	this->inputPort = inputPort;
	ev3_sensor_config(this->inputPort, TOUCH_SENSOR);
}
//タッチセンサーの状態を検出する trueが押されている状態falseが押されていない状態
bool_t TouchSensor_getState(TouchSensor* this)
{
	return ev3_touch_sensor_is_pressed(this->inputPort);
}

