/****************************************************************************/ /**
 * @file   IC.c
 * @brief  Input Capture module for measuring frequency and duty cycle using TIM3.
 * 
 * This module configures TIM3 channel 1 on GPIOA pin 6 in input capture mode.
 * It enables frequency and duty cycle measurement of an external PWM signal
 * using the PWM Input (PWMI) configuration.
 * 
 * @author Maverick Pi
 * @date   2025-08-31 23:08:59
 ********************************************************************************/

#include "IC.h"

/**
 * @brief  Initialize TIM3 Input Capture.
 * 
 * - Enables clocks for TIM3 and GPIOA.
 * - Configures PA6 as input with pull-up resistor.
 * - Sets TIM3 in PWM Input Mode to capture both period (CH1) and duty cycle (CH2).
 * - Uses internal clock, prescaler of 72 -> timer tick = 1 μs.
 * - Configures slave reset mode triggered by TI1 rising edge.
 * - Starts TIM3.
 */
void IC_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   // Input with pull-up
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;       // TIM3_CH1 on PA6
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TIM_InternalClockConfig(TIM3);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;   // 72MHz / 72 = 1 MHz
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;    // Capture channel 1
    TIM_ICInitStructure.TIM_ICFilter = 0xF;             // Max input filter
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;   // No prescaler
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_PWMIConfig(TIM3, &TIM_ICInitStructure);

    TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);    // Trigger on TI1 edge

    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);     // Reset counter on trigger

    TIM_Cmd(TIM3, ENABLE);  // Enable TIM3
}

/**
 * @brief Get the frequency of the input signal.
 * 
 * Formula:
 *  Freq = 1,000,000 / (Period + 1)
 * where timer clock = 1 MHz.
 * 
 * @return uint32_t Frequency in Hz.
 */
uint32_t IC_GetFreq(void)
{
    return 1000000 / (TIM_GetCapture1(TIM3) + 1);
}

/**
 * @brief Get the duty cycle of the input signal.
 * 
 * Formula:
 *  Duty = (HighTime / Period) * 100
 * where HighTime = Capture2, Period = Capture1
 * 
 * @return uint32_t Duty cycle in percentage (0-100%)
 */
uint32_t IC_GetDuty(void)
{
    return (TIM_GetCapture2(TIM3) + 1) * 100 / (TIM_GetCapture1(TIM3) + 1);
}
