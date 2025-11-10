/****************************************************************************/ /**
 * @file   main.c
 * @brief  Serial Port Transmission and Reception of HEX Data Packets
 * 
 * @author Maverick Pi
 * @date   2025-11-09 20:03:39
 ********************************************************************************/

#include "stm32f10x.h"
#include "OLED.h"
#include "Serial.h"
#include "Key.h"

int main(void)
{
    OLED_Init();
    Serial_Init();
    Key_Init();

    OLED_ShowString(1, 1, "Tx Packet:");
    OLED_ShowString(3, 1, "Rx Packet:");

    uint8_t txPacket[4] = { 0x00, 0x01, 0x02, 0x03 };

    while (1) {
        if (Key_GetNum() == 1) {
            OLED_ShowString(2, 1, "                ");
            for (int i = 0; i < 4; ++i) {
                OLED_ShowHexNum(2, i * 3 + 1, ++txPacket[i], 2);
            }
            Serial_SendPacket(txPacket, 4);
        }
        if (Serial_GetRxFlag() == 1) {
            OLED_ShowString(4, 1, "                ");
            for (int i = 0; i < Serial_RxLen; ++i) {
                OLED_ShowHexNum(i / 5 + 4, i % 5 * 3 + 1, Serial_RxPacket[i], 2);
            }
        }
    }
}
