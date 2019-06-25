/*
 *  Gray_detection.h
 *
 *  Created on: 2017/08/12
 *  Author: Hiroki
 */

/*
Rcource.cから呼び出される。
輝度値を取得して灰色検知する
*/
#ifndef GRAY_DETECTION_H_
#define GRAY_DETECTION_H_

#include "ev3api.h"
#include "kernel.h"
#include "D_ColorSensor.h"

typedef struct Gray_detection{
  int count;
  int state;
  uint8_t be_Color;
  uint8_t Color;
  ColorSensor* colorsensor;
}Gray_detection;


//イニシャライズ
void Gray_detection_init(Gray_detection* this, ColorSensor* colorsensor);
//灰色検知を行う
int Gray_detection_do(Gray_detection* this);

#endif /* GRAY_DETECTION_H_ */
