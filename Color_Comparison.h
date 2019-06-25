/*
 *  Color_Comparison.h
 *
 *  Created on: 2017/07/12
 *  Author: Hiroki
 */


/*
Rcource.cから呼び出される。
引数を使って、土俵の色とブロックの色をもらう
それを使用して、同色なら2、異色なら1を返す
*/
#ifndef COLOR_COMPARISON_H_
#define COLOR_COMPARISON_H_


#include "ev3api.h"
#include "kernel.h"

/*
typedef struct Color_Co{
  colorid_t Dohyo_Color;
  colorid_t Block_Color;
}Color_Co;
*/

//イニシャライズ
//void Color_Comparison_init(Color_Co* this, colorid_t Dohyo_Color, colorid_t Block_Color);
//ブロックの色を比較する
int Color_Comparison(colorid_t Dohyo_Color, colorid_t Block_Color);

#endif /* COLOR_COMPARISON_H_ */
