/*
 * Rcourse.h
 *
 *  Created on: 2017/07/06
 *  Author: Hiroki
 */

/*
 App.cから呼び出される。Rcourseの全体となるファイル
*/
#ifndef RCOURSE_H_
#define RCOURSE_H_

#include "ev3api.h"
#include "kernel.h"

#include "BasicRun.h"
#include "D_ColorSensor.h"
#include "D_SonarSensor.h"
#include "D_Motor.h"
#include "D_GyroSensor.h"
#include "Sumo_Action.h"
#include "Color_Comparison.h"
#include "Linetracer.h"
#include "PID_PID.h"
#include "Driver.h"
#include "Light_Lightdetector.h"
#include "Train_Timer.h"
#include "Gray_detection.h"

typedef struct Rcourse{
  colorid_t Dohyo_Color;
 	colorid_t Block_Color;
 	int state;
  int small_state;
 	int counter;
  int timer;
  int line_state;
  int last_action;
  int place;
  int big_timer;
  int distance;

 	BasicRun* basicrun;
 	ColorSensor* colorsensor;
  SonarSensor* sonarsensor;
  Motor* arm_motor;
  Motor* left_motor;
  Motor* right_motor;

  Sumo_Action* sumo_action;
  Linetracer* linetracer;

  PID* pid;
  Driver* driver;
  Light_Lightdetector* lightdetector;
  GyroSensor* gyrosensor;
  Gray_detection* gray_detection;
}Rcourse;

//イニシャライズ
void Start_Sumou_init(Rcourse* this, BasicRun* basicrun, ColorSensor* colorsensor, SonarSensor* sonarsensor, Motor* arm_motor, Sumo_Action* sumo_action, Linetracer* linetracer, PID* pid, Driver* driver, Light_Lightdetector* lightdetector, GyroSensor* gyrosensor, Gray_detection* gray_detection, Motor* left_motor, Motor* right_motor);
//相撲開始
int Start_Sumou(Rcourse* this);

#endif /* RCOURSE_H_ */
