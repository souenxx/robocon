#ifndef _LGAMECALC_H_  //2重インクルードを防ぐ処理．
#define _LGAMECALC_H_

#include <stdlib.h>
#include <math.h>

#include "Dijk.h"

#define M 999
#define T 1
#define F -1

typedef struct LgameCalc{
  int   mBlock[4];       //4つのカラーブロックの初期位置
  int	mPower;			 //パワーブロックの位置情報
  int   mCarry[4];      //運び終わったかどうか

  int   mRoute[7];
  int   mLength;

  int	code;

  Dijk  *mDijk;
  //Bluetooth   mBluetooth;
}LgameCalc;

void LgameCalc_init(LgameCalc* this, Dijk* dijk);
int LgameCalc_getcode(LgameCalc* this,int init_code);
void calc_code(LgameCalc* this);
void calc_ChangeBlack(LgameCalc *this);
void LgameCalc_foundnextnode(LgameCalc* this, int start);
void LgameCalc_HAVEFLAG(LgameCalc *this, int block);
int  LgameCalc_getroute(LgameCalc* this, int* Route, int* length);
void LgameCalc_updBlock(LgameCalc* this, int curBlock, int curNode);
int LgameCalc_curBlock(LgameCalc *this, int curNode);
int LgameCalc_FINAL_Move(LgameCalc* this);
void LgameCalc_foundfinalnode(LgameCalc* this, int start);

#endif
