/****************************************************************************/ /**
 * @file   main.c
 * @brief  Main application to display timer counter and overflow count on OLED
 * 
 * @author Maverick Pi
 * @date   2025-08-06 21:29:01
 ********************************************************************************/

#include "OLED.h"
#include "Timer.h"

uint16_t Num;   // Global variable to count timer overflow events

/**
 * @brief Main application entry point
 * 
 * Initializes peripherals and runs main display loop
 * 
 * @return int Program status (not used in embedded context)
 */
int main(void)
{
    // Initialize peripherals
    OLED_Init();    // Set up OLED display
    Timer_Init();   // Configure Timer2 for external clock mode

    // Display static labels on OLED
    OLED_ShowString(1, 1, "Num:");  // Show overflow count label
    OLED_ShowString(2, 1, "CNT:");  // Show raw counter value label

    // Main application loop
    while (1) {
        // Continuously update display values
        OLED_ShowNum(1, 5, Num, 5);     // Display overflow count (5 digits)
        OLED_ShowNum(2, 5, Timer_GetCounter(), 5);  // Display current timer count (5 digits)
    }
}

/**
 * @brief Timer2 interrupt service routine
 * 
 * Handles timer overflow events and increments global counter
 */
void TIM2_IRQHandler(void)
{
    // Check if update (overflow) interrupt occurred
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {
        Num++;  // Increment overflow counter
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);     // Clear interrupt flag
    }
}
