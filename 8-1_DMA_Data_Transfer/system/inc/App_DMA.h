/****************************************************************************/ /**
 * @file   App_DMA.h
 * @brief  Header file for DMA application layer
 * 
 * Contains function declarations for DMA initialization and transfer operations
 * 
 * @author Maverick Pi
 * @date   2025-09-19 15:28:58
 ********************************************************************************/

#ifndef __APP_DMA_H__
#define __APP_DMA_H__

#include "stm32f10x.h"

void App_DMA_Init(uint32_t addrA, uint32_t addrB, uint16_t size);
void App_DMA_Transfer(void);

#endif // !__APP_DMA_H__
