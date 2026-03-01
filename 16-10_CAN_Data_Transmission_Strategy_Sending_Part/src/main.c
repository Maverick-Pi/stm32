/****************************************************************************/ /**
 * @file   main.c
 * @brief  CAN 数据传输策略——发送部分
 * 
 * @author Maverick Pi
 * @date   2026-02-26 20:25:06
 ********************************************************************************/

#include "stm32f10x.h"
#include "OLED.h"
#include "Key.h"
#include "CAN_Hardware.h"
#include "Timer.h"

uint8_t KeyNum;

uint8_t TimingFlag;
uint8_t TriggerFlag;
uint8_t RequestFlag;

CanTxMsg TxMsg_Timing = {
    .StdId = 0x100,
    .IDE = CAN_Id_Standard,
    .RTR = CAN_RTR_Data,
    .DLC = 4,
    .Data = { 0x11, 0x22, 0x33, 0x44 }
};

CanTxMsg TxMsg_Trigger = {
    .StdId = 0x200,
    .IDE = CAN_Id_Standard,
    .RTR = CAN_RTR_Data,
    .DLC = 4,
    .Data = { 0x11, 0x22, 0x33, 0x44 }
};

CanTxMsg TxMsg_Request = {
    .StdId = 0x300,
    .IDE = CAN_Id_Standard,
    .RTR = CAN_RTR_Data,
    .DLC = 4,
    .Data = { 0x11, 0x22, 0x33, 0x44 }
};

CanRxMsg RxMsg;

int main(void)
{
    /* 外设初始化 */
    OLED_Init();    // OLED 显示屏初始化
    OLED_Clear();   // 清屏
    OLED_Update();  // 更新显示
    Key_Init();     // 按键初始化
    CAN_Hardware_Init();    // CAN 硬件初始化
    Timer_Init();  // 定时器初始化

    /* 显示固定界面元素 */
    OLED_ShowString(0, 0, FONT_SIZE_8, "Tx");    // 发送标签
    OLED_ShowString(0, 16, FONT_SIZE_8, "Tim:");   // 定时数据传输标签
    OLED_ShowString(0, 32, FONT_SIZE_8, "Tri:");   // 触发数据传输标签
    OLED_ShowString(0, 48, FONT_SIZE_8, "Req:");   // 请求传输数据标签
    OLED_Update();  // 更新显示

    while (1) {
        /* 定时发送 */
        if (TimingFlag) {
            TimingFlag = 0;
            for (int i = 0; i < 4; i++) {
                TxMsg_Timing.Data[i]++;
            }
            CAN_Hardware_Transmit(&TxMsg_Timing);
            OLED_ShowHexNum(40, 16, TxMsg_Timing.Data[0], FONT_SIZE_6);
            OLED_ShowHexNum(70, 16, TxMsg_Timing.Data[1], FONT_SIZE_6);
            OLED_ShowHexNum(40, 24, TxMsg_Timing.Data[2], FONT_SIZE_6);
            OLED_ShowHexNum(70, 24, TxMsg_Timing.Data[3], FONT_SIZE_6);
            OLED_Update();
        }

        /* 触发发送 */
        KeyNum = Key_GetNum();
        if (KeyNum == 1) {
            TriggerFlag = 1;
        }

        if (TriggerFlag) {
            TriggerFlag = 0;
            for (int i = 0; i < 4; i++) {
                TxMsg_Trigger.Data[i]++;
            }
            CAN_Hardware_Transmit(&TxMsg_Trigger);
            OLED_ShowHexNum(40, 32, TxMsg_Trigger.Data[0], FONT_SIZE_6);
            OLED_ShowHexNum(70, 32, TxMsg_Trigger.Data[1], FONT_SIZE_6);
            OLED_ShowHexNum(40, 40, TxMsg_Trigger.Data[2], FONT_SIZE_6);
            OLED_ShowHexNum(70, 40, TxMsg_Trigger.Data[3], FONT_SIZE_6);
            OLED_Update();
        }

        /* 请求发送 */
        if (CAN_Hardware_ReceiveFlag()) {
            CAN_Hardware_Receive(&RxMsg);

            if (RxMsg.IDE == CAN_Id_Standard && RxMsg.RTR == CAN_RTR_Remote && RxMsg.StdId == 0x300) {
                RequestFlag = 1;
            }

            if (RxMsg.IDE == CAN_Id_Standard && RxMsg.RTR == CAN_RTR_Data && RxMsg.StdId == 0x3FF) {
                RequestFlag = 1;
            }
        }

        if (RequestFlag) {
            RequestFlag = 0;
            for (int i = 0; i < 4; i++) {
                TxMsg_Request.Data[i]++;
            }
            CAN_Hardware_Transmit(&TxMsg_Request);
            OLED_ShowHexNum(40, 48, TxMsg_Request.Data[0], FONT_SIZE_6);
            OLED_ShowHexNum(70, 48, TxMsg_Request.Data[1], FONT_SIZE_6);
            OLED_ShowHexNum(40, 56, TxMsg_Request.Data[2], FONT_SIZE_6);
            OLED_ShowHexNum(70, 56, TxMsg_Request.Data[3], FONT_SIZE_6);
            OLED_Update();
        }
    }
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {
        TimingFlag = 1;
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
