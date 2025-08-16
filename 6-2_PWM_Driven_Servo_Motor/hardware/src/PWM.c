/****************************************************************************/ /**
 * @file   PWM.c
 * @brief  PWM driver implementation for STM32F10x
 * 
 * Hardware-specific implementation of PWM functions using TIM2 Channel 1.
 * Outputs PWM signal on GPIOA Pin 0 (PA0) with configurable duty cycle.
 * 
 * @author Maverick Pi
 * @date   2025-08-13 14:28:53
 ********************************************************************************/

#include "PWM.h"

/**
 * @brief Initialize TIM2 Channel 1 PWM output on PA0
 * 
 * Clock configuration:
 *  - Enables APB1 clock for TIM2
 *  - Enables APB2 clock for GPIOA
 * 
 * GPIO configuration:
 *  - PA0 configured as alternate function push-pull
 * 
 * Timer configuration:
 *  - Internal clock source
 *  - Up-counting mode
 *  - Period: 100 cycles (100% resolution)
 *  - Prescaler: 720 (72MHz/720 = 100kHz counter clock)
 *  - PWM frequency = 100kHz/100 = 1kHz
 * 
 * Output compare configuration:
 *  - PWM Mode 1 (active while counter < CCR1)
 *  - High polarity (active high)
 *  - Initial duty cycle: 0%
 */
void PWM_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TIM_InternalClockConfig(TIM2);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 7200 - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 200 - 1;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);

    TIM_Cmd(TIM2, ENABLE);
}

/**
 * @brief Set PWM duty cycle for TIM2 Channel 2
 * 
 * Directly sets the capture/compare register (CCR1) value.
 * Duty cycle = (compare / TIM_Period) * 100%
 * 
 * @param compare Pulse width value (0-100) corresponding to duty cycle percentage.
 * Value 0 = 0% duty cycle, Value 100 = 100% duty cycle.
 */
void PWM_SetCompare2(uint16_t compare)
{
    TIM_SetCompare2(TIM2, compare);
}
