/****************************************************************************/ /**
 * @file   main.c
 * @brief  0.96inch OLED 4Pins I2C Example
 * 
 * @author Maverick Pi
 * @date   2025-12-10 16:06:08
 ********************************************************************************/

#include "stm32f10x.h"
#include "OLED.h"

int main(void)
{
    OLED_Init();

    OLED_Clear();

    OLED_ShowMixedString(0, 0, "牛逼");
    OLED_ShowChineseChar(17, 3, "牛");

    while (1) {}
}
