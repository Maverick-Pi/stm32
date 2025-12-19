/****************************************************************************/ /**
 * @file   main.c
 * @brief  0.96inch OLED 4Pins I2C Example
 * 
 * @author Maverick Pi
 * @date   2025-12-10 16:06:08
 ********************************************************************************/

#include "stm32f10x.h"
#include "Serial.h"
#include "I2C_Hardware.h"
#include "OLED.h"

int main(void)
{
    OLED_Init();

    OLED_Clear();

    OLED_ShowMixedString(0, 0, "黎子瑶，瑶瑶，I love you. 中英文混杂显示。");

    while (1) {}
}
