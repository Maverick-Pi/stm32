/****************************************************************************/ /**
 * @file   MyI2C.h
 * @brief  Software I2C Implementation Header File
 * 
 * @author Maverick Pi
 * @date   2025-11-14 16:14:56
 ********************************************************************************/

#ifndef __MYI2C_H__
#define __MYI2C_H__

#include "stm32f10x.h"
#include "Delay.h"

void MyI2C_Init(void);
void MyI2C_Start(void);
void MyI2C_Stop(void);
void MyI2C_SendByte(uint8_t byte);
uint8_t MyI2C_ReceiveByte(void);
void MyI2C_SendAck(uint8_t ack);
uint8_t MyI2C_ReceiveAck(void);

#endif // !__MYI2C_H__
