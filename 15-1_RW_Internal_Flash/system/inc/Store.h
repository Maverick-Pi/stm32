/****************************************************************************/ /**
 * @file   Store.h
 * @brief  Header for non-volatile storage management using STM32 internal Flash.
 *         Defines storage base address, size, and function prototypes.
 * 
 * @author Maverick Pi
 * @date   2025-12-05 19:05:38
 ********************************************************************************/

#ifndef __STORE_H__
#define __STORE_H__

#include "stm32f10x.h"
#include "InternalFlash.h"

#define STORE_START_ADDR FLASH_PAGE_63  // Base address in Flash (page 63)
#define STORE_COUNT 512     // Number of 16-bit words to store

extern uint16_t Store_Data[];   // Global array mirroring stored data

/* Initialize storage: load data or format if needed */
void Store_Init(void);
/* Save the entire Store_Data array to Flash */
void Store_Save(void);
/* Clear all stored data except the flag */
void Store_Clear(void);

#endif // !__STORE_H__
