/*
 *  Sumo_Action.h
 *
 *  Created on: 2017/07/07
 *  Author: Hiroki
 */


/*
Rcource.cから呼び出される。
引数を使って、その値がもし
    「1」なら「押し出し」
    「2」なら「寄り切り」
を行う
*/
#ifndef SUMO_ACTION_H_
#define SUMO_ACTION_H_


#include "ev3api.h"
#include "kernel.h"

#include "BasicRun.h"
#include "D_Motor.h"

typedef struct Sumo_Action{
 	int state;
 	BasicRun* basicrun;
  Motor* arm_motor;
  Motor* tail_motor;
}Sumo_Action;

//イニシャライズ
void Sumo_Action_init(Sumo_Action* this, BasicRun* basicrun, Motor* arm_motor, Motor* tail_motor);
//相撲開始
int Start_Sumo(Sumo_Action* this, int mode, int counter);

#endif /* SERECT_BLOCK_H_ */
