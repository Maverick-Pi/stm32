/****************************************************************************/ /**
 * @file   W25Q64.h
 * @brief  W25Q64 Flash Memory driver header
 * 
 * @author Maverick Pi
 * @date   2025-11-20 21:03:28
 ********************************************************************************/

#ifndef __W25Q64_H__
#define __W25Q64_H__

#include "Soft_SPI.h"
#include "W25Q64_Ins.h"

void W25Q64_Init(void);
void W25Q64_ReadID(uint8_t* MID, uint16_t* DID);
void W25Q64_PageProgram(uint32_t addr, uint8_t *dataArr, uint16_t len);
void W25Q64_EraseSector(uint32_t addr);
void W25Q64_EraseChip(void);
void W25Q64_EraseBlock64K(uint32_t addr);
void W25Q64_EraseBlock32K(uint32_t addr);
void W25Q64_ReadData(uint32_t addr, uint8_t* dataArr, uint32_t len);

#endif // !__W25Q64_H__
