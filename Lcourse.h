#ifndef Lcourse_H_
#define Lcourse_H_

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#include "Linetracer.h"
#include "EdgeChanger.h"
#include "LgameCalc.h"
#include "LgameMove.h"
#include "Dijk.h"
#include "PID_PID.h"
#include "D_Motor.h"

#define M 999
#define T 1
#define F -1

typedef struct Lcourse{
  int   mState;
  int   mFlag_S2;
  int   mTimer;
  int   mHaveBlock;
  //int	Block_Color;

  int   mRoute[7];
  int   mLength;
  int   mLeng;
  //colorid_t Block_Color;
  int	curNode;
  int	curBlock;
  int	mmState;

  Linetracer  *mLinetracer;
  EdgeChanger *mEdgechanger;
  LgameCalc   *mLgameCalc;
  LgameMove   *mLgameMove;
  Dijk        *mDijk;
  BasicRun    *mbasicRun;
  PID         *mPid;
  Motor		  *arm_motor,*right_motor,*left_motor;

}Lcourse;

void Lcourse_init(Lcourse* this, Linetracer* linetracer, EdgeChanger* edgechanger,
                  LgameCalc* lgamecalc, LgameMove* lgamemove, Dijk* dijk, BasicRun* basicrun, PID* pid, Motor* arm_motor, Motor* left_motor, Motor* right_motor);
int  Lcourse_task(Lcourse* this);

#endif
