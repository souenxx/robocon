/*
 * LPF_LPF.h
 *
 *  Created on: 2016/07/13
 *      Author: Takenao
 */

//記述者 大塚 建直
#ifndef LPF_LPF_H_
#define LPF_LPF_H_


#include "ev3api.h"
#include "kernel.h"



 typedef struct LPF_LPF{
 	int present_lightdetector_level; //現在の明度
 	int last_LPF_output_level; //前回のLPFの出力
 	double parameter_A; //パラメータA（ 出力 ＝ 今回の入力 * (1−A) + 前回の出力 * A )
}LPF_LPF;


//関数
void lpf_init(LPF_LPF* this);
int lpf_calc(LPF_LPF* this,int present_lightdetector_level);

#endif /* LPF_LPF_H_ */
