/*
 *  Color_Comparison.c
 *
 *  Created on: 2017/07/12
 *  Author: Hiroki
 */

 #include "Color_Comparison.h"

//初期化を行う（要らなそう）
/*
 void Color_Comparison_init(Color_Co* this, colorid_t Dohyo_Color, colorid_t Block_Color){
  this -> Dohyo_Color = COLOR_NONE;
  this -> Block_Color = COLOR_NONE;
 }
 */

//土俵の色とブロックの色を比較する
int Color_Comparison(colorid_t Dohyo_Color,colorid_t Block_Color){
  //「state == 1」なら「押し出し」
  if(Dohyo_Color != Block_Color){
    return 1;
  }
  //「state == 2」なら「寄り切り」
  else if(Dohyo_Color == Block_Color){
    return 2;
  }
  return 0;
}
