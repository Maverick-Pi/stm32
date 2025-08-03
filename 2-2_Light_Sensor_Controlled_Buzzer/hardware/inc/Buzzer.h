/****************************************************************************/ /**
 * @file   Buzzer.h
 * @brief  Buzzer Control Module Header
 * 
 * Defines interface for buzzer operations including initialization and state
 * control. Provides macros for buzzer ON/OFF states.
 * 
 * @author Maverick Pi
 * @date   2025-08-03 13:44:07
 ********************************************************************************/

#ifndef __BUZZER_H__
#define __BUZZER_H__

#include "stm32f10x.h"

#define BUZZER_ON   0   ///< Buzzer activation state
#define BUZZER_OFF  1   ///< Buzzer deactivation state

void Buzzer_Init(uint16_t GPIO_Pin);
void Buzzer_Control(uint8_t ON_OFF, uint16_t GPIO_Pin);

#endif // !__BUZZER_H__
