/*
 *  Haiiro.c
 *
 *  Created on: 2017/08/12
 *  Author: Hiroki
 */

#include "Gray_detection.h"

//新幹線の場所を調べて通過可能か判断する
//「1」なら新幹線を超音波センサで感知してから通過
//「2」なら1秒待機してから通過
//「3」ならすぐに通過する
void Gray_detection_init(Gray_detection* this, ColorSensor* colorsensor){
  this -> colorsensor = colorsensor;
  this -> count = 0;
  this -> state = 0;
  this -> Color = 0;
}


//灰色検知パターン1（輝度値を用いて40以下が40以上カウントされれば灰色検知）
int Gray_detection_do(Gray_detection* this){
  this -> Color = ColorSensor_getColor(this -> colorsensor);
  //int f=ColorSensor_getColor(&colorsensor)
  if(this -> count > 40){
    return 1;
  }
  else{
    if(this -> Color == 2){
      this -> count++;
    }
    else{
      this -> count = 0;
    }
  }
  return 0;
}

/*
//灰色検知パターン2（輝度値をとって前回との差分が10以上なら灰色検知）
//PIDのtarget_Vが28ぐらいじゃないと正確に動作しない可能性・・・
int Gray_detection_do(Gray_detection* this){
  if(this -> state == 0){
    this -> be_Color = this -> Color;
    this -> state = 1;
  }
  else{
    this -> Color = ColorSensor_getReflectBrightness(this -> colorsensor);
    this -> state = 0;
    if(this -> count != 0){
      if(this -> be_Color - this -> Color > 10){
        return 1;
      }
    }
    else{
      this -> count++;
    }
  }
  return 0;
}
*/
