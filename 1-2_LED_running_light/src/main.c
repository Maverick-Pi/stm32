/****************************************************************************/ /**
 * @file   main.c
 * @brief  LED running light
 * 
 * @author Maverick Pi
 * @date   2025-07-31 22:31:24
 ********************************************************************************/

#include "stm32f10x.h"
#include "Delay.h"

/**
 * @brief 
 * 
 * @return int 
 */
int main(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOA, &GPIO_InitStruct);

    while (1) {
        for (int i = 0; i < 8; ++i) {
            GPIO_Write(GPIOA, ~(0x0001 << i));
            Delay_ms(100);
        }
    }
}
