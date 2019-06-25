/*
 * Light_Lightdetector.c
 *
 *  Created on: 2016/07/13
 *      Author: Takenao
 */

//記述者 大塚 建直

#ifndef LIGHT_LIGHTDETECTOR_H_
#define LIGHT_LIGHTDETECTOR_H_

#include "ev3api.h"
#include "kernel.h"
#include "LPF_LPF.h"
#include "D_ColorSensor.h"




 typedef struct Light_Lightdetector{

//変数
int light_level;	//明度
int ColorSensor_refrectlevel;		//カラーセンサーから取ってきた値
ColorSensor* colorsensor;
LPF_LPF* lpf;

}Light_Lightdetector;


//関数
void Lightdetector_init(Light_Lightdetector* this, ColorSensor* colorsensor, LPF_LPF* lpf);
void Lightdetector_get(Light_Lightdetector* this, int* normal_light, int* lpf_light);

#endif /* LIGHT_LIGHTDETECTOR_H_ */
