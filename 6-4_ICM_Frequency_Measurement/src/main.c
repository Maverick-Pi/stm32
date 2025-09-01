/****************************************************************************/ /**
 * @file   main.c
 * @brief  Input Capture Mode for Frequency Measurement
 * 
 * @author Maverick Pi
 * @date   2025-08-25 22:46:22
 ********************************************************************************/

#include "OLED.h"
#include "PWM.h"
#include "IC.h"

/**
 * @brief 
 * 
 * @return int 
 */
int main(void)
{
    OLED_Init();
    PWM_Init();
    IC_Init();

    OLED_ShowString(1, 1, "Freq:     Hz");
    
    PWM_SetPrescaler(720 - 1);  // f = 72M / (PSC + 1) / 100
    PWM_SetCompare1(50);    // duty = CCR / 100

    while (1) {
        OLED_ShowNum(1, 6, IC_GetFreq(), 5);
    }
}
