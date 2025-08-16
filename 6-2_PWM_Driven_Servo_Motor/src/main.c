/****************************************************************************/ /**
 * @file   main.c
 * @brief  Main application for PWM-driven servo motor control with OLED display
 * 
 * @author Maverick Pi
 * @date   2025-08-16 12:54:09
 ********************************************************************************/

#include "Servo.h"
#include "Key.h"
#include "OLED.h"

int main(void)
{
    float angle = 0;

    PWM_Init();
    Key_Init();
    Servo_Init();
    OLED_Init();

    OLED_ShowString(1, 1, "Servo Angle:");

    while (1) {
        if (Key_GetNum() == 1) {
            angle += 30;
            if (angle > 180) {
                angle = 0;
            }
        }
        Servo_SetAngle(angle);
        OLED_ShowNum(1, 13, angle, 3);
    }
}
