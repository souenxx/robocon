/*
 * BasicRun.c
 *
 *  Created on: 2016/08/24
 *      Author: Fukunaga
 */

#include "BasicRun.h"
/*
 *注意:すべてのmethodは終わったら1サイクルだけ1を返します.
 *    次のサイクルでまた呼ぶと同じ処理をやり直します.
 *    このクラスのmethodで自分のステートを動かしたい場合
 *    ifでステートを動かすのではなくswitch文での運用をおすすめします.
 */


/*
 * 初期化
 */
void BasicRun_init(BasicRun* this, Motor* leftMotor, Motor* rightMotor){
  this->leftMotor = leftMotor;
	this->rightMotor = rightMotor;
  this->state = 0;
  this->initangle = 0;
  BasicRun_reset(this);

}
//速度変更
void BasicRun_speedchange(BasicRun* this, int speed){
  this->Speed = speed;
}
//各変数の変更  運搬時用
void BasicRun_paramchange(BasicRun* this){
  this->delta = del*0.95;//小さくするほど遠くまで行く
  this->pivot90 = basepivot - 10;//小さくするほど回らない
  this->angle90 = baseangle - 5.5;//小さくするほど回らない
  BasicRun_speedchange(this,10);
}
//再初期化
//1が返ってくる前に他のmethodを使うことをやめたら必ず呼ぶ
void BasicRun_reset(BasicRun* this){
  this->state = 0;
  this->Speed = CurveSpeed;
  this->delta = del;
  this->pivot90 = basepivot;
  this->angle90 = baseangle;
}

//軸に従って適した軸のポインタを返す
Motor *BasicRun_getMotor(BasicRun* this,int rightPivot){
  //右車輪軸回転 左を動かす
  if(rightPivot==0){
    return this->rightMotor;
  }//浸り車輪軸回転
  else{
    return this->leftMotor;
  }
}
//片足を軸に旋回する
//rightPivotが1で右軸で左車輪が動く
//終わったら1を返却する
int BasicRun_pivot(BasicRun* this, int angle, int rightPivot){

  switch(this->state){
    //今の回転角度を取得
    case 0:
      Motor_stop(this->leftMotor,true);
      Motor_stop(this->rightMotor,true);
      this->initangle = Motor_getAngle(BasicRun_getMotor(this,rightPivot));
      this->state = 1;
      break;
    //pivot
    case 1:
      if(angle > 0){
        Motor_setPower(BasicRun_getMotor(this,rightPivot), this->Speed);
      }else{
        Motor_setPower(BasicRun_getMotor(this,rightPivot), -1*this->Speed);
      }
      //指定角度まで進んだら
      float tmp = abs(Motor_getAngle(BasicRun_getMotor(this,rightPivot))-this->initangle);
      if( tmp >= abs(this->pivot90/90.0*angle)  ){
        this->state = 2;
      }
      break;
    case 2://停止
      Motor_stop(this->leftMotor,true);
      Motor_stop(this->rightMotor,true);
      if(Motor_getPower(this->rightMotor) == 0){
        this->state = 3;
      }
      break;
    case 3:
      BasicRun_reset(this);
      return 1;
      break;
    default:
      break;
  }
  return 0;
}

//終わったら1を返却する
//その場旋回
//angleがプラスで右曲がり　マイナスで左曲がり
int BasicRun_Curve(BasicRun* this, int angle){

  switch(this->state){
    //今の回転角度を取得
    case 0:
      Motor_stop(this->leftMotor,true);
      Motor_stop(this->rightMotor,true);
      this->initangle = Motor_getAngle(this->rightMotor);
      this->state = 1;
      break;
    //旋回
    case 1:
      if(angle > 0){//右曲がり
        Motor_setPower(this->rightMotor, -1*this->Speed);
        Motor_setPower(this->leftMotor, this->Speed);
      }else{//左曲がり
        Motor_setPower(this->rightMotor, this->Speed);
        Motor_setPower(this->leftMotor, -1*this->Speed);
      }

      //指定角度まで進んだら
      float tmp = abs(Motor_getAngle(this->rightMotor) - this->initangle);
      if( tmp >= abs(this->angle90/90.0*angle)){
        this->state = 2;
      }
      break;
    case 2://停止
      Motor_stop(this->leftMotor,true);
      Motor_stop(this->rightMotor,true);
      if(Motor_getPower(this->rightMotor) == 0){
        this->state = 3;
      }
      break;
    case 3:
      BasicRun_reset(this);
      return 1;
      break;
    default:
      break;
  }
  return 0;
}


//指定距離　直進する
//終わったら1を返却する
int BasicRun_GoStraight(BasicRun* this, int cm){
  switch(this->state){
    //今の回転角度を取得
    case 0:
      Motor_stop(this->leftMotor,true);
      Motor_stop(this->rightMotor,true);
      this->initangle = Motor_getAngle(this->rightMotor);
      this->state = 1;
      break;
    //直進
    case 1:
      if(cm > 0){
        Motor_setPower(this->rightMotor, (this->Speed));
        Motor_setPower(this->leftMotor, (this->Speed));
      }else{
        Motor_setPower(this->rightMotor, -1*(this->Speed));
        Motor_setPower(this->leftMotor, -1*(this->Speed));
      }
      //指定距離まで進んだら
      float tmp = abs(Motor_getAngle(this->rightMotor) - this->initangle);
      if( tmp*this->delta >= abs(cm)  ){
        this->state = 2;
      }
      break;
    case 2://停止
      Motor_stop(this->leftMotor,true);
      Motor_stop(this->rightMotor,true);
      if(Motor_getPower(this->rightMotor) == 0){
        this->state = 3;
      }
      break;
    case 3:
      BasicRun_reset(this);
      return 1;
      break;
    default:
      break;
  }
  return 0;

}
int BasicRun_Tama(BasicRun* this, int cm){
  switch(this->state){
    //今の回転角度を取得
    case 0:
      Motor_stop(this->leftMotor,true);
      Motor_stop(this->rightMotor,true);
      this->initangle = Motor_getAngle(this->rightMotor);
      this->state = 1;
      break;
    //直進
    case 1:
      if(cm > 0){
        Motor_setPower(this->rightMotor, (this->Speed));
        Motor_setPower(this->leftMotor, (this->Speed)*2/3);
      }else{
        Motor_setPower(this->rightMotor, -1*(this->Speed));
        Motor_setPower(this->leftMotor, -1*(this->Speed)*2/3);
      }
      //指定距離まで進んだら
      float tmp = abs(Motor_getAngle(this->rightMotor) - this->initangle);
      if( tmp*this->delta >= abs(cm)  ){
        this->state = 2;
      }
      break;
    case 2://停止
      Motor_stop(this->leftMotor,true);
      Motor_stop(this->rightMotor,true);
      if(Motor_getPower(this->rightMotor) == 0){
        this->state = 3;
      }
      break;
    case 3:
      BasicRun_reset(this);
      return 1;
      break;
    default:
      break;
  }
  return 0;

}
int BasicRun_Kido(BasicRun* this, int cm){
  switch(this->state){
    //今の回転角度を取得
    case 0:
      Motor_stop(this->leftMotor,true);
      Motor_stop(this->rightMotor,true);
      this->initangle = Motor_getAngle(this->rightMotor);
      this->state = 1;
      break;
    //直進
    case 1:
      if(cm > 0){
        Motor_setPower(this->rightMotor, (this->Speed));
        Motor_setPower(this->leftMotor, (this->Speed)*1/2);
      }else{
        Motor_setPower(this->rightMotor, -1*(this->Speed));
        Motor_setPower(this->leftMotor, -1*(this->Speed)*1/3);
      }
      //指定距離まで進んだら
      float tmp = abs(Motor_getAngle(this->rightMotor) - this->initangle);
      if( tmp*this->delta >= abs(cm)  ){
        this->state = 2;
      }
      break;
    case 2://停止
      Motor_stop(this->leftMotor,true);
      Motor_stop(this->rightMotor,true);
      if(Motor_getPower(this->rightMotor) == 0){
        this->state = 3;
      }
      break;
    case 3:
      BasicRun_reset(this);
      return 1;
      break;
    default:
      break;
  }
  return 0;

}
