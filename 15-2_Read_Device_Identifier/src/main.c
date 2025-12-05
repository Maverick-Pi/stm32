/****************************************************************************/ /**
 * @file   main.c
 * @brief  Read Device Identifier
 * 
 * @author Maverick Pi
 * @date   2025-12-05 23:24:58
 ********************************************************************************/

#include "stm32f10x.h"
#include "OLED.h"

int main(void)
{
    OLED_Init();

    OLED_ShowString(1, 1, "F_SIZE:");
    OLED_ShowHexNum(1, 8, *((__IO uint16_t *)0x1FFFF7E0), 4);

    OLED_ShowString(2, 1, "UID_L:");
    OLED_ShowHexNum(2, 8, *((__IO uint32_t *)0x1FFFF7E8), 8);
    OLED_ShowString(3, 1, "UID_M:");
    OLED_ShowHexNum(3, 8, *((__IO uint32_t *)0x1FFFF7EC), 8);
    OLED_ShowString(4, 1, "UID_H:");
    OLED_ShowHexNum(4, 8, *((__IO uint32_t *)0x1FFFF7F0), 8);

    while (1) {

    }
}
