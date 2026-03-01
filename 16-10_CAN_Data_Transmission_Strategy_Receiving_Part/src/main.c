/****************************************************************************/ /**
 * @file   main.c
 * @brief  CAN 数据传输策略——接收部分
 * 
 * @author Maverick Pi
 * @date   2026-02-28 23:51:39
 ********************************************************************************/

#include "stm32f10x.h"
#include "OLED.h"
#include "Key.h"
#include "CAN_Hardware.h"

uint8_t KeyNum;

CanTxMsg TxMsg_Req_Remote = {
    .StdId = 0x300,    // 标准帧 ID
    .ExtId = 0x00,     // 扩展帧 ID（不使用）
    .IDE = CAN_Id_Standard,   // 标准帧
    .RTR = CAN_RTR_Remote,    // 远程帧
    .DLC = 0,   // 数据长度为 0 字节
    .Data = {0}   // 数据内容（远程帧不包含数据）
};

CanTxMsg TxMsg_Req_Data = {
    .StdId = 0x3FF,    // 标准帧 ID
    .ExtId = 0x00,     // 扩展帧 ID（不使用）
    .IDE = CAN_Id_Standard,   // 标准帧
    .RTR = CAN_RTR_Data,    // 数据帧
    .DLC = 0,   // 数据长度为 0 字节
    .Data = {0}   // 数据内容（远程帧不包含数据）
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

    /* 显示固定界面元素 */
    OLED_ShowString(0, 0, FONT_SIZE_8, "Rx");    // 接收标签
    OLED_ShowString(0, 16, FONT_SIZE_8, "Tim:");   // 定时数据传输标签
    OLED_ShowString(0, 32, FONT_SIZE_8, "Tri:");   // 触发数据传输标签
    OLED_ShowString(0, 48, FONT_SIZE_8, "Req:");   // 请求传输数据标签
    OLED_Update();  // 更新显示

    while (1) {
        /* 请求部分 */
        KeyNum = Key_GetNum();
        if (KeyNum == 1) {
            CAN_Hardware_Transmit(&TxMsg_Req_Remote);   // 发送远程帧请求数据
        }
        if (KeyNum == 2) {
            CAN_Hardware_Transmit(&TxMsg_Req_Data);     // 发送数据帧请求数据
        }

        /* 接收部分 */
        if (CAN_Hardware_ReceiveFlag()) {
            CAN_Hardware_Receive(&RxMsg);

            if (RxMsg.RTR == CAN_RTR_Data) {
                /* 收到定时数据帧 */
                if (RxMsg.StdId == 0x100 && RxMsg.IDE == CAN_Id_Standard) {
                    OLED_ShowHexNum(40, 16, RxMsg.Data[0], FONT_SIZE_6);
                    OLED_ShowHexNum(70, 16, RxMsg.Data[1], FONT_SIZE_6);
                    OLED_ShowHexNum(40, 24, RxMsg.Data[2], FONT_SIZE_6);
                    OLED_ShowHexNum(70, 24, RxMsg.Data[3], FONT_SIZE_6);
                    OLED_Update();
                }

                /* 收到触发数据帧 */
                if (RxMsg.StdId == 0x200 && RxMsg.IDE == CAN_Id_Standard) {
                    OLED_ShowHexNum(40, 32, RxMsg.Data[0], FONT_SIZE_6);
                    OLED_ShowHexNum(70, 32, RxMsg.Data[1], FONT_SIZE_6);
                    OLED_ShowHexNum(40, 40, RxMsg.Data[2], FONT_SIZE_6);
                    OLED_ShowHexNum(70, 40, RxMsg.Data[3], FONT_SIZE_6);
                    OLED_Update();
                }

                /* 收到请求数据帧 */
                if (RxMsg.StdId == 0x300 && RxMsg.IDE == CAN_Id_Standard) {
                    OLED_ShowHexNum(40, 48, RxMsg.Data[0], FONT_SIZE_6);
                    OLED_ShowHexNum(70, 48, RxMsg.Data[1], FONT_SIZE_6);
                    OLED_ShowHexNum(40, 56, RxMsg.Data[2], FONT_SIZE_6);
                    OLED_ShowHexNum(70, 56, RxMsg.Data[3], FONT_SIZE_6);
                    OLED_Update();
                }
            }
        }
    }
}
