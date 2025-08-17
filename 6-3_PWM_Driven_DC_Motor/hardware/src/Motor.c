/****************************************************************************/ /**
 * @file   Motor.c
 * @brief  Implementation of DC motor control module
 * 
 * This module implements motor control using:
 *  - GPIO PA4/PA5 for H-bridge direction control
 *  - Timer PWM output (Channel 3) for speed regulation
 * Speed values are clamped to ±100 range internally
 * 
 * @author Maverick Pi
 * @date   2025-08-16 22:56:22
 ********************************************************************************/

#include "Motor.h"

/**
 * @brief Initialize motor control hardware
 * 
 *  1. Enables GPIOA peripheral clock
 *  2. Configures PA4 and PA5 as push-pull outputs
 *  3. Initializes PWM subsystem for speed control
 */
void Motor_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    PWM_Init();
}

/**
 * @brief Set motor speed and direction
 * 
 * Handles both direction control through GPIO pins and PWM duty cycle setting.
 * Automatically converts negative speeds to absolute values for PWM module.
 * 
 * @param speed Signed speed value (-100 to 100)
 */
void Motor_SetSpeed(int8_t speed)
{
    if (speed >= 0) {
        GPIO_SetBits(GPIOA, GPIO_Pin_4);
        GPIO_ResetBits(GPIOA, GPIO_Pin_5);
        PWM_SetCompare3(speed);
    } else {
        GPIO_SetBits(GPIOA, GPIO_Pin_5);
        GPIO_ResetBits(GPIOA, GPIO_Pin_4);
        PWM_SetCompare3(-speed);
    }
}
