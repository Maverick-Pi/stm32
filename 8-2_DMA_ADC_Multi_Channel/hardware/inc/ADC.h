/****************************************************************************/ /**
 * @file   ADC.h
 * @brief  Header file for Analog-to-Digital Converter (ADC) module
 * 
 * This file contains the declarations and external references for ADC operations,
 * including global variables and initialization function prototypes.
 * 
 * @author Maverick Pi
 * @date   2025-09-19 22:14:28
 *******************************************************************************/

#ifndef __ADC_H__
#define __ADC_H__

#include "stm32f10x.h"

extern uint16_t AD_Value[4];

void AD_Init(void);

#endif // !__ADC_H__
