/****************************************************************************/ /**
 * @file   main.c
 * @brief  LED running light implementation for STM32F10x
 * 
 * Controls 8 LEDs connected to GPIOA in a sequential running pattern.
 * Hardware: LEDs connected to PA0-PA7 (active-low configuration).
 * Timing: Each LED stays lit for 100ms before moving to the next position.
 * 
 * @author Maverick Pi
 * @date   2025-07-31 22:31:24
 ********************************************************************************/

#include "stm32f10x.h"
#include "Delay.h"

/**
 * @brief Main program entry point
 * 
 * Initializes GPIOA pins for LED control and executes running light pattern.
 * 
 * @return int Program status (not used in embedded loop)
 */
int main(void)
{
    /* Enable clock for GPIOA peripheral */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /* Configuration GPIOA pins 0-7 as push-pull outputs */
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;   // Push-pull output mode
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_All;        // Use all pins d(PA0-PA7)
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;  // High-speed output
    GPIO_Init(GPIOA, &GPIO_InitStruct);             // Apply configuration

    /* Main superloop (runs indefinitely) */
    while (1) {
        /* Cycle through each LED position (0-7) */
        for (int i = 0; i < 8; ++i) {
            /**
             * Set GPIOA output state:
             * - Inverts one-hot pattern (0x0001 << i) to create active-low signal
             * - Example: When i=0 -> ~(0x0001) = 0xFFFE (PA0 low, others high)
             * - Hardware: Low signal lights the LED
             */
            GPIO_Write(GPIOA, ~(0x0001 << i));
            /* Maintain current LED state for 100ms */
            Delay_ms(100);
        }
    }
}
