/*
 * LPF_LPF.c
 *
 *  Created on: 2016/07/13
 *      Author: Takenao
 */

//記述者 大塚 建直

#include "LPF_LPF.h"


void lpf_init(LPF_LPF* this){
	this -> present_lightdetector_level = 0;
	this -> last_LPF_output_level = 0;
	this -> parameter_A = 0.5;

}

int lpf_calc(LPF_LPF* this,int present_lightdetector_level){
	this -> present_lightdetector_level = present_lightdetector_level;

 return this -> last_LPF_output_level = this -> present_lightdetector_level * (1- this -> parameter_A) + this -> last_LPF_output_level * this -> parameter_A;

}
