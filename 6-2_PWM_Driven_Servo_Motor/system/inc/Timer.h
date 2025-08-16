/****************************************************************************/ /**
 * @file   Timer.h
 * @brief  Timer2 configuration and interface header
 * 
 * Provides functions for timer initialization and counter value access
 * 
 * @author Maverick Pi
 * @date   2025-08-05 19:02:55
 ********************************************************************************/

#ifndef __TIMER_H__
#define __TIMER_H__

#include "stm32f10x.h"

void Timer_Init(void);
uint16_t Timer_GetCounter(void);

#endif // !__TIMER_H__
