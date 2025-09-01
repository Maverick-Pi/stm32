/****************************************************************************/ /**
 * @file   IC.c
 * @brief  Input Capture driver implementation using TIM3
 * 
 * This module initializes TIM3 in input capture mode to measure
 * the frequency of an external signal applied to GPIOA Pin 6.
 * 
 * @author Maverick Pi
 * @date   2025-08-31 23:08:59
 ********************************************************************************/

#include "IC.h"

/**
 * @brief Initialize TIM3 for input capture mode on PA6.
 * 
 * Configures GPIOA Pin 6 as input with pull-up, enables TIM3 clock,
 * sets TIM3 base frequency, configures channel 1 in input capture mode
 * to detect rising edges, and enables reset mode for accurate period
 * measurement. Finally, starts TIM3.
 */
void IC_Init(void)
{
    // Enable peripheral clocks for TIM3 and GPIOA
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // Configure PA6 as input with pull-up
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Use internal clock for TIM3
    TIM_InternalClockConfig(TIM3);

    // Configure TIM3 time base: 1 MHz counter frequency
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;   // Max count
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;   // 72 MHz / 72 = 1 MHz
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

    // Configure TIM3 Channel 1 for input capture
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICFilter = 0xF;     // Max filter
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;     // Rising edge trigger
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;   // No prescaler
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInit(TIM3, & TIM_ICInitStructure);

    // Use TI1FP1 as input trigger
    TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);

    // Reset counter on each valid capture
    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);

    // Enable TIM3
    TIM_Cmd(TIM3, ENABLE);
}

/**
 * @brief Get measured frequency from TIM3 input capture.
 * 
 * Uses the captured period (in microseconds) from TIM3 Channel 1
 * and calculates the input frequency in Hz.
 * 
 * @return uint32_t Frequency in Hz
 */
uint32_t IC_GetFreq(void)
{
    return 1000000 / (TIM_GetCapture1(TIM3) + 1);
}
