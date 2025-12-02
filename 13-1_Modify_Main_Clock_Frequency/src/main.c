/****************************************************************************/ /**
 * @file   main.c
 * @brief  Modify the main clock frequency and display it on OLED.
 * 
 * @author Maverick Pi
 * @date   2025-12-02 15:56:11
 ********************************************************************************/

#include "stm32f10x.h"
#include "OLED.h"
#include "Delay.h"

int main(void)
{
    OLED_Init();

    OLED_ShowString(1, 1, "SYSCLK:");
    OLED_ShowNum(1, 9, SystemCoreClock, 8);

    while (1) {
        OLED_ShowString(2, 1, "Running...");
        Delay_ms(500);
        OLED_ShowString(2, 1, "          ");
        Delay_ms(500);
    }
}
