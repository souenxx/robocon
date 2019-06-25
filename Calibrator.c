/*
 * Calibrator_Calibrate.c
 *
 *  Created on: 2016/07/14
 *      Author: ennyutakumi
 */

#include "Calibrator.h"

void Calibrator_init(Calibrator* this,ColorSensor* colorsensor,Motor* left_motor,Motor* right_motor){

	this->brack = 99;
	this->white = 1;
	this->boundary = 0;
	this->state =0;

	this->colorsensor = colorsensor;
	this->left_motor = left_motor;
	this->right_motor = right_motor;
}
int Calibrator_calibrate(Calibrator* this){
//	まずは前進しつづける
	Motor_setPower(this->left_motor,10);
	Motor_setPower(this->right_motor,10);

 	int light = ColorSensor_getReflectBrightness(this->colorsensor);
// 　反射光の強さは0〜100までで黒〜白に対応する
//	一番明るいところと暗いところを保持する。
	if(light >= this->white){
		this->white=light;
	}
	if(light <= this->brack){
		this->brack=light;
	}
//	境界値の決定
	this->boundary =  this->brack +(this->white-this->brack)*PARA;

	switch(this->state){
//	白から黒に入る場合
	case 0:
		if(this->white-light >=20){
			this->state = 1;
		}
		break;
//	黒から白に入る場合
	case 1:
		if(this->white-light <=15){
			this->state = 2;
		}
		break;

//	白から白に入る場合
	case 2:
		if(this->white-light <=10){
			Motor_stop(this->left_motor,true);
			Motor_stop(this->right_motor,true);
			return true;
		}
		return false;
		break;

	default:
		return false;

		}
	return false;
	}

double Calibrator_get_boundary(Calibrator* this){
	return this->boundary;
}

double Calibrator_get_white(Calibrator* this){
		return this->white;
	}
