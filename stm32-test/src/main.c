/****************************************************************************/ /**
 * @file   main.c
 * @brief  自制 STM32F103C8T6 开发板测试程序
 * 
 * @author Maverick Pi
 * @date   2026-01-18 01:31:05
 ********************************************************************************/

#include "stm32f10x.h"
#include "OLED.h"

int main(void)
{
    // RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    // GPIO_Init(GPIOC, &(GPIO_InitTypeDef) {
    //     .GPIO_Pin = GPIO_Pin_13,
    //     .GPIO_Speed = GPIO_Speed_50MHz,
    //     .GPIO_Mode = GPIO_Mode_Out_PP
    // });

    // GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);

    OLED_Init();
    OLED_Clear();

    OLED_ShowString(0, 0, FONT_SIZE_8, "中文显示测试");

    OLED_Update();

    while (1) { }
}
