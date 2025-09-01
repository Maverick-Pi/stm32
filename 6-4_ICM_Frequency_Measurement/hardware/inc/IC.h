/****************************************************************************/ /**
 * @file   IC.h
 * @brief  Input Capture
 * 
 * @author Maverick Pi
 * @date   2025-08-31 23:07:59
 ********************************************************************************/

#ifndef __IC_H__
#define __IC_H__

#include "stm32f10x.h"

void IC_Init(void);
uint32_t IC_GetFreq(void);

#endif // !__IC_H__
