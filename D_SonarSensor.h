#ifndef UltrasonicSensor_H_
#define UltrasonicSensor_H_

#include "ev3api.h"
#include "kernel.h"


// 属性を保持するための構造体の定義
typedef struct SonarSensor
{
	sensor_port_t inputPort;
} SonarSensor;

void SonarSensor_init(SonarSensor* this,sensor_port_t inputPort);
int16_t SonarSensor_getDistance(SonarSensor* this);
#endif /*!defined(EA_914BE224_8829_4365_AC51_C6B2AB5A8275__INCLUDED_)*/
 
