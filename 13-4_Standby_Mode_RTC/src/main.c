/****************************************************************************/ /**
 * @file   main.c
 * @brief  RTC with standby mode
 * 
 * @author Maverick Pi
 * @date   2025-12-02 23:31:24
 ********************************************************************************/

#include "stm32f10x.h"
#include "OLED.h"
#include "MyRTC.h"
#include "Delay.h"

int main(void)
{
    OLED_Init();
    MyRTC_Init();

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    OLED_ShowString(1, 1, "CNT :");
    OLED_ShowString(2, 1, "ALR :");
    OLED_ShowString(3, 1, "ALRF:");

    PWR_WakeUpPinCmd(ENABLE);

    uint32_t alarm = RTC_GetCounter() + 10;
    RTC_SetAlarm(alarm);
    OLED_ShowNum(2, 6, alarm, 10);

    while (1) {
        OLED_ShowNum(1, 6, RTC_GetCounter(), 10);
        OLED_ShowNum(3, 6, RTC_GetFlagStatus(RTC_FLAG_ALR), 1);

        OLED_ShowString(4, 1, "Running");
        Delay_ms(100);
        OLED_ShowString(4, 1, "        ");
        Delay_ms(100);

        OLED_ShowString(4, 9, "Standby");
        Delay_ms(100);
        OLED_ShowString(4, 9, "       ");
        Delay_ms(100);

        OLED_Clear();

        PWR_ClearFlag(PWR_FLAG_WU);
        PWR_EnterSTANDBYMode();
    }
}
