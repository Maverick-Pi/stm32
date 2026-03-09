/**
 * @file Key.h
 * @author Maverick Pi (pimingjie@gmail.com)
 * @brief 表驱动状态机按键检测模块接口
 * 提供按键初始化和事件获取函数，支持单击、双击、长按
 * @version 0.1
 * @date 2026-03-05
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef __KEY_H__
#define __KEY_H__

#include "stm32f10x.h"

void Key_Init(void);
uint8_t Key_GetNum(void);
void Key_Tick(void);

#endif // !__KEY_H__
