/*
 * Light_Lightdetector.c
 *
 *  Created on: 2016/07/13
 *      Author: Takenao
 */

//記述者 大塚 建直

#include "Light_Lightdetector.h"

/*
void PID_init(PID* this, int target_v){
	this -> target_V = target_v;
	this -> en_2 = 0;
	this -> en_1 = 0;
	this -> MVn_1 = 0;
}
*/

//LPF_LPF lpf;
void Lightdetector_init(Light_Lightdetector* this, ColorSensor* colorsensor, LPF_LPF* lpf){
	this -> light_level = 0;
	this -> ColorSensor_refrectlevel = 0;
	this -> colorsensor = colorsensor;
	this -> lpf = lpf;
	//lpf_init(this -> lpf);
}

void Lightdetector_get(Light_Lightdetector* this, int* normal_light, int* lpf_light){
	*normal_light = ColorSensor_getReflectBrightness(this -> colorsensor);
	*lpf_light = lpf_calc(this->lpf,*normal_light);
	//return this -> light_level;
}
