/*
 * EdgeChanger.h
 *
 *  Created on: 2016/09/13
 *      Author: Kohei
 */

#ifndef EDGECHANGER_H_
#define EDGECHANGER_H_

#include "ev3api.h"
#include "BasicRun.h"

typedef struct EdgeChanger
{
	BasicRun* basicRun;
}EdgeChanger;


void EdgeChanger_init(EdgeChanger* this, BasicRun* basicRun);
int EdgeChange(EdgeChanger* this, int edge);

#endif /* EDGECHANGER_H_ */
