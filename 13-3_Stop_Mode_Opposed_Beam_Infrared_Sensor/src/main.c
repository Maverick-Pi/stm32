/****************************************************************************/ /**
 * @file   main.c
 * @brief  Opposed beam infrared sensor counter with stop mode
 * 
 * @author Maverick Pi
 * @date   2025-12-02 22:43:17
 ********************************************************************************/

#include "stm32f10x.h"
#include "OLED.h"
#include "CountSensor.h"
#include "Delay.h"

int main(void)
{
    OLED_Init();
    CountSensor_Init();

    OLED_ShowString(1, 1, "Count:");

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); // Enable power interface clock

    while (1) {
        OLED_ShowNum(1, 8, CountSensor_GetCount(), 5);

        OLED_ShowString(2, 1, "Running...");
        Delay_ms(100);
        OLED_ShowString(2, 1, "           ");
        Delay_ms(100);

        PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI); // Enter stop mode
        SystemInit();
    }
}
