/****************************************************************************/ /**
 * @file   CountSensor.h
 * @brief  Header file for Count Sensor module
 * 
 * @author Maverick Pi
 * @date   2025-08-04 18:58:25
 ********************************************************************************/

#ifndef __COUNTSENSOR_H__
#define __COUNTSENSOR_H__

#include "stm32f10x.h"
#include "Delay.h"

void CountSensor_Init(void);
uint16_t CountSensor_GetCount(void);

#endif // !__COUNTSENSOR_H__
