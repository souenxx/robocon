/*
 *  Sumo_Action.c
 *
 *  Created on: 2017/07/07
 *  Author: Hiroki
 */

 #include "Sumo_Action.h"
//初期化を行う
 void Sumo_Action_init(Sumo_Action* this, BasicRun* basicrun, Motor* arm_motor, Motor* tail_motor){
 	this -> state = 0;
 	this -> basicrun = basicrun;
  this -> arm_motor = arm_motor;
  this -> tail_motor = tail_motor;
 }

//相撲を行う
//mode == 1:押し出し　2:寄り切り
//counter : 今何個目を動かしてるか
int Start_Sumo(Sumo_Action* this, int mode, int counter){
  //「mode == 1」なら「押し出し」
  if(mode == 1){
    //真っ直ぐに進む
    if(this -> state == 0){
      if(BasicRun_GoStraight(this -> basicrun,13) == 1){
        this -> state = 1;
      }
    }
    //押し出した後，後方に進む（8個目だけ違う）
    else if(this -> state == 1){
      if(counter == 8){
        if(BasicRun_GoStraight(this -> basicrun,-15) == 1){
          this -> state = 2;
        }
      }
      else{
        if(BasicRun_GoStraight(this -> basicrun,-22) == 1){
          this -> state = 2;
        }
      }
    }
    //反対方向を向く
    else if(this -> state == 2){
      if(counter == 8){
        if(BasicRun_Curve(this -> basicrun,-180) == 1){
          this -> state = 3;
        }
      }
      else if(counter == 1 || counter == 3 || counter == 5 || counter == 7){
        if(BasicRun_Curve(this -> basicrun,165) == 1){
          this -> state = 3;
        }
      }
      else{
        if(BasicRun_Curve(this -> basicrun,-175) == 1){
          this -> state = 3;
        }
      }
    }
    //state = 0にして1を返す
    else if(this -> state == 3){
      this -> state = 0;
      return 1;
    }
  }
  //「mode == 2」なら「寄り切り」
  else if(mode == 2){
    //後方に少し移動
    if(this -> state == 0){
      if(BasicRun_GoStraight(this -> basicrun,-2) == 1){
        this -> state = 4;
      }
    }
    //反対方向を向く（ブロックを尻尾で移動）
    else if(this -> state == 4){
      if(counter == 1 || counter == 3 || counter == 5 || counter == 7){
        if(BasicRun_Curve(this -> basicrun,165) == 1){
          this -> state = 5;
        }
      }
      else{
        if(BasicRun_Curve(this -> basicrun,-180) == 1){
          this -> state = 5;
        }
      }
    }
    //尻尾を回転させてブロックをさらに動かす
    else if(this -> state == 5){
      if(counter == 1 || counter == 3 || counter == 5 || counter == 7){
        Motor_setRotatedegree(this -> tail_motor, 360, 20, true);
        this -> state = 6;
      }
      else{
        Motor_setRotatedegree(this -> tail_motor, -360, 20, true);
        this -> state = 6;
      }
    }
    //少し前進
    else if(this -> state == 6){
      if(counter == 8){
        this -> state = 7;
      }
      else{
        if(BasicRun_GoStraight(this -> basicrun,7) == 1){
          this -> state = 7;
        }
      }
    }
    //state = 0にして1を返す
    else if(this -> state == 7){
      this -> state = 0;
      return 1;
    }
  }
  return 0;
}
