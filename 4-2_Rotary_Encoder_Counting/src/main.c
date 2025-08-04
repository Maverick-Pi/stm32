/****************************************************************************/ /**
 * @file   main.c
 * @brief  Main application using rotary encoder and OLED display
 * 
 * Demonstrates reading encoder pulses and displaying accumulated count on OLED
 * 
 * @author Maverick Pi
 * @date   2025-08-04 22:18:24
 ********************************************************************************/

#include "OLED.h"
#include "Rotary_Encoder.h"

/**
 * @brief Main application entry point
 * 
 * Initializes OLED display and rotary encoder hardware.
 * Continuously reads encoder count and updates display.
 * 
 * @return int Program status (not used in embedded context)
 */
int main(void)
{
    int16_t num;    // Initialize counter

    OLED_Init();    // Initialize OLED dispaly
    RotaryEncoder_Init();   // Initialize encoder hardware

    OLED_ShowString(1, 1, "Count:");    // Display static label

    while (1) {
        // Accumulate encoder counts and update display
        num += RotaryEncoder_GetCount();
        OLED_ShowSignedNum(1, 7, num, 5);   // Display 5-digit signed number
    }
}
