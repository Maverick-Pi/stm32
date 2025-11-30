/****************************************************************************/ /**
 * @file   main.c
 * @brief  Main application for STM32F10x backup register read/write demonstration
 * 
 * @author Maverick Pi
 * @date   2025-11-29 12:57:03
 ********************************************************************************/

#include "stm32f10x.h"
#include "OLED.h"
#include "Key.h"

/**
 * @brief Main application entry point
 * 
 * This program demonstrates reading and writing to STM32 backup registers.
 * When button 1 is pressed, two 16-bit values are incremented and written to backup registers.
 * The current values and values read from backup registers are displayed on OLED.
 * 
 * @return int Not used in embedded application (infinite loop)
 */
int main(void)
{
    uint8_t keyNum = 0;
    uint16_t writeArr[] = { 0x1234, 0x5678 };
    uint16_t readArr[2] = { 0 };

    OLED_Init();
    Key_Init();

    OLED_ShowString(1, 1, "W:");
    OLED_ShowString(2, 1, "R:");

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);

    PWR_BackupAccessCmd(ENABLE);

    while (1) {
        keyNum = Key_GetNum();

        if (keyNum == 1) {
            BKP_WriteBackupRegister(BKP_DR1, ++writeArr[0]);
            BKP_WriteBackupRegister(BKP_DR2, ++writeArr[1]);
            OLED_ShowHexNum(1, 4, writeArr[0], 4);
            OLED_ShowHexNum(1, 9, writeArr[1], 4);
        }

        readArr[0] = BKP_ReadBackupRegister(BKP_DR1);
        readArr[1] = BKP_ReadBackupRegister(BKP_DR2);
        OLED_ShowHexNum(2, 4, readArr[0], 4);
        OLED_ShowHexNum(2, 9, readArr[1], 4);
    }
}
