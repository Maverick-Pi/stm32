/****************************************************************************/ /**
 * @file   main.c
 * @brief  Main program entry and control loop for STM32F10x, integrating 
 *         OLED display, key input, and non-volatile storage management.
 *
 * The program initializes peripherals, then enters an infinite loop to:
 *  - Read keypad input
 *  - Store predefined data or clear storage based on keypress
 *  - Display storage content (both hex and ASCII) on OLED
 * 
 * @author Maverick Pi
 * @date   2025-12-05 16:34:35
 ********************************************************************************/

#include "stm32f10x.h"
#include "OLED.h"
#include "Key.h"
#include "Store.h"

int main(void)
{
    OLED_Init();
    Key_Init();
    Store_Init();

    OLED_ShowString(1, 1, "Flag:");
    OLED_ShowString(2, 1, "Data:");

    uint8_t keyNum = 0;

    while (1) {
        keyNum = Key_GetNum();

        /* If KEY1 pressed: write a predefined string to storage and save */
        if (keyNum == 1) {
            Store_Data[1] = 0x2049;
            Store_Data[2] = 0x6F6C;
            Store_Data[3] = 0x6576;
            Store_Data[4] = 0x7920;
            Store_Data[5] = 0x756F;
            Store_Data[6] = 0x002E;
            Store_Save();
        }

        /* If KEY2 pressed: clear all stored data (except flag) */
        if (keyNum == 2) {
            Store_Clear();
        }

        OLED_ShowHexNum(1, 7, Store_Data[0], 4);
        OLED_ShowHexNum(2, 7, Store_Data[1], 4);
        OLED_ShowHexNum(2, 12, Store_Data[2], 4);
        OLED_ShowHexNum(3, 1, Store_Data[3], 4);
        OLED_ShowHexNum(3, 6, Store_Data[4], 4);
        OLED_ShowHexNum(3, 11, Store_Data[5], 4);

        /* Convert stored hex words (index 1-6) to ASCII string for display */
        char str[16] = { 0 };
        uint8_t index = 0;
        for (uint8_t i = 1; i <= 6; i++) {
            str[index++] = (char)(Store_Data[i] & 0xFF);
            str[index++] = (char)((Store_Data[i] >> 8) & 0xFF);
        }
        OLED_ShowString(4, 1, str);
    }
}
