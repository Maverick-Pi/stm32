/****************************************************************************/ /**
 * @file   Servo.c
 * @brief  Implementation of servo motor control functions
 * 
 * @author Maverick Pi
 * @date   2025-08-16 21:08:44
 ********************************************************************************/

#include "Servo.h"

/**
 * @brief Initializes servo control system
 * 
 */
void Servo_Init(void)
{
    PWM_Init();
}

/**
 * @brief Sets servo rotation angle
 * 
 * @param angle Desired rotation angle (0-180 degrees)
 */
void Servo_SetAngle(float angle)
{
    PWM_SetCompare2(angle * 4 + 180);
}
