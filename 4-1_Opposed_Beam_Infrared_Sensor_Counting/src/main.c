/****************************************************************************/ /**
 * @file   main.c
 * @brief  Main application for count sensor demonstration
 * 
 * @author Maverick Pi
 * @date   2025-08-04 21:13:06
 ********************************************************************************/

#include "OLED.h"
#include "CountSensor.h"

/**
 * @brief Main program entry point
 * 
 * @return int Program status (not used in embedded systems)
 */
int main(void)
{
    // Initialize peripherals
    OLED_Init();        // Initialize OLED display
    CountSensor_Init();     // Initialize count sensor

    // Show static label on OLED
    OLED_ShowString(1, 1, "Count:");

    while (1) {
        // Continuously update displayed count value
        OLED_ShowNum(1, 7, CountSensor_GetCount(), 5);
    }
}
