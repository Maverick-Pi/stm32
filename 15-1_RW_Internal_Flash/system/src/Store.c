/****************************************************************************/ /**
 * @file   Store.c
 * @brief  Non-volatile storage manager using STM32 internal Flash.
 *         Manages a 16-bit array that mirrors Flash content.
 * 
 * @author Maverick Pi
 * @date   2025-12-05 19:05:28
 ********************************************************************************/

#include "Store.h"

/* Global array mirroring Flash content */
uint16_t Store_Data[STORE_COUNT];

/**
 * @brief Initialize storage system.
 *        - If the first word (flag) is not 0xA5A5, format the page.
 *        - Load Flash content into Store_Data array.
 */
void Store_Init(void)
{
    /* Check if storage has been initialized (flag = 0xA5A5) */
    if (InternalFlash_ReadHalfWord(STORE_START_ADDR) != 0xA5A5) {
        /* Format: erase page, write flag, and clear all data words */
        InternalFlash_PageErase(STORE_START_ADDR);
        InternalFlash_WriteHalfWord(STORE_START_ADDR, 0xA5A5);

        for (uint16_t i = 1; i < STORE_COUNT; i++) {
            InternalFlash_WriteHalfWord(STORE_START_ADDR + i * 2, 0x0000);
        }
    }

    /* Load all stored words from Flash into RAM array */
    for (uint16_t i = 0; i < STORE_COUNT; i++) {
        Store_Data[i] = InternalFlash_ReadHalfWord(STORE_START_ADDR + i * 2);
    }
}

/**
 * @brief Save the entire Store_Data array to Flash.
 *        This erases the page and rewrites all data.
 */
void Store_Save(void)
{
    InternalFlash_PageErase(STORE_START_ADDR);
    for (uint16_t i = 0; i < STORE_COUNT; i++) {
        InternalFlash_WriteHalfWord(STORE_START_ADDR + i * 2, Store_Data[i]);
    }
}

/**
 * @brief Clear all stored data (except the flag) and save to Flash.
 * 
 */
void Store_Clear(void)
{
    /* Set all data words to 0 (keep flag at index 0 unchanged) */
    for (uint16_t i = 1; i < STORE_COUNT; i++) {
        Store_Data[i] = 0x0000;
    }
    /* Commit changes to Flash */
    Store_Save();
}
