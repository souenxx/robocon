/*
 * EdgeChanger.c
 *
 *  Created on: 2016/09/13
 *      Author: Kohei
 */


#include "EdgeChanger.h"

void EdgeChanger_init(EdgeChanger* this, BasicRun* basicRun){
	this->basicRun = basicRun;
}

/*
 * エッジを切り替える
 * 0→左　1→右
 * その後ライン復帰クラス，ライントレースクラス，PIDクラスの構造体にあるエッジ情報を変更する
 */
 int EdgeChange(EdgeChanger* this, int edge){
	 int num=0;
 	if(edge == 1){
 		if(BasicRun_pivot(this->basicRun, 30, 0)==1){
 			num = 1;
 		}
 	}else{
 		if(BasicRun_pivot(this->basicRun, 30, 1)==1){
 			num = 1;
 		}
 	}
	return num;
 }
