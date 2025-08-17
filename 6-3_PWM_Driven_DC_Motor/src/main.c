/****************************************************************************/ /**
 * @file   main.c
 * @brief  Main application for PWM-driven DC motor controller
 * 
 * Features:
 *  - Interactive speed control using push button
 *  - Real-time speed display on OLED
 *  - Speed range: -100％ to +100％ in 25％ increments
 * 
 * Operation flow:
 *  1. Initialize OLED display and motor control
 *  2. Show static label on OLED
 *  3. Poll button state to cycle through speed values
 *  4. Update motor speed and OLED display continuously
 * 
 * @author Maverick Pi
 * @date   2025-08-16 22:27:38
 ********************************************************************************/

#include "Motor.h"
#include "OLED.h"
#include "Key.h"

int main(void)
{
    int8_t speed = 0;   // Current speed setting (-100 to 100)

    Key_Init();
    OLED_Init();
    Motor_Init();

    OLED_ShowString(1, 1, "Motor Speed:");

    while (1) {
        // Button press increments speed by 25%, wraps from 100% to -100%
        if (Key_GetNum() == 1) {
            speed += 25;
            if (speed > 100) speed = -100;
        }
        OLED_ShowSignedNum(1, 13, speed, 3);    // Update speed display
        Motor_SetSpeed(speed);  // Apply new speed setting
    }
}
