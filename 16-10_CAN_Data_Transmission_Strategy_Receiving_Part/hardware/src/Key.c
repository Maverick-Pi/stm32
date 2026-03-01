/****************************************************************************/ /**
 * @file   Key.c
 * @brief  Key detection implementation
 * 
 * Implements key scanning with hardware debouncing for GPIO-connected keys.
 * 
 * @author Maverick Pi
 * @date   2025-08-02 19:48:33
 ********************************************************************************/

#include "Key.h"
#include "Delay.h"

/**
 * @brief Initialize key GPIO pins
 * 
 * Enables GPIOB clock and configures PB1 and PB11 as input pull-up pins.
 */
void Key_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
 * @brief Detect pressed key with debounce
 * 
 * Scans configured key pins with 20ms debounce delay.
 * 
 * @return uint8_t Key number (1 for PB1, 2 for PB11, 0 if none pressed)
 */
uint8_t Key_GetNum(void)
{
    uint8_t keyNum = 0;

    // Check key1 (PB1) with debounce
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0) {
        Delay_ms(20);
        while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0);      // Wait for release
        Delay_ms(20);
        keyNum = 1;
    }

    // Check key2 (PB11) with debounce
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0) {
        Delay_ms(20);
        while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0);     // Wait for release
        Delay_ms(20);
        keyNum = 2;
    }

    return keyNum;
}
