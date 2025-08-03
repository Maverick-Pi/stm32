/****************************************************************************/ /**
 * @file   Buzzer.c
 * @brief  Buzzer Control Module Implementation
 * 
 * Implements GPIO initialization and control functions for buzzer operation.
 * Configures specified pin on GPIOB as push-pull output with default OFF state.
 * 
 * @author Maverick Pi
 * @date   2025-08-03 13:43:53
 ********************************************************************************/

#include "Buzzer.h"

/**
 * @brief Initializes the buzzer GPIO pin as push-pull output
 * 
 * @param GPIO_Pin Specifies the GPIO pin to initialize
 */
void Buzzer_Init(uint16_t GPIO_Pin)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_SetBits(GPIOB, GPIO_Pin);  // Initialize buzzer in OFF state
}

/**
 * @brief Controls the buzzer state (ON/OFF)
 * 
 * @param ON_OFF Specifies the desired state (BUZZER_ON or BUZZER_OFF)
 * @param GPIO_Pin Specifies the GPIO pin to control
 */
void Buzzer_Control(uint8_t ON_OFF, uint16_t GPIO_Pin)
{
    if (ON_OFF == BUZZER_ON) GPIO_ResetBits(GPIOB, GPIO_Pin_12);    // Activate buzzer
    else GPIO_SetBits(GPIOB, GPIO_Pin_12);  // Deactivate buzzer
}
