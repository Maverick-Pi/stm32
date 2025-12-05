/****************************************************************************/ /**
 * @file   InternalFlash.c
 * @brief  Implementation of internal Flash memory driver for STM32F10x.
 * 
 * @author Maverick Pi
 * @date   2025-12-05 16:50:46
 ********************************************************************************/

#include "InternalFlash.h"

/**
 * @brief Read a 32-bit word from Flash memory.
 * 
 * @param addr: Flash address (must be word-aligned).
 * @return The 32-bit value at the specified address.
 */
uint32_t InternalFlash_ReadWord(uint32_t addr)
{
    return *(__IO uint32_t*)addr;
}

/**
 * @brief Read a 16-bit half-word from Flash memory.
 * 
 * @param addr: Flash address (must be half-word-aligned).
 * @return The 16-bit value at the specified address.
 */
uint16_t InternalFlash_ReadHalfWord(uint32_t addr)
{
    return *(__IO uint16_t*)addr;
}

/**
 * @brief Read an 8-bit byte from Flash memory.
 * 
 * @param addr: Flash address.
 * @return The 8-bit value at the specified address.
 */
uint8_t InternalFlash_ReadByte(uint32_t addr)
{
    return *(__IO uint8_t*)addr;
}

/**
 * @brief Perform a mass erase of the entire Flash memory.
 */
void InternalFlash_MassErase(void)
{
    FLASH_Unlock();
    FLASH_EraseAllPages();
    FLASH_Lock();
}

/**
 * @brief Erase a specific page in Flash memory.
 * 
 * @param pageAddr: Address of the page to erase (must be page-aligned).
 */
void InternalFlash_PageErase(uint32_t pageAddr)
{
    FLASH_Unlock();
    FLASH_ErasePage(pageAddr);
    FLASH_Lock();
}

/**
 * @brief Write a 32-bit word to Flash memory.
 * 
 * @param addr: Flash address (must be word-aligned).
 * @param data: 32-bit data to write.
 */
void InternalFlash_WriteWord(uint32_t addr, uint32_t data)
{
    FLASH_Unlock();
    FLASH_ProgramWord(addr, data);
    FLASH_Lock();
}

/**
 * @brief Write a 16-bit half-word to Flash memory.
 * 
 * @param addr: Flash address (must be half-word-aligned).
 * @param data: 16-bit data to write.
 */
void InternalFlash_WriteHalfWord(uint32_t addr, uint16_t data)
{
    FLASH_Unlock();
    FLASH_ProgramHalfWord(addr, data);
    FLASH_Lock();
}
