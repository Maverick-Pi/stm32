/****************************************************************************/ /**
 * @file   main.c
 * @brief  Independent Watchdog
 * 
 * @author Maverick Pi
 * @date   2025-12-03 17:15:36
 ********************************************************************************/

#include "stm32f10x.h"
#include "OLED.h"
#include "delay.h"
#include "Key.h"

int main(void)
{
    OLED_Init();
    Key_Init();

    OLED_ShowString(1, 1, "IWDG TEST");

    if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST)) {
        OLED_ShowString(2, 1, "IWDG RESET");
        Delay_ms(1000);
        OLED_ShowString(2, 1, "            ");
        RCC_ClearFlag();
    } else {
        OLED_ShowString(3, 1, "RESET");
        Delay_ms(1000);
        OLED_ShowString(3, 1, "            ");
    }

    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(IWDG_Prescaler_16);
    IWDG_SetReload(2499); // 1s
    IWDG_ReloadCounter();
    IWDG_Enable();

    while (1) {
        Key_GetNum();

        IWDG_ReloadCounter();
        OLED_ShowString(4, 1, "FEED DOG");
        Delay_ms(500);
        OLED_ShowString(4, 1, "         ");
        Delay_ms(300);
    }
}
