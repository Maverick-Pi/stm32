/****************************************************************************/ /**
 * @file   PWM.h
 * @brief  PWM configuration interface for STM32F10x
 * 
 * Provides initialization and control functions for Timer 2 Channel 1 PWM output.
 * Targets GPIO Pin A0 (PA0) as PWM output pin.
 * 
 * @author Maverick Pi
 * @date   2025-08-13 14:28:32
 ********************************************************************************/

#ifndef __PWM_H__
#define __PWM_H__

#include "stm32f10x.h"

void PWM_Init(void);
void PWM_SetCompare1(uint16_t compare);
void PWM_SetPrescaler(uint16_t Prescaler);

#endif // !__PWM_H__
