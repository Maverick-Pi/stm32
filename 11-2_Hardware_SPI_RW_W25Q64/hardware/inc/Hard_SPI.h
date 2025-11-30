/****************************************************************************/ /**
 * @file   Hard_SPI.h
 * @brief  Hardware SPI driver for STM32F10x - Header file
 * 
 * @author Maverick Pi
 * @date   2025-11-23 15:37:26
 ********************************************************************************/

#ifndef __HARD_SPI_H__
#define __HARD_SPI_H__

#include "stm32f10x.h"

/* SPI1 Pin Definitions */
#define SPI1_CS_PIN     GPIO_Pin_4   // PA4 - Chip Select
#define SPI1_SCK_PIN    GPIO_Pin_5   // PA5 - Serial Clock
#define SPI1_MISO_PIN   GPIO_Pin_6   // PA6 - Master In Slave Out
#define SPI1_MOSI_PIN   GPIO_Pin_7   // PA7 - Master Out Slave In

void Hard_SPI_Init(void);
void Hard_SPI_Start(void);
void Hard_SPI_Stop(void);
uint8_t Hard_SPI_TransferByte(uint8_t data);
void Hard_SPI_TransferContinuous(uint8_t *pTxData, uint8_t *pRxData, uint16_t size);

#endif // !__HARD_SPI_H__
