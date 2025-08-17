/****************************************************************************/ /**
 * @file   Motor.h
 * @brief  Header file for DC motor control module
 * 
 * This module provides interface functions for controlling a DC motor using
 * GPIO for direction and PWM for speed regulation. It abastracts motor control
 * operations including initialization and speed/direction setting.
 * 
 * @author Maverick Pi
 * @date   2025-08-16 22:55:52
 ********************************************************************************/

#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "PWM.h"

void Motor_Init(void);
void Motor_SetSpeed(int8_t speed);

#endif // !__MOTOR_H__
