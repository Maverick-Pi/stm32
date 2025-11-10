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

#define SERIAL_PACKET_SIZE 64

extern uint8_t Serial_RxPacket[];
extern uint16_t Serial_RxLen;

void Serial_Init(void);     // Initialize USART1 peripheral
void Serial_SendByte(uint8_t b);    // Send single byte
void Serial_SendArray(uint8_t *arr, uint16_t len);     // Send array of 16-bit values
void Serial_SendString(char *str);  // Send null-terminated string
void Serial_SendNumber(uint32_t num, uint8_t len);  // Send numeric value as ASCII
void Serial_Printf(char *format, ...);  // Custom printf implementation
uint8_t Serial_GetRxFlag(void);     // Check if new data has been received
void Serial_SendPacket(uint8_t *txPacket, uint16_t num);  // Send a data packet with predefined structure

#endif // !__SERIAL_H__
