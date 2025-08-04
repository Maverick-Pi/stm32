/****************************************************************************/ /**
 * @file   Rotary_Encoder.h
 * @brief  Header for rotary encoder driver using STM32F10x
 * 
 * Provides interface for initializing rotary encoder and reading pulse count
 * 
 * @author Maverick Pi
 * @date   2025-08-04 22:19:58
 ********************************************************************************/

#ifndef __ROTARY_ENCODER_H__
#define __ROTARY_ENCODER_H__

#include "stm32f10x.h"

void RotaryEncoder_Init(void);
int16_t RotaryEncoder_GetCount(void);

#endif // !__ROTARY_ENCODER_H__
