/*
 * Driver.c
 *
 *  Created on: 2016/07/12
 *      Author: Chihiro
 */

 #include <stdlib.h>
 #include <math.h>

 #include "Driver.h"

/*
 * 初期化
 */
void Driver_init(Driver* this, Motor* leftmotor, Motor* rightmotor){
	this->mLeftmotor = leftmotor;
	this->mRightmotor = rightmotor;
  this -> radius = 0;
}

/*
 * 鶴田
 * モータの出力を計算する
 */
void Driver_calcPower(Driver* this, double pid, int edge, int save){

	/*
	 * 現在の旋回半径の逆数を取得 (左右の車輪の出力比から計算)
	 * 目標の旋回半径の逆数 = 現在の旋回半径の逆数 + pid値
	 * 基準速度(両方の車輪出力が100を超えないよう考慮した値)に左右の比率をかけて、車輪の出力を決定
   */

   //現在のモータの値を取得
	 int lm_power = Motor_getPower(this -> mLeftmotor);
   int rm_power = Motor_getPower(this -> mRightmotor);

   // 導出した計算式から、旋回半径の逆数を計算(そのままだと分母が0になる可能性がある)
   double radius_inv = (2 / L) * (rm_power - lm_power) / (rm_power + lm_power);

	// 目標の旋回半径の逆数
	if(save == 1){
		radius_inv += pid;
	}else{
		//radius_inv += pid * 0.9;
    radius_inv += pid;
	}
  this -> radius = radius_inv;
	//-16 < radius_inv < 16
	if(radius_inv >= MAX_RADIUS){
		radius_inv = MAX_RADIUS;
	}else if(radius_inv < -MAX_RADIUS ){
		radius_inv = -MAX_RADIUS;
	}

	// 車体の速度vを計算
	int robo_v;
	if(save == 1){
		robo_v = 25;
	}else if(save == 0){
    if(abs(radius_inv) > 10.0){
			robo_v = 25;
		}else if(abs(radius_inv) > 7.5){
			robo_v = 30;
		}else if(abs(radius_inv) > 5.0){
			robo_v = 35;
		}else if(abs(radius_inv) > 2.5){
			robo_v = 45;
		}else{
    	robo_v = 60;
    }
  }else{
    robo_v = 15;     //駐車時の超低速
  }




	// 内輪の速度
	int X = (int)(1. - abs(radius_inv) * L / 2.) * robo_v;
	// 外輪の速度
	int Y = (int)(1. + abs(radius_inv) * L / 2.) * robo_v;

	// 左折中か右折中かで出力を左右の車輪に充てる
	if(radius_inv >= 0){
		lm_power = Y;
		rm_power = X;
	}else{
		lm_power = X;
		rm_power = Y;
	}

  //モータに出力
	Motor_setPower(this->mLeftmotor, lm_power);
	Motor_setPower(this->mRightmotor, rm_power);
}
