/****************************************************************************/ /**
 * @file   main.c
 * @brief  Window Watchdog
 * 
 * @author Maverick Pi
 * @date   2025-12-03 22:40:34
 ********************************************************************************/

#include "stm32f10x.h"
#include "OLED.h"
#include "Delay.h"
#include "Key.h"

int main(void)
{
    OLED_Init();
    Key_Init();

    OLED_ShowString(1, 1, "WWDG TEST");

    if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST)) {
        OLED_ShowString(2, 1, "WWDG RESET");
        Delay_ms(500);
        OLED_ShowString(2, 1, "            ");
        RCC_ClearFlag();
    } else {
        OLED_ShowString(3, 1, "RESET");
        Delay_ms(500);
        OLED_ShowString(3, 1, "     ");
    }

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
    WWDG_SetPrescaler(WWDG_Prescaler_8);
    WWDG_SetWindowValue(0x40 | 21);
    WWDG_Enable(0x40 | 54);


    while (1) {
        Key_GetNum();

        OLED_ShowString(4, 1, "FEED DOG");
        Delay_ms(20);
        OLED_ShowString(4, 1, "         ");
        Delay_ms(10);

        WWDG_SetCounter(0x40 | 54);
    }
}
