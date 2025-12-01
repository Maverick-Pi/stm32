/****************************************************************************/ /**
 * @file   main.c
 * @brief  Main application demonstrating RTC functionality with OLED display
 * 
 * @author Maverick Pi
 * @date   2025-11-30 21:55:20
 ********************************************************************************/

#include "stm32f10x.h"
#include "OLED.h"
#include "MyRTC.h"
#include <stdio.h>

/**
 * @brief  Display current timezone on OLED screen
 * @param  line OLED display line (row) position
 * @param  column OLED display column position
 */
void OLED_ShowTimeZone(uint8_t line, uint8_t column)
{
    char tzStr[6];
    int8_t tz = MyRTC_GetTimeZone();
    if (tz >= 0) {
        sprintf(tzStr, "UTC+%d", tz);
    } else {
        sprintf(tzStr, "UTC%d", tz);
    }
    OLED_ShowString(line, column, tzStr);
}

int main(void)
{
    MyRTC_Time currentTime;
    MyRTC_Time setTime = { 2025, 12, 1, 21, 13, 0 };

    OLED_Init();
    MyRTC_Init();

    OLED_ShowString(1, 1, "RTC:");
    OLED_ShowString(2, 1, "Date: xxxx-xx-xx");
    OLED_ShowString(3, 1, "Time: xx:xx:xx");
    OLED_ShowString(4, 1, "TZ: ");
    OLED_ShowTimeZone(4, 5);

    MyRTC_SetTime(&setTime);

    while (1) {
        OLED_ShowNum(1, 7, RTC_GetCounter(), 10);
        currentTime = MyRTC_GetTime();
        OLED_ShowNum(2, 7, currentTime.year, 4);
        OLED_ShowNum(2, 12, currentTime.month, 2);
        OLED_ShowNum(2, 15, currentTime.day, 2);
        OLED_ShowNum(3, 7, currentTime.hour, 2);
        OLED_ShowNum(3, 10, currentTime.minute, 2);
        OLED_ShowNum(3, 13, currentTime.second, 2);
        OLED_ShowTimeZone(4, 5);
    }
}
