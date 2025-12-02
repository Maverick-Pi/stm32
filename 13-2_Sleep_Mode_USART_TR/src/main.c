/****************************************************************************/ /**
 * @file   main.c
 * @brief  USART transmission and reception with sleep mode
 * 
 * @author Maverick Pi
 * @date   2025-12-02 16:11:02
 ********************************************************************************/

#include "stm32f10x.h"
#include "OLED.h"
#include "Serial.h"
#include "Delay.h"

int main(void)
{
    OLED_Init();
    Serial_Init();

    OLED_ShowString(1, 1, "Rx:");

    uint8_t rxData;

    while (1) {
        if (Serial_GetRxFlag()) {
            rxData = Serial_GetRxData();
            Serial_SendByte(rxData);  // Echo back received data
            OLED_ShowHexNum(1, 5, rxData, 2);  // Display received data in hex
        }

        OLED_ShowString(2, 1, "Running...");
        Delay_ms(100);
        OLED_ShowString(2, 1, "            ");
        Delay_ms(100);

        __WFI();
    }
}
