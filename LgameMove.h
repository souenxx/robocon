#ifndef _LGAMEMOVE_H_  //2重インクルードを防ぐ処理．
#define _LGAMEMOVE_H_

#include <stdlib.h>
#include <math.h>

#include "D_ColorSensor.h"
#include "D_Motor.h"
#include "BasicRun.h"

#define M 999
#define T 1
#define F -1

typedef struct LgameMove{
  ColorSensor *mColorSensor;
  BasicRun *mBasicRun;
  int small_state;
  Motor* arm_motor;
}LgameMove;

void LgameMove_init(LgameMove* this, ColorSensor* colorsensor, BasicRun* basicrun, Motor* arm_motor);
int LgameMove_nextline(LgameMove* this, int n1, int n2, int n3);
int LgameMove_findcolor(LgameMove* this);
int LgameMove_checkcolor(LgameMove* this, int* color);
#endif
