/****************************************************************************/ /**
 * @file   ADC.h
 * @brief  ADC1 single-channel (CH0 on PA0) driver for STM32F10x.
 * 
 * This header exposes a minimal interface to initialize the ADC peripheral
 * and retrieve a single 12-bit conversion value using software trigger.
 * 
 * Hardware assumptions:
 * - Device family: STM32F10x (StdPeriph Library API)
 * - Analog input: PA0 -> ADC1_IN0
 * - VREF+ = 3.3V (used by the example to convert counts to volts)
 * 
 * @author Maverick Pi
 * @date   2025-09-15 21:32:21
 ********************************************************************************/

#ifndef __ADC_H__
#define __ADC_H__

#include "stm32f10x.h"

void AD_Init(void);
uint16_t AD_GetValue(void);

#endif // !__ADC_H__
