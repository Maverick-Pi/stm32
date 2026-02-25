/****************************************************************************/ /**
 * @file   LED.c
 * @brief  LED control implementation
 * 
 * Implements GPIO-based LED control functions including initialization,
 * state setting, and toggling.
 * 
 * @author Maverick Pi
 * @date   2025-08-02 19:00:01
 ********************************************************************************/

#include "LED.h"

/**
 * @brief Initialize LED GPIO pins
 * 
 * Enables GPIOA clock and configures PA1 and PA2 as push-pull outputs.
 * Initializes both LEDs to OFF state.
 */
void LED_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2);   // Turn off LEDs
}

/**
 * @brief Set LED state
 * 
 * @param ON_OFF Desired state (LED_ON or LED_OFF)
 * @param GPIO_Pin Target pin (GPIO_Pin_1 or GPIO_Pin_2)
 */
void LED_Control(uint8_t ON_OFF, uint16_t GPIO_Pin)
{
    if (ON_OFF == LED_ON) GPIO_ResetBits(GPIOA, GPIO_Pin);  // Set pin low to turn on LED
    else GPIO_SetBits(GPIOA, GPIO_Pin);     // Set pin high to turn off LED
}

/**
 * @brief Toggle LED state
 * 
 * @param GPIO_Pin Target pin (GPIO_Pin_1 or GPIO_Pin_2)
 */
void LED_Turn(uint16_t GPIO_Pin)
{
    if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin)) GPIO_ResetBits(GPIOA, GPIO_Pin); // If currently off, turn on
    else GPIO_SetBits(GPIOA, GPIO_Pin);     // If currently on, turn off
}
