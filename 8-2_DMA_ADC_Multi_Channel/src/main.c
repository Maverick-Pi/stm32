/****************************************************************************/ /**
 * @file   main.c
 * @brief  Main application file for ADC data acquisition and display system
 * 
 * This file contains the main application code that initializes the OLED display
 * and ADC peripherals, then continuously reads and displays analog values from
 * four ADC channels on the OLED screen.
 * 
 * @author Maverick Pi
 * @date   2025-09-19 21:44:22
 *******************************************************************************/

#include "OLED.h"
#include "Delay.h"
#include "ADC.h"

int main(void)
{
    OLED_Init();
    AD_Init();

    OLED_ShowString(1, 1, "ADC1:");
    OLED_ShowString(2, 1, "ADC2:");
    OLED_ShowString(3, 1, "ADC3:");
    OLED_ShowString(4, 1, "ADC4:");

    while (1) {
        /* Update display with current ADC values for all four channels */
        for (int i = 0; i < sizeof(AD_Value) / sizeof(AD_Value[0]); ++i) {
            OLED_ShowNum(i + 1, 6, AD_Value[i], 5);
        }

        Delay_ms(100);
    }
}
