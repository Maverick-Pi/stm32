/****************************************************************************/ /**
 * @file   main.c
 * @brief  Demonstration of PWM generation and measurement using OLED display.
 * 
 * This program:
 * - Initializes OLED display, PWM output, and TIM3 Input Capture.
 * - Generates a PWM signal on a configured channel.
 * - Measures the frequency and duty cycle of the PWM signal.
 * - Displays results dynamically on OLED.
 * 
 * @author Maverick Pi
 * @date   2025-09-01 17:06:02
 ********************************************************************************/

#include "OLED.h"
#include "PWM.h"
#include "IC.h"

/**
 * @brief Main program entry points.
 * 
 * @return int (never returns in practice)
 */
int main(void)
{
    OLED_Init();
    PWM_Init();
    IC_Init();

    OLED_ShowString(1, 1, "Freq:     Hz");
    OLED_ShowString(2, 1, "Duty:  %");

    PWM_SetPrescaler(7200 - 1);  // Freq = 72M / (PSC + 1) / 100
    PWM_SetCompare1(80);    // Duty = CCR / 100

    while (1) {
        OLED_ShowNum(1, 6, IC_GetFreq(), 5);
        OLED_ShowNum(2, 6, IC_GetDuty(), 2);
    }
}
