/*
 *  Train_Timer.c
 *
 *  Created on: 2017/08/12
 *  Author: Hiroki
 */

 #include "Train_Timer.h"

 double ki_time = 18.6;

//新幹線の場所を調べて通過可能か判断する
//returnする値が
//「1」なら新幹線を超音波センサで感知してから通過
//「2」なら1秒待機してから通過
//「3」ならすぐに通過する
int Train_Timer(int place, int timer){
  double eq_time1 = timer * 0.004;
  double eq_time2 = eq_time1 / ki_time;
  double eq_time3 = eq_time2 - (int)eq_time2;
  double use_time = eq_time3 * ki_time;
  //新幹線が現在どの位置にいるか確認
  int train_place = Train_Place(use_time);

  //1つ目の場所の時
  if(place == 1){
    //新幹線を超音波センサで感知してから通るようにする
    if(11 <= train_place && train_place <= 16){return 1;}
    //1秒待機してから通過する
    else if(17 <= train_place && train_place <= 21){return 2;}
    //新幹線を超音波センサで感知してから通るようにする
    else if(26 <= train_place && train_place <= 32){return 1;}
    //1秒待機してから通過する
    else if(33 <= train_place && train_place <= 37){return 2;}
    //すぐに通過する
    else{return 3;}
  }
  //2つ目の場所の時
  else if(place == 2){
    //新幹線を超音波センサで感知してから通るようにする
    if(19 <= train_place && train_place <= 24){return 1;}
    //1秒待機してから通過する
    else if(25 <= train_place && train_place <= 29){return 2;}
    //すぐに通過する
    else{return 3;}
  }
  //3つ目の場所の時
  else if(place == 3){
    //新幹線を超音波センサで感知してから通るようにする
    if(37 <= train_place && train_place <= 38){return 1;}
    //新幹線を超音波センサで感知してから通るようにする
    else if(1 <= train_place && train_place <= 5){return 1;}
    //1秒待機してから通過する
    else if(6 <= train_place && train_place <= 10){return 2;}
    //すぐに通過する
    else{return 3;}
  }
  return 0;
}


//レールに番号をふり、現在新幹線がどこにいるかを返す
int Train_Place(int use_time){
  if(0 <= use_time && use_time < 0.55){return 1;}
  else if(0.55 <= use_time && use_time < 1.1){return 2;}
  else if(1.1 <= use_time && use_time < 1.5){return 3;}
  else if(1.5 <= use_time && use_time < 1.9){return 4;}
  else if(1.9 <= use_time && use_time < 2.45){return 5;}
  else if(2.45 <= use_time && use_time < 3.0){return 6;}
  else if(3.0 <= use_time && use_time < 3.55){return 7;}
  else if(3.55 <= use_time && use_time < 3.95){return 8;}
  else if(3.95 <= use_time && use_time < 4.35){return 9;}
  else if(4.35 <= use_time && use_time < 4.9){return 10;}
  else if(4.9 <= use_time && use_time < 5.45){return 11;}
  else if(5.45 <= use_time && use_time < 6.0){return 12;}
  else if(6.0 <= use_time && use_time < 6.55){return 13;}
  else if(6.55 <= use_time && use_time < 6.95){return 14;}
  else if(6.95 <= use_time && use_time < 7.35){return 15;}
  else if(7.35 <= use_time && use_time < 7.9){return 16;}
  else if(7.9 <= use_time && use_time < 8.45){return 17;}
  else if(8.45 <= use_time && use_time < 9.0){return 18;}
  else if(9.0 <= use_time && use_time < 9.4){return 19;}
  else if(9.4 <= use_time && use_time < 9.8){return 20;}
  else if(9.8 <= use_time && use_time < 10.35){return 21;}
  else if(10.35 <= use_time && use_time < 10.75){return 22;}
  else if(10.75 <= use_time && use_time < 11.15){return 23;}
  else if(11.15 <= use_time && use_time < 11.7){return 24;}
  else if(11.7 <= use_time && use_time < 12.25){return 25;}
  else if(12.25 <= use_time && use_time < 12.8){return 26;}
  else if(12.8 <= use_time && use_time < 13.2){return 27;}
  else if(13.2 <= use_time && use_time < 13.6){return 28;}
  else if(13.6 <= use_time && use_time < 14.15){return 29;}
  else if(14.15 <= use_time && use_time < 14.55){return 30;}
  else if(14.55 <= use_time && use_time < 14.95){return 31;}
  else if(14.95 <= use_time && use_time < 15.5){return 32;}
  else if(15.5 <= use_time && use_time < 16.05){return 33;}
  else if(16.05 <= use_time && use_time < 16.6){return 34;}
  else if(16.6 <= use_time && use_time < 17.0){return 35;}
  else if(17.0 <= use_time && use_time < 17.4){return 36;}
  else if(17.4 <= use_time && use_time < 17.95){return 37;}
  else if(17.95 <= use_time && use_time <= 18.6){return 38;}
  return 0;
}
