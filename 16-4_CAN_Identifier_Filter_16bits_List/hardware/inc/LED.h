/****************************************************************************/ /**
 * @file   LED.h
 * @brief  Header file for LED control module
 * 
 * Provides LED initialization and control functions including on/off and toggle.
 * 
 * @author Maverick Pi
 * @date   2025-08-02 18:58:37
 ********************************************************************************/

#include "stm32f10x.h"

#ifndef __LED_H__
#define __LED_H__

#define LED_ON      0   ///< Logical state for turning LED on
#define LED_OFF     1   ///< Logical state for turning LED off

void LED_Init(void);
void LED_Control(uint8_t ON_OFF, uint16_t GPIO_Pin);
void LED_Turn(uint16_t GPIO_Pin);

#endif // !__LED_H__
