/****************************************************************************/ /**
 * @file   Timer.c
 * @brief  Timer2 implementation for external clock counting
 * 
 * Configures Timer2 to count external pulses on PA0 (ETR input)
 * 
 * @author Maverick Pi
 * @date   2025-08-05 19:03:10
 ********************************************************************************/

#include "Timer.h"

/**
 * @brief Initialize Timer2 with external clock source
 * 
 * Configures:
 *  - GPIO PA0 as input pull-up for external signal
 *  - Timer2 in external clock mode 2 (ETR pin)
 *  - 16-bit upcounter with period 10 (overflow every 10 pulses)
 *  - Update interrupt on overflow
 *  - NVIC interrupt configuration
 */
void Timer_Init(void)
{
    // Enable peripheral clocks
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);    // Timer2 clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   // GPIOA clock

    // Configure PA0 as input with pull-up
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;       // Input pull-up
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;           // PA0 (ETR input)
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   // I/O speed
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Configure external clock source mode
    TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0x00);

    // Configure timer base settings
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     // No clock division
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;     // Up-counting
    TIM_TimeBaseInitStructure.TIM_Period = 10 - 1;  // Count 10 pulses per overflow (auto-reload value)
    TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;    // No internal prescaler
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;    // Not used in basic timers
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    // Clear pending update flag
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);

    // Enable timer update interrupt (overflow event)
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); 

    // Configure NVIC
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;     // Timer2 interrupt channel
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     // Enable interrupt
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   // Priority group
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  // Sub-priority
    NVIC_Init(&NVIC_InitStructure);

    // Start Timer2
    TIM_Cmd(TIM2, ENABLE);
}

/**
 * @brief Read current Timer2 counter value
 * 
 * @return uint16_t Current counter value (0-9 in this configuration)
 */
uint16_t Timer_GetCounter(void)
{
    return TIM_GetCounter(TIM2);
}

// void TIM2_IRQHandler(void)
// {
//     if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {
//         TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//     }
// }
