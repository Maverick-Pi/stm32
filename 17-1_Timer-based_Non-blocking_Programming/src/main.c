/**
 * @file main.c
 * @author Maverick Pi (pimingjie@gmail.com)
 * @brief 定时器实现非阻塞式编程
 * 
 * @version 0.1
 * @date 2026-03-09
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "stm32f10x.h"
#include "System.h"
#include "Timer.h"
#include "OLED.h"
#include "Key.h"
#include "LED.h"

/* 全局变量，用于计数 */
uint32_t i;

/* 组合按键与 LED 控制函数 */
void Combine_Key_LED(LED* ledList);

int main(void)
{
    System_Init();
    Timer_Init();
    OLED_Init();
    OLED_Clear();
    OLED_Update();

    Key_Init();
    LED_Init();

    OLED_ShowString(32, 0, FONT_SIZE_8, "LED MODE");
    OLED_ShowString(0, 16, FONT_SIZE_8, "LED1:");
    OLED_ShowString(0, 32, FONT_SIZE_8, "LED2:");
    OLED_ShowString(0, 48, FONT_SIZE_8, "i:");

    while (1) {
        Combine_Key_LED(LED_List);
        OLED_ShowNum(24, 48, i, FONT_SIZE_8);
        OLED_Update();
    }
}

/**
 * @brief 组合按键与 LED 控制函数
 * 
 * @param ledList LED 对象数组
 */
void Combine_Key_LED(LED* ledList)
{
    uint8_t keyNum = Key_GetNum();

    if (keyNum == 1) {
        LED_STATE currentMode = ledList[0].mode;          // 获取当前模式
        currentMode = (LED_STATE)((currentMode + 1) % 5); // 切换到下一个模式
        LED_SetMode(&ledList[0], currentMode);

        // 更新 OLED 显示
        OLED_ClearArea(80, 16, 48, 16);
        switch (currentMode) {
            case OFF:       OLED_ShowString(80, 16, FONT_SIZE_8, "OFF  "); break;
            case SOLID:     OLED_ShowString(80, 16, FONT_SIZE_8, "SOLID"); break;
            case SLOW_BLINK:OLED_ShowString(80, 16, FONT_SIZE_8, "SLOW "); break;
            case FAST_BLINK:OLED_ShowString(80, 16, FONT_SIZE_8, "FAST "); break;
            case PULSE:     OLED_ShowString(80, 16, FONT_SIZE_8, "PULSE"); break;
        }
        OLED_Update();
    }
    else if (keyNum == 2) {
        LED_STATE currentMode = ledList[1].mode;
        currentMode = (LED_STATE)((currentMode + 1) % 5);
        LED_SetMode(&ledList[1], currentMode);

        OLED_ClearArea(80, 32, 48, 16);
        switch (currentMode) {
            case OFF:       OLED_ShowString(80, 32, FONT_SIZE_8, "OFF  "); break;
            case SOLID:     OLED_ShowString(80, 32, FONT_SIZE_8, "SOLID"); break;
            case SLOW_BLINK:OLED_ShowString(80, 32, FONT_SIZE_8, "SLOW "); break;
            case FAST_BLINK:OLED_ShowString(80, 32, FONT_SIZE_8, "FAST "); break;
            case PULSE:     OLED_ShowString(80, 32, FONT_SIZE_8, "PULSE"); break;
        }
        OLED_Update();
    }
}

/* 定时器中断服务程序 */
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        Key_Tick();
        LED_Tick(LED_List, 2);
        i++;
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 清除中断标志位
    }
}
