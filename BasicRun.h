/*
 * BasicRun.h
 *
 *  Created on: 2016/08/24
 *      Author: Fukunaga
 */

#ifndef BASICRUN_H
#define BASICRUN_H

#include "D_Motor.h"
#include <stdlib.h>
#include <math.h>

//1度あたりのcm
//一周25.5cm/360語
#define del (31.4/360.0)//理論値0.0708
//旋回時の速度（仮）
#define CurveSpeed 20
#define basepivot 295//90度　理論値277.2
#define baseangle 133.5//90度　その場旋回用  理論値138.6





typedef struct BasicRun
{
	Motor *leftMotor;
	Motor *rightMotor;

	int state;
	int initangle;
	int Speed;
	float delta;
	float pivot90;
	float angle90;

} BasicRun;

void BasicRun_init(BasicRun* this, Motor* leftMotor, Motor* rightMotor);

//運搬時用の後から追加したmethod
//速度変更
void BasicRun_speedchange(BasicRun* this, int speed);
//各変数の変更
void BasicRun_paramchange(BasicRun* this);
//1が返ってくる前に他のmethodを使うことをやめたら必ず呼ぶ
void BasicRun_reset(BasicRun* this);
Motor *BasicRun_getMotor(BasicRun* this,int rightPivot);//private
//rightPivotが1で右軸で左車輪が動く
int BasicRun_pivot(BasicRun* this, int angle, int rightPivot);
//その場旋回
int BasicRun_Curve(BasicRun* this, int angle);
int BasicRun_GoStraight(BasicRun* this, int cm);
int BasicRun_Tama(BasicRun* this, int cm);


#endif
