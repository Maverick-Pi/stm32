/****************************************************************************/ /**
 * @file   main.c
 * @brief  Main application demonstrating DMA-based memory transfer
 * 
 * Demonstrates DMA transfer between two memory buffers with visual feedback
 * on OLED display. Transfers data from DataA to DataB and increments source
 * data after each transfer.
 * 
 * @author Maverick Pi
 * @date   2025-09-17 19:13:42
 ********************************************************************************/

#include "OLED.h"
#include "Delay.h"
#include "App_DMA.h"

uint8_t DataA[] = { 0x01, 0x02, 0x03, 0x04 };   ///< Source data buffer
uint8_t DataB[4];   ///< Destination data buffer

int main(void)
{
    uint16_t pause = 1000;  // Delay period between operations (ms)

    OLED_Init();    // Initialize OLED display

    // Display static information
    OLED_ShowString(1, 1, "DataA");
    OLED_ShowString(3, 1, "DataB");
    OLED_ShowHexNum(1, 8, (uint32_t)DataA, 8);  // Show source buffer address
    OLED_ShowHexNum(3, 8, (uint32_t)DataB, 8);  // Show destination buffer address

    // Initialize DMA for transfer between DataA and DataB
    App_DMA_Init((uint32_t)DataA, (uint32_t)DataB, 4);

    while (1) {
        // Display current values of source buffer
        OLED_ShowHexNum(2, 1, DataA[0], 2);
        OLED_ShowHexNum(2, 4, DataA[1], 2);
        OLED_ShowHexNum(2, 7, DataA[2], 2);
        OLED_ShowHexNum(2, 10, DataA[3], 2);

        // Display current values of destination buffer
        OLED_ShowHexNum(4, 1, DataB[0], 2);
        OLED_ShowHexNum(4, 4, DataB[1], 2);
        OLED_ShowHexNum(4, 7, DataB[2], 2);
        OLED_ShowHexNum(4, 10, DataB[3], 2);

        Delay_ms(pause);    // Wait for specified period

        App_DMA_Transfer();     // Execute DMA transfer

        // Refresh display with updated values after transfer
        OLED_ShowHexNum(2, 1, DataA[0], 2);
        OLED_ShowHexNum(2, 4, DataA[1], 2);
        OLED_ShowHexNum(2, 7, DataA[2], 2);
        OLED_ShowHexNum(2, 10, DataA[3], 2);

        OLED_ShowHexNum(4, 1, DataB[0], 2);
        OLED_ShowHexNum(4, 4, DataB[1], 2);
        OLED_ShowHexNum(4, 7, DataB[2], 2);
        OLED_ShowHexNum(4, 10, DataB[3], 2);

        // Increment source data for next transfer
        for (int i = 0; i < sizeof(DataA) / sizeof(DataA[0]); ++i) {
            DataA[i]++;
        }

        Delay_ms(pause);    // Wait for specified period
    }
}
