/****************************************************************************/ /**
 * @file   Servo.h
 * @brief  Header file for servo motor control using PWM signals
 * 
 * @author Maverick Pi
 * @date   2025-08-16 20:52:49
 ********************************************************************************/

#ifndef __SERVO_H__
#define __SERVO_H__

#include "PWM.h"

void Servo_Init(void);
void Servo_SetAngle(float angle);

#endif // !__SERVO_H__
