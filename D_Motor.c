/* モータに関するプログラム

プログラム担当：明石　啓司郎*/

#include "D_Motor.h"


void Motor_init(Motor* this, int outputport,motor_type_t type){
	this -> outputport = outputport;
	ev3_motor_config(this->outputport,type);
	ev3_motor_stop(this->outputport,true);
}
//モーターの角位置を取得する
int32_t Motor_getAngle(Motor* this){
	return ev3_motor_get_counts(this->outputport);
}
//モーターのパワーを取得する
int Motor_getPower(Motor* this){
	return ev3_motor_get_power(this->outputport);
}
//モータの角位置をゼロにリセットする
ER Motor_resetAnglecount(Motor* this){
	return ev3_motor_reset_counts(this->outputport);
}
//モーターを指定した角度だけ回転させる
ER Motor_setRotatedegree(Motor* this, int degrees,uint32_t speed_abs,bool_t blocking){
	return ev3_motor_rotate	(this->outputport,degrees,speed_abs,blocking);
}
//モーターのパワーを設定する
ER Motor_setPower(Motor* this, int power){
	return ev3_motor_set_power(this->outputport,power);
}
//モーターを停止する brakeモードで固定停止か、稼働停止かを指定できる trueがブレーキ,falseがフロート
ER Motor_stop(Motor* this,bool_t brake){
	return ev3_motor_stop(this->outputport,brake);
}
