/*
 * Driver.h
 *
 *  Created on: 2016/07/12
 *      Author: Chihiro
 */

 #ifndef DRIVER_H
 #define DRIVER_H

 #include "D_Motor.h"

 #define MAX_POWER 100
 #define MIN_POWER 0
 #define L 0.125
 #define MAX_RADIUS 16.0

 typedef struct Driver{
 	Motor *mLeftmotor;
 	Motor *mRightmotor;
  double radius;
 }Driver;

void Driver_init(Driver* this, Motor* leftmotor, Motor* rightmotor);
void Driver_calcPower(Driver* this, double pid, int edge, int save);

#endif
