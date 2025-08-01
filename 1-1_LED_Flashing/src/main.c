/****************************************************************************/ /**
 * @file   main.c
 * @brief  LED Blink Program for STM32F10x
 * 
 * Controls an LED connected to GPIOA Pin0 with 1Hz blinking pattern.
 * Hardware configuration:
 *  - LED anode connected to PA0 with current-limiting resistor (active-high)
 *  - LED cathode connected to ground
 *  - Alternatively: LED cathode to PA0 with resistor VCC (active-low)
 * Timing: 500ms ON, 500ms OFF (1Hz frequency)
 * 
 * @author Maverick Pi
 * @date   2025-08-01 16:21:29
 ********************************************************************************/

#include "stm32f10x.h"
#include "Delay.h"

/**
 * @brief Main program entry point
 * 
 * Initializes GPIOA Pin0 as output and executes LED blinking routine.
 * 
 * @return int Program execution status (not used in embedded environment)
 */
int main(void)
{
    /* Enable GPIOA peripheral clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /* Configure GPIOA Pin0 as push-pull output */
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;  // Push-pull output mode
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_0;        // Select Pin 0 (PA0)
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;  // Maximum output speed
    GPIO_Init(GPIOA, &GPIO_InitStruct);             // Apply configuration

    /* Main execution loop */
    while (1) {
        /* Activate LED (active-high) / Deactivate (active-low) */
        GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);    // Set PA0 output low
        Delay_ms(500);                                  // Maintain state for 500ms

        /* Deactivate LED (active-high) / Activate (active-low) */
        GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);      // Set PA0 output high
        Delay_ms(500);                                  // Maintain state for 500ms
    }
}
