/****************************************************************************/ /**
 * @file   main.c
 * @brief  
 * 
 * @author Maverick Pi
 * @date   2025-08-03 21:30:06
 ********************************************************************************/

#include "OLED.h"

int main(void)
{
    OLED_Init();
    OLED_ShowChar(1, 1, 'A');
    OLED_ShowString(1, 3, "Hello World!");
    OLED_ShowBinNum(2, 1, 0xAABB, 16);
    OLED_ShowHexNum(3, 1, 0xCD8E, 4);
    OLED_ShowNum(4, 1, 1314520, 7);
    OLED_ShowSignedNum(4, 9, 1314520, 7);

    while (1) {}
}
