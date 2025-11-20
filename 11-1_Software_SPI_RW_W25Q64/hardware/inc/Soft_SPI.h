/****************************************************************************/ /**
 * @file   Soft_SPI.h
 * @brief  Software-based SPI communication driver header
 * 
 * @author Maverick Pi
 * @date   2025-11-20 15:54:49
 ********************************************************************************/

#ifndef __SOFT_SPI_H__
#define __SOFT_SPI_H__

#include "stm32f10x.h"

void Soft_SPI_Init(void);
void Soft_SPI_Start(void);
void Soft_SPI_Stop(void);
uint8_t Soft_SPI_TransferByte(uint8_t data);

#endif // !__SOFT_SPI_H__
