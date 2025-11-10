/****************************************************************************/ /**
 * @file   main.c
 * @brief  Serial Port Text Data Packet Transmission and Reception
 * 
 * @author Maverick Pi
 * @date   2025-11-10 12:16:28
 ********************************************************************************/

#include "stm32f10x.h"
#include "Key.h"
#include "OLED.h"
#include "Serial.h"
#include "LED.h"
#include <string.h>

int main(void)
{
    Key_Init();
    LED_Init();
    OLED_Init();
    Serial_Init();

    OLED_ShowString(1, 1, "Tx_Packet:");
    OLED_ShowString(3, 1, "Rx_Packet:");

    while (1) {
        if (Serial_GetRxFlag() == 1) {
            OLED_ShowString(4, 1, "                ");
            OLED_ShowString(4, 1, Serial_RxPacket);

            if (strcmp(Serial_RxPacket, "LED_ON") == 0) {
                LED_Control(LED_ON, GPIO_Pin_1);
                Serial_SendString("LED_ON_OK\r\n");
                OLED_ShowString(2, 1, "                ");
                OLED_ShowString(2, 1, "LED_ON_OK");
            } else if (strcmp(Serial_RxPacket, "LED_OFF") == 0) {
                LED_Control(LED_OFF, GPIO_Pin_1);
                Serial_SendString("LED_OFF_OK\r\n");
                OLED_ShowString(2, 1, "                ");
                OLED_ShowString(2, 1, "LED_OFF_OK");
            } else {
                Serial_SendString("ERROR_COMMAND\r\n");
                OLED_ShowString(2, 1, "                ");
                OLED_ShowString(2, 1, "ERROR_COMMAND");
            }
        }
    }
}
