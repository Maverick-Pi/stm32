/****************************************************************************/ /**
 * @file   Encoder.c
 * @brief  This file contains functions for initializing and interacting with
 *         the encoder hardware interface using Timer 3 and GPIO pins PA6 and PA7.
 *         It configures the encoder interface to read pulse signals and calculate
 *         the position or speed.
 * 
 * @author Maverick Pi
 * @date   2025-09-10 18:18:48
 ********************************************************************************/

#include "Encoder.h"

/**
 * @brief Initializes the encoder hardware by configuring the required GPIO pins
 *        and Timer 3 for input capture in encoder mode. Enables the clocks for 
 *        GPIOA and Timer 3, configures GPIO pins PA6 and PA7 as input pull-up, 
 *        and set up Timer 3 for encoder mode.
 * 
 */
void Encoder_Init(void)
{
    // Enable clock for GPIOA and Timer 3
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    // Configure GPIOA pins PA6 and PA7 as input pull-up
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Configure Timer 3 for encoder input capture mode
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

    // Configure Timer 3 input capture for both channels (1 and 2)
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICFilter = 0xF;     // Filter value for noise reduction
    TIM_ICInit(TIM3, &TIM_ICInitStructure);

    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICFilter = 0xF;
    TIM_ICInit(TIM3, &TIM_ICInitStructure);

    // Configure Timer 3 for encoder mode with rising edge polarity
    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

    // Start Timer 3
    TIM_Cmd(TIM3, ENABLE);
}

/**
 * @brief Retrieves the current encoder position by reading the counter value from
 *        Timer 3. The counter is then reset to 0.
 * 
 * @return int16_t The current position value from the encoder.
 */
int16_t Encoder_Get(void)
{
    int16_t temp = TIM_GetCounter(TIM3); // Read the current value of the counter
    TIM_SetCounter(TIM3, 0);    // Reset the counter for the next reading
    return temp;    // Return the position value
}
