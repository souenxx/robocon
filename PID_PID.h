/*
 * PID_PID.h
 *
 *  Created on: 2016/07/13
 *      Author: Yuki
 */

#include "Calibrator.h"
//狩野 悠貴
#ifndef PID_PID_H_
#define PID_PID_H_

#define TARGET_V 11


//#define kp (8.5)		//8.5	8.5
//#define ki (0.3)		//0.3	0.5
//#define kd (0.075)		//0.075	0.3

typedef struct PID_PID{

//変数
double target_V;	//目標値
int en_2;		//前々回の偏差
int en_1;		//前回の偏差
double MVn_1;		//前回の操作量
double MVn;
double kp;
double ki;
double kd;
int light_level;
}PID;

//関数
void PID_init(PID* this, Calibrator* calibrator);
void PID_reset(PID* this);//PIDのパrメータを走行前の状態に戻す
void PID_saveParam(PID* this);//Driver_calcPower(安全走行)の開始時に一回だけ呼ぶ
void PID_gray_detection(PID* this);
double PID_calc(PID* this, int light_v, int edge);
void PID_set_target(PID* this,double target_v);
#endif /* PID_PID_H_ */
