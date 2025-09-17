/****************************************************************************/ /**
 * @file   ADC.h
 * @brief  Header file for Analog-to-Digital Converter (ADC) driver.
 * 
 * This file contains the function prototypes and necessary definitions for 
 * initializing the ADC peripheral and reading conversion results.
 * 
 * @author Maverick Pi
 * @date   2025-09-15 21:32:21
 ********************************************************************************/

#ifndef __ADC_H__
#define __ADC_H__

#include "stm32f10x.h"

void AD_Init(void);
uint16_t AD_GetValue(uint8_t ADC_Channel);

#endif // !__ADC_H__
