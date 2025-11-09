/****************************************************************************/ /**
 * @file   main.c
 * @brief  Main application file for USART communication with OLED display
 * 
 * @author Maverick Pi
 * @date   2025-11-07 17:35:02
 ********************************************************************************/

#include "stm32f10x.h"
#include "OLED.h"
#include "Serial.h"

/**
 * @brief Main application entry point
 * 
 * Initializes OLED display and USART communication, then enters main loop
 * to receive data via USART and display it on OLED
 * 
 * @return int Program status (not used in embedded context)
 */
int main(void)
{
    uint8_t rx_data;    // Buffer for received data

    OLED_Init();
    Serial_Init();

    OLED_ShowString(1, 1, "RxData:");

    while (1) {
        // Check if new data has been received
        if (Serial_GetRxFlag()) {
            rx_data = Serial_GetRxData();
            Serial_SendByte(rx_data);
            OLED_ShowHexNum(1, 8, rx_data, 2);
        }
    }
}
