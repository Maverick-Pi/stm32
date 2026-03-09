/**
 * @file Key.c
 * @author Maverick Pi (pimingjie@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2026-03-06
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "Key.h"

uint8_t Key_Num;

/**
 * @brief 初始化按键 GPIOB Pin1 和 Pin11
 * 
 * 配置为上拉输入模式，启用时钟，设置引脚模式和速度
 */
void Key_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

uint8_t Key_GetNum(void)
{
    uint8_t tempNum = Key_Num;
    Key_Num = 0; // 读取后清除事件标志
    return tempNum;
}

uint8_t Key_GetState(void)
{
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == Bit_RESET) {
        return 1;
    }
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == Bit_RESET) {
        return 2;
    }
    return 0;
}

void Key_Tick(void)
{
    static uint8_t counter = 0;
    static uint8_t lastState = 0, currentState = 0;
    if (++counter >= 20) { // 每20ms检测一次按键状态
        counter = 0;
        lastState = currentState;
        currentState = Key_GetState();

        if (currentState == 0 && lastState != 0) {
            Key_Num = lastState; // 记录按键事件
        }
    }
}
