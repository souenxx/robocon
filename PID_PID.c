/*
 * PID_PID.c
 *
 *  Created on: 2016/07/13
 *      Author: Yuki
 */

//記述者 狩野 悠貴
//p:0.3, i:0.0005, d:0.5
#include "PID_PID.h"

void PID_init(PID* this ,Calibrator* calibrator){
	this -> target_V = Calibrator_get_boundary(calibrator);
	this -> en_2 = 0;
	this -> en_1 = 0;
	this -> MVn_1 = 0;
	this -> MVn = 0;
	this -> kp = 0.36;//ほぼ確定 0.33
	this -> ki = 0.0056;//0.0055
	this -> kd = 0.027;
	this -> light_level = 42;
}

//PIDのパrメータを走行前の状態に戻す
void PID_reset(PID* this){
	this -> target_V = TARGET_V;
	this -> en_2 = 0;
	this -> en_1 = 0;
	this -> MVn_1 = 0;
	this -> MVn = 0;
	this -> kp = 0.36;//ほぼ確定  0.3
	this -> ki = 0.0056;//0.0005
	this -> kd = 0.027; //0.5
}

//Driver_calcPower(安全走行)の開始時に一回だけ呼ぶ
void PID_saveParam(PID* this){
	PID_reset(this);
	this -> kp = 0.4;
	this -> ki = 0.0001;
	this -> kd = 5.0;
}

//灰色検知の際に起動する
void PID_gray_detection(PID* this){
	this -> target_V = 30;//ちょっと黒よりの値でいいんじゃ？
	this -> en_2 = 0;
	this -> en_1 = 0;
	this -> MVn_1 = 0;
	this -> MVn = 0;
	this -> kp = 0.36;//ほぼ確定
	this -> ki = 0.0055;//0.0005
	this -> kd = 0.027;//0.5
	this -> light_level = 42;
}



double PID_calc(PID* this, int light_v, int edge){
	//int MVn;	//今回の操作量
	double dMVn;	//今回の操作量差分
	int en; 	//今回の偏差

	this -> light_level = light_v;

	en = this -> target_V - light_v;

	dMVn = this -> kp * (en - this -> en_1) + this -> ki * en
			+ this -> kd * (en + this -> en_2 - 2*this -> en_1);

	this->MVn = this -> MVn_1 + dMVn;


	//if(this->MVn <= -100)		this->MVn = -100;	//最小値
	//else if(this->MVn >= 100)	this->MVn = 100;	//最大値

	//渡す値は補正値にするかどうか？


	this -> en_2 = this -> en_1;
	this -> en_1 = en;
	this -> MVn_1 = this->MVn;


	if(edge == 0){
		return this->MVn * -1;
	}else{
		return this->MVn;
	}

}

void PID_set_target(PID* this,double target_v){
	this->target_V=target_v;
}
