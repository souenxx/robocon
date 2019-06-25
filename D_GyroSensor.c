#include "D_GyroSensor.h"
#define  GYRO_OFFSET  610
/*
 * 初期化する
 */
void GyroSensor_init(GyroSensor* this, sensor_port_t inputPort)
{
	this->inputPort = inputPort;
	ev3_sensor_config(this->inputPort, GYRO_SENSOR);
}

/*
 * ジャイロセンサ値を得る
 * 以下，仕様より抜粋
 */
 
int16_t GyroSensor_getAngle(GyroSensor* this)
{
	return ev3_gyro_sensor_get_angle(this->inputPort);
}

int16_t GyroSensor_getRate(GyroSensor* this)
{
	return ev3_gyro_sensor_get_rate(this->inputPort);
}

ER GyroSensor_Reset(GyroSensor* this)
{
	return ev3_gyro_sensor_reset(this->inputPort);
}

unsigned int GyroSensor_getOffset(GyroSensor* this)
{
	return GYRO_OFFSET;
}
