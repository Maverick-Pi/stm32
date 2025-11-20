/****************************************************************************/ /**
 * @file   main.c
 * @brief  Main application file for W25Q64 Flash Memory test with OLED display
 * 
 * @author Maverick Pi
 * @date   2025-11-19 21:13:01
 ********************************************************************************/

#include "stm32f10x.h"
#include "OLED.h"
#include "W25Q64.h"

/**
 * @brief Main application entry point
 * 
 * This function initializes the OLED display and W25Q64 flash memory,
 * reads the device ID, performs erase and read operations, and displays
 * the results on the OLED screen.
 * 
 * @return int Program status (not used in embedded context)
 */
int main(void)
{
    OLED_Init();
    W25Q64_Init();

    OLED_ShowString(1, 1, "MID:    DID:");
    OLED_ShowString(2, 1, "W:");
    OLED_ShowString(3, 1, "R:");

    uint8_t MID;    // Manufacturer ID
    uint16_t DID;   // Device ID

    W25Q64_ReadID(&MID, &DID);
    OLED_ShowHexNum(1, 5, MID, 2);
    OLED_ShowHexNum(1, 13, DID, 4);

    // Test data arrays for write and read operations
    uint8_t wArr[] = { 0xCA, 0xFE, 0xBA, 0xBE };
    uint8_t rArr[4] = {0};

    // Erase sector before programming
    W25Q64_EraseSector(0x000000);
    W25Q64_PageProgram(0x000000, wArr, 4);

    for (int i = 0; i < 4; i++) {
        OLED_ShowHexNum(2, 4 + i * 3, wArr[i], 2);
    }

    W25Q64_ReadData(0x000000, rArr, 4);

    for (int i = 0; i < 4; i++) {
        OLED_ShowHexNum(3, 4 + i * 3, rArr[i], 2);
    }

    while (1) {

    }
}
