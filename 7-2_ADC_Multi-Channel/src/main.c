/****************************************************************************/ /**
 * @file   main.c
 * @brief  Main program for reading and displaying multiple ADC channels on OLED.
 * 
 * This file contains the main application which initializes the OLED display
 * and ADC peripheral, then continuously reads four ADC channels and displays
 * their values on the OLED screen.
 * 
 * @author Maverick Pi
 * @date   2025-09-17 11:41:52
 ********************************************************************************/

#include "OLED.h"
#include "ADC.h"
#include "Delay.h"

int main(void)
{
    uint16_t AD0, AD1, AD2, AD3; // Variables to hold ADC conversion results

    OLED_Init();    // Initialize the OLED display
    AD_Init(); // Initialize the ADC peripheral

    // Display static channel labels on the OLED
    OLED_ShowString(1, 1, "ADC0:");
    OLED_ShowString(2, 1, "ADC1:");
    OLED_ShowString(3, 1, "ADC2:");
    OLED_ShowString(4, 1, "ADC3:");

    while (1) {
        // Read analog values from four different ADC channels
        AD0 = AD_GetValue(ADC_Channel_0);
        AD1 = AD_GetValue(ADC_Channel_1);
        AD2 = AD_GetValue(ADC_Channel_2);
        AD3 = AD_GetValue(ADC_Channel_3);

        // Update the OLED display with the new ADC values
        OLED_ShowNum(1, 6, AD0, 5);
        OLED_ShowNum(2, 6, AD1, 5);
        OLED_ShowNum(3, 6, AD2, 5);
        OLED_ShowNum(4, 6, AD3, 5);

        Delay_ms(100);  // Delay for 100ms to control refresh rate
    }
}
