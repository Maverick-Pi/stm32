/****************************************************************************/ /**
 * @file   main.c
 * @brief  Special Buzzer Sound Pattern on STM32F10x
 * 
 * Controls a buzzer connected to GPIOB Pin12 with a distinctive sound pattern:
 *   - Two short beeps (100ms ON, 100ms OFF) 
 *   - Followed by a long silence (700ms OFF)
 * The complete cycle repeats every 1000ms.
 * 
 * Hardware configuration:
 *   - Buzzer connected to PB12 with driver circuit
 *   - Active-low: Buzzer sounds when PB12 = LOW (Bit_RESET)
 *   - Buzzer silent when PB12 = HIGH (Bit_SET) 
 * 
 * @author Maverick Pi
 * @date   2025-08-01 22:44:39
 ********************************************************************************/

#include "stm32f10x.h"
#include "Delay.h"

/**
 * @brief Main program entry point
 * 
 * Initializes GPIOB Pin12 as output and executes buzzer sound pattern.
 * 
 * @return int Program execution status (not used in embedded environment)
 */
int main(void)
{
    /* Enable GPIOB peripheral clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    /* Configure GPIOB Pin12 as push-pull output */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; // Push-pull output mode
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;      // Select Pin 12 (PB12)
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // Maximum output speed
    GPIO_Init(GPIOB, &GPIO_InitStructure);            // Apply configuration

    /* Main execution loop - runs continuously */
    while (1) {
        /* First short beep (100ms activation) */
        GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_RESET); // Buzzer ON (active-low)
        Delay_ms(100);

        /* First gap between beeps (100ms silence) */
        GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET); // Buzzer OFF
        Delay_ms(100);

        /* Second short beep (100ms activation) */
        GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_RESET); // Buzzer ON
        Delay_ms(100);

        /* Long silent period (700ms off - completes 1000ms cycle) */
        GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET); // Buzzer OFF
        Delay_ms(700);
    }
}
