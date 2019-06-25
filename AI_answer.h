#ifndef AI_ANSWER_H
#define AI_ANSWER_H

#include "D_Motor.h"
#include "D_ColorSensor.h"
#include "BasicRun.h"
#include "BasicRun_L.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "Linetracer.h"

#define num_edge 10
#define tate 28
#define yoko 15
#define BRACK 1
#define WHITE 2
#define GREEN 3
#define MUGEN 999
#define RIGHTANGLE 91
#define RIGHT 35
#define LEFT -30

typedef struct AI_answer
{
	int state;
	int state2;
	int sumou_state;

	int flag;
	int minus1_zero; //バックか前進かを指定
	int ans_num_deg;
	int ans_num_ana;

  BasicRun* basicrun;
	BasicRun_L* basicrun_L;
	Linetracer* linetracer;
	ColorSensor* colorsensor;

  Motor* left_motor;
  Motor* right_motor;

}AI_answer;

int result;
int result_num;
int hoge;

void AI_answer_init(AI_answer* this, ColorSensor* colorsensor, BasicRun* basicrun, BasicRun_L* basicrun_L, Motor* left_motor, Motor* right_motor, Linetracer* linetracer);

//デジタル数字読み足り関数
int degital_recog(AI_answer* this);

//アナログ数字読み取り関数
int analog_recog();

//相撲を始める関数（駐車は別）
int sumou_do(AI_answer* this, int analog, int degital);
int next_sumou(AI_answer* this); //private
int next_sumou2(AI_answer* this); //private
int next_sumou1(AI_answer* this, int addang); //private

void blockLR(AI_answer* this, int bit,int deg_flag);
//degi -> ans の動き
int degiToAns(AI_answer* this);

//linetrace -> degiの動き
int LineToDegi(AI_answer* this);

void state_reset(AI_answer* this);
void state2_reset(AI_answer* this);
void flag_reset(AI_answer* this);

int blockmove_ana(AI_answer* this, int direction);
int blockmove_deg(AI_answer* this, int direction);

//basicrunテスト用
int basicrunL_test(AI_answer* this);

//vertCurveテスト用
int vertCurve_test(AI_answer* this);

int go_park(AI_answer* this);

#endif
