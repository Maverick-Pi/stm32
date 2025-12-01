/****************************************************************************/ /**
 * @file   Key.h
 * @brief  Header file for key detection module
 * 
 * Provides initialization and key state reading functions for GPIO-based keys.
 * 
 * @author Maverick Pi
 * @date   2025-08-02 19:46:14
 ********************************************************************************/

#include "stm32f10x.h"

#ifndef __KEY_H__
#define __KEY_H__

void Key_Init(void);
uint8_t Key_GetNum(void);

#endif // !__KEY_H__
