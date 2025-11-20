/****************************************************************************/ /**
 * @file   W25Q64.c
 * @brief  W25Q64 Flash Memory driver implementation
 * 
 * @author Maverick Pi
 * @date   2025-11-20 21:03:15
 ********************************************************************************/

#include "W25Q64.h"

/**
 * @brief Initialize W25Q64 Flash Memory interface
 * 
 * Initializes the software SPI interface used to communicate with the W25Q64.
 */
void W25Q64_Init(void)
{
    Soft_SPI_Init();
}

/**
 * @brief Read Manufacturer ID and Device ID from W25Q64
 * 
 * @param MID Pointer to store Manufacturer ID (1 byte)
 * @param DID Pointer to store Device ID (2 bytes)
 */
void W25Q64_ReadID(uint8_t* MID, uint16_t* DID)
{
    Soft_SPI_Start();
    Soft_SPI_TransferByte(W25Q64_JEDEC_ID);     // Send JEDEC ID command
    *MID = Soft_SPI_TransferByte(W25Q64_DUMMY_BYTE);    // Read Manufacturer ID
    *DID = Soft_SPI_TransferByte(W25Q64_DUMMY_BYTE);    // Read Device ID MSB
    *DID <<= 8;
    *DID |= Soft_SPI_TransferByte(W25Q64_DUMMY_BYTE);   // Read Device ID LSB
    Soft_SPI_Stop();
}

/**
 * @brief Send Write Enable command to W25Q64
 * 
 * Must be called before any write, program, or erase operation.
 */
void W25Q64_WriteEnable(void)
{
    Soft_SPI_Start();
    Soft_SPI_TransferByte(W25Q64_WRITE_ENABLE);
    Soft_SPI_Stop();
}

/**
 * @brief Wait until W25Q64 is no longer busy
 * 
 * Polls the status register until the BUSY bit is cleared.
 */
void W25Q64_WaitBusy(void)
{
    Soft_SPI_Start();
    Soft_SPI_TransferByte(W25Q64_READ_STATUS_REG1);
    // Wait while BUSY bit (bit 0) is set
    while (Soft_SPI_TransferByte(W25Q64_DUMMY_BYTE) & 0x01);
    Soft_SPI_Stop();
}

/**
 * @brief Program a page (up to 256 bytes) in W25Q64
 * 
 * @param addr Starting address for page program (24-bit)
 * @param dataArr Pointer to data array to program
 * @param len Number of bytes to program (1-256)
 */
void W25Q64_PageProgram(uint32_t addr, uint8_t *dataArr, uint16_t len)
{
    W25Q64_WriteEnable();   // Enable write operations

    Soft_SPI_Start();
    Soft_SPI_TransferByte(W25Q64_PAGE_PROGRAM);     // Send page program command
    Soft_SPI_TransferByte(addr >> 16);      // Send address byte 2
    Soft_SPI_TransferByte(addr >> 8);       // Send address byte 1
    Soft_SPI_TransferByte(addr);            // Send address byte 0

    // Send data bytes
    for (uint16_t i = 0; i < len; i++) {
        Soft_SPI_TransferByte(dataArr[i]);
    }

    Soft_SPI_Stop();
    W25Q64_WaitBusy();  // Wait for programming to complete
}

/**
 * @brief Erase a 4KB sector in W25Q64
 * 
 * @param addr Any address within the 4KB sector to erase
 */
void W25Q64_EraseSector(uint32_t addr)
{
    W25Q64_WriteEnable();   // Enable write operations

    Soft_SPI_Start();
    Soft_SPI_TransferByte(W25Q64_SECTOR_ERASE);     // Send sector erase command
    Soft_SPI_TransferByte(addr >> 16);      // Send address byte 2
    Soft_SPI_TransferByte(addr >> 8);       // Send address byte 1
    Soft_SPI_TransferByte(addr);            // Send address byte 0
    Soft_SPI_Stop();

    W25Q64_WaitBusy();  // Wait for erase to complete
}

/**
 * @brief Erase entire W25Q64 chip
 * 
 * This operation may take several seconds to complete.
 */
void W25Q64_EraseChip(void)
{
    W25Q64_WriteEnable();   // Enable write operations

    Soft_SPI_Start();
    Soft_SPI_TransferByte(W25Q64_CHIP_ERASE);   // Send chip erase command
    Soft_SPI_Stop();

    W25Q64_WaitBusy();  // Wait for chip erase to complete
}

/**
 * @brief Erase a 64KB block in W25Q64
 * 
 * @param addr Any address within the 64KB block to erase
 */
void W25Q64_EraseBlock64K(uint32_t addr)
{
    W25Q64_WriteEnable();   // Enable write operations

    Soft_SPI_Start();
    Soft_SPI_TransferByte(W25Q64_BLOCK_ERASE_64K);  // Send 64KB block erase command
    Soft_SPI_TransferByte(addr >> 16);      // Send address byte 2
    Soft_SPI_TransferByte(addr >> 8);       // Send address byte 1
    Soft_SPI_TransferByte(addr);            // Send address byte 0
    Soft_SPI_Stop();

    W25Q64_WaitBusy();  // Wait for erase to complete
}

/**
 * @brief Erase a 32KB block in W25Q64
 * 
 * @param addr Any address within the 32KB block to erase
 */
void W25Q64_EraseBlock32K(uint32_t addr)
{
    W25Q64_WriteEnable();   // Enable write operations

    Soft_SPI_Start();
    Soft_SPI_TransferByte(W25Q64_BLOCK_ERASE_32K);  // Send 32KB block erase command
    Soft_SPI_TransferByte(addr >> 16);      // Send address byte 2
    Soft_SPI_TransferByte(addr >> 8);       // Send address byte 1
    Soft_SPI_TransferByte(addr);            // Send address byte 0
    Soft_SPI_Stop();

    W25Q64_WaitBusy();  // Wait for erase to complete
}

/**
 * @brief Read data from W25Q64 flash memory
 * 
 * @param addr Starting address to read from (24-bit)
 * @param dataArr Pointer to buffer for storing read data
 * @param len Number of bytes to read
 */
void W25Q64_ReadData(uint32_t addr, uint8_t* dataArr, uint32_t len)
{
    Soft_SPI_Start();
    Soft_SPI_TransferByte(W25Q64_READ_DATA);    // Send read data command
    Soft_SPI_TransferByte(addr >> 16);      // Send address byte 2
    Soft_SPI_TransferByte(addr >> 8);       // Send address byte 1
    Soft_SPI_TransferByte(addr);            // Send address byte 0

    // Read data bytes
    for (uint32_t i = 0; i < len; i++) {
        dataArr[i] = Soft_SPI_TransferByte(W25Q64_DUMMY_BYTE);
    }

    Soft_SPI_Stop();
}
