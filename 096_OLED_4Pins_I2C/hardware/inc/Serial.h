/****************************************************************************/ /**
 * @file   Serial.h
 * @brief  Header file for USART serial communication functions
 * 
 * @author Maverick Pi
 * @date   2025-10-13 14:55:41
 ********************************************************************************/

#ifndef __SERIAL_H__
#define __SERIAL_H__

#include "stm32f10x.h"
#include <stdio.h>
#include <stdarg.h>

#define SERIAL_BAUDRATE     115200

void Serial_Init(void);     // Initialize USART1 peripheral
void Serial_SendByte(uint8_t b);    // Send single byte
void Serial_SendArray(uint16_t *arr, uint16_t len);     // Send array of 16-bit values
void Serial_SendString(char *str);  // Send null-terminated string
void Serial_SendNumber(uint32_t num, uint8_t len);  // Send numeric value as ASCII
void Serial_Printf(char *format, ...);  // Custom printf implementation
uint8_t Serial_GetRxFlag(void);     // Check if new data has been received
uint8_t Serial_GetRxData(void);     // Get the received data byte

#endif // !__SERIAL_H__
