/*
 * BasicRun_L.h
 *
 *  Created on: 2016/08/24
 *      Author: Fukunaga
 */

#ifndef BASICRUNL_H
#define BASICRUNL_H
#define SET 5

#include "D_Motor.h"
#include "D_ColorSensor.h"
#include "Light_Lightdetector.h"
#include "Calibrator.h"
#include "D_TouchSensor.h"
#include <stdlib.h>
#include <math.h>

//1度あたりのcm
//一周25.5cm/360語
#define del (31.4/360.0)//理論値0.0708
//旋回時の速度（仮）
#define CurveSpeed_L 20
// #define basepivot_L 295//90度　理論値277.2
// #define baseangle_L 138.6//90度　その場旋回用  理論値138.6

#define basepivot_L 300//90度　理論値277.2
#define baseangle_L 133.5//90度　その場旋回用  理論値138.6



typedef struct BasicRun_L
{
	Motor *leftMotor;
	Motor *rightMotor;
	ColorSensor *colorsensor;
	Light_Lightdetector *lightdetector;
	TouchSensor *touchsensor;
	Calibrator *calibrator;
  int bound;
	int bound_count;

	int count_tama_state;
	int tama_count;
	int curve_tama_state;
	double tamangle;
	int white_tama;
	int roll_tama_state;
	int fukki_state;

	int state;
	int state_count;
	int state_vert;
	int count_vert;
	int count;
	int ang;

	int initangle;
	int Speed;
	int detect_color;

	float delta;
	float pivot90;
	float angle90;

}BasicRun_L;

void BasicRun_L_init(BasicRun_L* this,TouchSensor* touchsensor, Motor* leftMotor, Motor* rightMotor, ColorSensor* colorsensor, Light_Lightdetector* lightdetector, Calibrator* calibrator);

//運搬時用の後から追加したmethod
//速度変更
void BasicRun_L_speedchange(BasicRun_L* this, int speed);
//各変数の変更
void BasicRun_L_paramchange(BasicRun_L* this);
//1が返ってくる前に他のmethodを使うことをやめたら必ず呼ぶ
void BasicRun_L_reset(BasicRun_L* this);
Motor *BasicRun_L_getMotor(BasicRun_L* this,int rightPivot);//private
//rightPivotが1で右軸で左車輪が動く
int BasicRun_L_pivot(BasicRun_L* this, int angle, int rightPivot);
//その場旋回
int BasicRun_L_Curve(BasicRun_L* this, int angle);
int BasicRun_L_GoStraight(BasicRun_L* this, double cm, int color, int colorStop);

//線と並行になるよ！！
int vertCurve(BasicRun_L* this, int leftflag);//
int countup(BasicRun_L* this);

void set_bound_count(BasicRun_L* this, int count);

int get_bound_count(BasicRun_L* this);

void reset_tama(BasicRun_L* this);
int get_tama(BasicRun_L* this);
int count_tama(BasicRun_L* this);
int tamaCurve(BasicRun_L* this,int base_count, int tama);
int Line_fukki(BasicRun_L* this,int base_count,int tama);
int Tama_special(BasicRun_L* this, int tama);
#endif
