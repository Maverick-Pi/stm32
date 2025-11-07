/****************************************************************************/ /**
 * @file   main.c
 * @brief  Main application entry point with OLED and Serial communication
 * 
 * @author Maverick Pi
 * @date   2025-10-12 22:31:34
 ********************************************************************************/

#include "OLED.h"
#include "Serial.h"

/**
 * @brief Main program entry point
 * 
 * Initializes OLED display and Serial communication (USART1),
 * displays a welcome message on OLED, and sends formatted text via Serial.
 * 
 * @return int Program status (not used in embedded context)
 */
int main(void)
{
    OLED_Init();
    Serial_Init();

    OLED_ShowString(1, 1, "Hello Qt!");

    // Serial_SendByte('A');
    // uint16_t arr[] = { 0x31, 0x32, 0x33, 0x34 };
    // Serial_SendArray(arr, 4);
    // Serial_SendString("Hello USART!\r\n");
    // Serial_SendNumber(1949, 4);
    // printf("Num=%d\r\n", 666);

    // char str[100];
    // sprintf(str, "Num=%d\r\n", 666);
    // Serial_SendString(str);

    // Serial_Printf("Num=%d\n\n\n", 888);
    Serial_Printf("你好，世界！");

    while (1) {

    }
}
