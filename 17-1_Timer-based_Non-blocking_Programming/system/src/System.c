/****************************************************************************/ /**
 * @file   System.c
 * @brief  系统初始化
 * 
 * @author Maverick Pi
 * @date   2026-03-04 23:10:45
 ********************************************************************************/

#include "System.h"

/**
 * @brief 系统初始化
 * 包括 NVIC 分组设置等系统级配置
 * 
 */
void System_Init(void)
{
    /* NVIC 分组 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}
