/****************************************************************************/ /**
 * @file   LightSensor.h
 * @brief  Light Sensor Module Header
 * 
 * Defines interface for light sensor operations including initialization and
 * state reading. Configures GPIO pins for light detection using STM32F10x.
 * 
 * @author Maverick Pi
 * @date   2025-08-03 14:40:58
 ********************************************************************************/

#ifndef __LIGHTSENSOR_H__
#define __LIGHTSENSOR_H__

#include "stm32f10x.h"

void LightSensor_Init(uint16_t GPIO_Pin);
uint8_t LightSensor_Get(uint16_t GPIO_Pin);

#endif // !__LIGHTSENSOR_H__
