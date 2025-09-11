/****************************************************************************/ /**
 * @file   Encoder.h
 * @brief  
 * 
 * @author Maverick Pi
 * @date   2025-09-10 18:19:01
 ********************************************************************************/

#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "stm32f10x.h"

void Encoder_Init(void);
int16_t Encoder_Get(void);

#endif // !__ENCODER_H__
