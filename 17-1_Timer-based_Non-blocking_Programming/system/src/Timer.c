/****************************************************************************/ /**
 * @file   Timer.c
 * @brief  通用定时器 TIM2 初始化及其中断配置
 * 
 * @author Maverick Pi
 * @date   2026-03-04 15:57:34
 ********************************************************************************/

#include "Timer.h"

void Timer_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // 使能 TIM2 时钟

    TIM_InternalClockConfig(TIM2); // 使用内部时钟源
    TIM_TimeBaseInit(TIM2, &(TIM_TimeBaseInitTypeDef) {
        .TIM_ClockDivision = TIM_CKD_DIV1, // 滤波器不分频
        .TIM_CounterMode = TIM_CounterMode_Up, // 向上计数模式
        .TIM_Period = 72 - 1,
        .TIM_Prescaler = 1000 - 1,     // 计时：72 * 1000 / 72MHz = 1ms
        .TIM_RepetitionCounter = 0  // 重复计数器，仅 TIM1 和 TIM8 有效
    });
    TIM_ClearFlag(TIM2, TIM_FLAG_Update); // 清除更新标志位
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // 使能更新中断

    NVIC_Init(&(NVIC_InitTypeDef) {
        .NVIC_IRQChannel = TIM2_IRQn, // TIM2 中断
        .NVIC_IRQChannelPreemptionPriority = 0, // 抢占优先级
        .NVIC_IRQChannelSubPriority = 0, // 子优先级
        .NVIC_IRQChannelCmd = ENABLE // 使能中断
    });

    TIM_Cmd(TIM2, ENABLE); // 使能 TIM2
}

// void TIM2_IRQHandler(void)
// {
//     if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
//         TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 清除中断标志位
//     }
// }
