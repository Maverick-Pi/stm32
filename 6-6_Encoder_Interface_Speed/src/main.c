/****************************************************************************/ /**
 * @file   main.c
 * @brief  This file contains the main function that initializes the OLED display,
 *         Timer, and Encoder modules. It continuously reads the encoder value and
 *         displays the speed on the OLED screen.
 * 
 * @author Maverick Pi
 * @date   2025-09-09 22:40:42
 ********************************************************************************/

#include "OLED.h"
#include "Encoder.h"
#include "Timer.h"

int16_t Speed; // Variable to store the speed or position value from the encoder

int main(void)
{
    // Initialize the OLED display, Timer, and Encoder hardware
    OLED_Init();
    Timer_Init();
    Encoder_Init();

    // Display the "Speed:" label on the OLED screen
    OLED_ShowString(1, 1, "Speed:");

    while (1) {
        // Continuously display the encoder speed (or position) on the OLED screen
        OLED_ShowSignedNum(1, 7, Speed, 5);
    }
}

/**
 * @brief Timer 2 interrupt handler. It is triggered on Timer 2 update events.
 *        The interrupt reads the encoder value and updates the speed variable.
 * 
 */
void TIM2_IRQHandler(void)
{
    // Check if the interrupt flag is set for Timer 2 update event
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {
        // Read the encoder value and store it in the Speed variable
        Speed = Encoder_Get();

        // Clear the interrupt pending bit for Timer 2
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
