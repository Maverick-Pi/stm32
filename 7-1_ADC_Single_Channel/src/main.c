/****************************************************************************/ /**
 * @file   main.c
 * @brief  ADC single-channel readout shown on an OLED (via user library).
 * 
 * The program reads ADC1 channel 0 (PA0), converts the raw 12-bit code to a
 * voltage assuming VREF+ = 3.3V, and displays both raw value and voltage on
 * a character OLED. Update period is 100ms.
 * 
 * @author Maverick Pi
 * @date   2025-09-15 21:22:27
 ********************************************************************************/

#include "OLED.h"
#include "ADC.h"
#include "Delay.h"

int main(void)
{
    uint16_t ADValue;   // raw 12-bit ADC code [0..4095]
    float voltage;      // computed voltage in volts

    /* Initialize peripherals */
    OLED_Init();
    AD_Init();

    /* Static labels */
    OLED_ShowString(1, 1, "ADCValue:");
    OLED_ShowString(2, 1, "Voltage:0.00V");

    while (1) {
        /* Acquire one sample */
        ADValue = AD_GetValue();
        /* Convert 12-bit code to volts for VREF+ = 3.3V */
        voltage = ADValue / 4095.0 * 3.3;
        /* Show raw code (5 digits)*/
        OLED_ShowNum(1, 10, ADValue, 5);
        /* Show voltage as x.xx (integer part + two decimals) */
        OLED_ShowNum(2, 9, voltage, 1);     // integer part
        OLED_ShowNum(2, 11, (uint16_t)(voltage * 100) % 100, 2); // fractional part

        Delay_ms(100); // ~10 Hz update rate
    }
}
