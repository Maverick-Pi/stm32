/****************************************************************************/ /**
 * @file   InternalFlash.h
 * @brief  Low-level Flash memory driver for STM32F10x.
 *         Defines page addresses and provides read/write/erase functions.
 * 
 * @author Maverick Pi
 * @date   2025-12-05 16:50:50
 ********************************************************************************/

#ifndef __INTERNALFLASH_H__
#define __INTERNALFLASH_H__

#include "stm32f10x.h"

/* Flash memory page addresses (1 KB per page) for STM32F10x */
#define FLASH_PAGE_0   0x08000000
#define FLASH_PAGE_1   0x08000400
#define FLASH_PAGE_2   0x08000800
#define FLASH_PAGE_3   0x08000C00
#define FLASH_PAGE_4   0x08001000
#define FLASH_PAGE_5   0x08001400
#define FLASH_PAGE_6   0x08001800
#define FLASH_PAGE_7   0x08001C00
#define FLASH_PAGE_8   0x08002000
#define FLASH_PAGE_9   0x08002400
#define FLASH_PAGE_10  0x08002800
#define FLASH_PAGE_11  0x08002C00
#define FLASH_PAGE_12  0x08003000
#define FLASH_PAGE_13  0x08003400
#define FLASH_PAGE_14  0x08003800
#define FLASH_PAGE_15  0x08003C00
#define FLASH_PAGE_16  0x08004000
#define FLASH_PAGE_17  0x08004400
#define FLASH_PAGE_18  0x08004800
#define FLASH_PAGE_19  0x08004C00
#define FLASH_PAGE_20  0x08005000
#define FLASH_PAGE_21  0x08005400
#define FLASH_PAGE_22  0x08005800
#define FLASH_PAGE_23  0x08005C00
#define FLASH_PAGE_24  0x08006000
#define FLASH_PAGE_25  0x08006400
#define FLASH_PAGE_26  0x08006800
#define FLASH_PAGE_27  0x08006C00
#define FLASH_PAGE_28  0x08007000
#define FLASH_PAGE_29  0x08007400
#define FLASH_PAGE_30  0x08007800
#define FLASH_PAGE_31  0x08007C00
#define FLASH_PAGE_32  0x08008000
#define FLASH_PAGE_33  0x08008400
#define FLASH_PAGE_34  0x08008800
#define FLASH_PAGE_35  0x08008C00
#define FLASH_PAGE_36  0x08009000
#define FLASH_PAGE_37  0x08009400
#define FLASH_PAGE_38  0x08009800
#define FLASH_PAGE_39  0x08009C00
#define FLASH_PAGE_40  0x0800A000
#define FLASH_PAGE_41  0x0800A400
#define FLASH_PAGE_42  0x0800A800
#define FLASH_PAGE_43  0x0800AC00
#define FLASH_PAGE_44  0x0800B000
#define FLASH_PAGE_45  0x0800B400
#define FLASH_PAGE_46  0x0800B800
#define FLASH_PAGE_47  0x0800BC00
#define FLASH_PAGE_48  0x0800C000
#define FLASH_PAGE_49  0x0800C400
#define FLASH_PAGE_50  0x0800C800
#define FLASH_PAGE_51  0x0800CC00
#define FLASH_PAGE_52  0x0800D000
#define FLASH_PAGE_53  0x0800D400
#define FLASH_PAGE_54  0x0800D800
#define FLASH_PAGE_55  0x0800DC00
#define FLASH_PAGE_56  0x0800E000
#define FLASH_PAGE_57  0x0800E400
#define FLASH_PAGE_58  0x0800E800
#define FLASH_PAGE_59  0x0800EC00
#define FLASH_PAGE_60  0x0800F000
#define FLASH_PAGE_61  0x0800F400
#define FLASH_PAGE_62  0x0800F800
#define FLASH_PAGE_63  0x0800FC00

/* Read 32-bit word */
uint32_t InternalFlash_ReadWord(uint32_t addr);
/* Read 16-bit half-word */
uint16_t InternalFlash_ReadHalfWord(uint32_t addr);
/* Read 8-bit byte */
uint8_t InternalFlash_ReadByte(uint32_t addr);
/* Mass erase entire Flash memory */
void InternalFlash_MassErase(void);
/* Erase a specific Flash memory page */
void InternalFlash_PageErase(uint32_t pageAddr);
/* Write 32-bit word to Flash memory */
void InternalFlash_WriteWord(uint32_t addr, uint32_t data);
/* Write 16-bit half-word to Flash memory */
void InternalFlash_WriteHalfWord(uint32_t addr, uint16_t data);

#endif // !__INTERNALFLASH_H__
