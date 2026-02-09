/****************************************************************************/ /**
 * @file   CAN_Hardware.h
 * @brief  CAN 硬件驱动头文件
 *         提供 CAN 控制器初始化、发送、接收等功能的接口声明
 * 
 * @author Maverick Pi
 * @date   2026-02-02 19:18:46
 ********************************************************************************/

#ifndef __CAN_HARDWARE_H__
#define __CAN_HARDWARE_H__

#include "stm32f10x.h"

void CAN_Hardware_Init(void);
void CAN_Hardware_Transmit(CanTxMsg* txMsg);
uint8_t CAN_Hardware_ReceiveFlag(void);
void CAN_Hardware_Receive(CanRxMsg* rxMsg);

#endif // !__CAN_HARDWARE_H__
