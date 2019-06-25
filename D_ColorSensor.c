#include "D_ColorSensor.h"

void ColorSensor_init(ColorSensor* this, sensor_port_t inputPort)
{
	this->inputPort = inputPort;
	ev3_sensor_config(this->inputPort,COLOR_SENSOR);
}

// 環境光の輝度を得る
uint8_t ColorSensor_getAmbientBrightness(ColorSensor* this)
{
	return ev3_color_sensor_get_ambient(this->inputPort);
}

//　カラーセンサーでカラーを識別する(EV3決め打ちのデータで識別する)
colorid_t ColorSensor_getColor(ColorSensor* this)
{
	return ev3_color_sensor_get_color(this->inputPort);

}
// 反射光の輝度を得る
uint8_t ColorSensor_getReflectBrightness(ColorSensor* this)
{
	return  ev3_color_sensor_get_reflect(this->inputPort);
}
//　RGB値でカラーの値を得る (Valにデータが入る)
void ColorSensor_getRawColor(ColorSensor* this,rgb_raw_t* val){
	ev3_color_sensor_get_rgb_raw(this->inputPort,val);
}

