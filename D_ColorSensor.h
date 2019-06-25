#ifndef ColorSensor_H
#define ColorSensor_H

#include "ev3api.h"
#include "kernel.h"


// 属性を保持するための構造体の定義
typedef struct ColorSensor
{
	sensor_port_t inputPort;
} ColorSensor;

// 公開操作
void ColorSensor_init(ColorSensor* this, sensor_port_t inputPort);
uint8_t ColorSensor_getAmbientBrightness(ColorSensor* this);
colorid_t  ColorSensor_getColor(ColorSensor* this);
uint8_t ColorSensor_getReflectBrightness(ColorSensor* this);
void ColorSensor_getRawColor(ColorSensor* this,rgb_raw_t* val);

#endif // LightSensor_H
