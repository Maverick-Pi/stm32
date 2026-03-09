/**
 * @file LED.c
 * @author Maverick Pi (pimingjie@gmail.com)
 * @brief LED 控制模块实现
 * LED_Init() 初始化 LED GPIO，LED_SetMode() 设置 LED 模式，LED_Tick() 根据模式更新 LED 状态。
 * 
 * @version 0.1
 * @date 2026-03-08
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "LED.h"

/* LED 列表 */
LED LED_List[2] = {
    { OFF, LED_PIN_1 }, // LED1 初始化
    { OFF, LED_PIN_2 }  // LED2 初始化
};

/**
 * @brief 初始化 LED GPIO
 * 
 * 使能 GPIOA 时钟，配置 PA1 和 PA2 为推挽输出，并将其初始状态设置为 OFF。
 * LED1 连接在 PA1，LED2 连接在 PA2。
 */
void LED_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2);   // Turn off LEDs
}

/**
 * @brief 控制 LED 亮灭
 * 
 * @param ON_OFF 需要设置的状态（LED_ON 或 LED_OFF）
 * @param LED_x 目标 LED（LED_1 或 LED_2）
 */
static void LED_Control(uint8_t ON_OFF, uint16_t LED_x)
{
    if (ON_OFF == LED_ON) GPIO_ResetBits(GPIOA, LED_x);  // 点亮 LED_x 
    else GPIO_SetBits(GPIOA, LED_x);     // 熄灭 LED_x
}

/**
 * @brief 设置 LED 模式
 * 
 * @param led 目标 LED 对象
 * @param mode 要设置的模式
 */
void LED_SetMode(LED* led, LED_STATE mode)
{
    led->mode = mode;
    led->counter = 0;
}

/**
 * @brief LED 模式更新函数
 * 
 * 根据每个 LED 的模式和计数器值，更新其状态。OFF 模式保持熄灭，SOLID 模式保持点亮，SLOW_BLINK 和 FAST_BLINK 模式
 * 根据计数器值切换状态，PULSE 模式短暂点亮后熄灭。
 * @param ledList LED 对象数组
 * @param size LED 数组大小
 */
void LED_Tick(LED* ledList, uint8_t size)
{
    for (uint8_t i = 0; i < size; ++i) {
        LED* led = &ledList[i];
        switch (led->mode)
        {
            case OFF:
                LED_Control(LED_OFF, led->LED_Pin);
                break;
            case SOLID:
                LED_Control(LED_ON, led->LED_Pin);
                break;
            case SLOW_BLINK:
                led->counter++;
                if (led->counter >= 1000) led->counter = 0; // 每1000ms重置计数器
                if (led->counter < 500) {
                    LED_Control(LED_ON, led->LED_Pin);
                } else {
                    LED_Control(LED_OFF, led->LED_Pin);
                }
                break;
            case FAST_BLINK:
                led->counter++;
                if (led->counter >= 100) led->counter = 0; // 每1000ms重置计数器
                if (led->counter < 50) {
                    LED_Control(LED_ON, led->LED_Pin);
                } else {
                    LED_Control(LED_OFF, led->LED_Pin);
                }
                break;
            case PULSE:
                led->counter++;
                if (led->counter >= 1000) led->counter = 0; // 每1000ms重置计数器
                if (led->counter < 100) {
                    LED_Control(LED_ON, led->LED_Pin);
                } else {
                    LED_Control(LED_OFF, led->LED_Pin);
                }
                break;
        }
    }
}
