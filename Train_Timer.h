/*
 *  Train_Timer.h
 *
 *  Created on: 2017/08/12
 *  Author: Hiroki
 */


/*
Rcource.cから呼び出される。
引数（スタートからの経過時間）を使って、
新幹線がどの位置に居るかを返す
ロボットが今どの位置にいるかも引数で持ってきて
線路を通過可能なら「1」を返す
不可能なら「0」を返す
*/
#ifndef TRAIN_TIMER_H_
#define TRAIN_TIMER_H_

#include "ev3api.h"
#include "kernel.h"

/*
typedef struct Sumo_Action{
 	int time;
  int place;
}Train_Timer;
*/

//イニシャライズ
//void Train_Timer_init(Train_Timer* this);
//新幹線の場所を調べて通過可能か判断する
int Train_Timer(int place, int timer);
//新幹線の場所を示す
int Train_Place(int use_time);

#endif /* TRAIN_TIMER_H_ */
