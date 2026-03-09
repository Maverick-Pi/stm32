/**
 * @file LED.h
 * @author Maverick Pi (pimingjie@gmail.com)
 * @brief LED 控制模块头文件
 * 使用结构体数组提供一组 LED 对象，每个对象包含模式、引脚和计数器。提供初始化函数、设置模式函数和定时器滴答函数。
 * 
 * @version 0.1
 * @date 2026-03-09
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "stm32f10x.h"
#include "OLED.h"

#ifndef __LED_H__
#define __LED_H__

#define LED_PIN_1       GPIO_Pin_1   ///< LED1 连接在 PA1
#define LED_PIN_2       GPIO_Pin_2   ///< LED2 连接在 PA2

#define LED_ON      0           ///< LED 点亮状态
#define LED_OFF     1           ///< LED 熄灭状态

typedef enum {
    OFF, 
    SOLID,
    SLOW_BLINK,
    FAST_BLINK,
    PULSE
} LED_STATE;

typedef struct {
    LED_STATE mode;     // LED 模式
    uint16_t LED_Pin;   // LED 引脚
    uint16_t counter;   // 模式计数器，用于闪烁模式
} LED;

extern LED LED_List[2];

void LED_Init(void);
void LED_SetMode(LED* led, LED_STATE mode);
void LED_Tick(LED* ledList, uint8_t size);

#endif // !__LED_H__
