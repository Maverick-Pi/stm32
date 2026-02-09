/****************************************************************************/ /**
 * @file   main.c
 * @brief  CAN 通信主程序
 *        初始化 OLED 显示屏、按键和 CAN 硬件，进入主循环检测按键输入和 CAN 数据接收
 * 
 * @author Maverick Pi
 * @date   2026-02-04 00:34:07
 ********************************************************************************/

#include "stm32f10x.h"
#include "OLED.h"
#include "Key.h"
#include "CAN_Hardware.h"

uint8_t KeyNum = 0;     // 按键键值变量

/* CAN 发送报文数组，包含标准帧和扩展帧的示例数据 */
CanTxMsg TxMsg[] = {
    {
        .StdId = 0x111,
        .IDE   = CAN_Id_Standard,
        .RTR   = CAN_RTR_Data,
        .DLC   = 4,
        .Data  = { 0x11, 0x22, 0x33, 0x44 }
     },
     {
        .ExtId = 0x12345678,
        .IDE = CAN_Id_Extended,
        .RTR = CAN_RTR_Data,
        .DLC = 4,
        .Data = { 0xAA, 0xBB, 0xCC, 0xDD }
     },
     {
        .StdId = 0x222,
        .IDE = CAN_Id_Standard,
        .RTR = CAN_RTR_Remote
     },
     {
        .ExtId = 0x0789ABCD,
        .IDE = CAN_Id_Extended,
        .RTR = CAN_RTR_Remote
     }
};

/* 发送报文索引变量 */
uint8_t TxMsgIndex = 0;

/* 接收报文变量 */
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
    OLED_ShowString(0, 0, FONT_SIZE_8, "Rx:");    // 显示发送 ID 标签
    OLED_ShowString(0, 16, FONT_SIZE_8, "RxID:");   // 显示接收 ID 标签
    OLED_ShowString(0, 32, FONT_SIZE_8, "Leng:");   // 显示数据长度标签
    OLED_ShowString(0, 48, FONT_SIZE_8, "Data:");   // 显示数据标签
    OLED_Update();  // 更新显示

    while (1) {
        /* 检测按键输入 */
        KeyNum = Key_GetNum();
        
        /* 按键 1 按下时发送 CAN 数据 */
        if (KeyNum == 1) {
            /* 发送 CAN 报文 */
            CAN_Hardware_Transmit(&TxMsg[TxMsgIndex++]);
            if (TxMsgIndex >= sizeof(TxMsg) / sizeof(TxMsg[0])) {
                TxMsgIndex = 0;    // 循环发送报文
            }
        }

        /* 检测是否有 CAN 数据接收 */
        if (CAN_Hardware_ReceiveFlag()) {
            /* 读取接收到的 CAN 报文 */
            CAN_Hardware_Receive(&RxMsg);

            if (RxMsg.IDE == CAN_Id_Standard) {
                OLED_ShowString(32, 0, FONT_SIZE_8, "STD");
                OLED_ClearArea(48, 16, 80, 16);    // 清除 ID 显示区域
                OLED_ShowHexNum(48, 16, RxMsg.StdId, FONT_SIZE_8);         // 显示接收标准 ID
            } else if (RxMsg.IDE == CAN_Id_Extended) {
                OLED_ShowString(32, 0, FONT_SIZE_8, "EXT");
                OLED_ShowHexNum(48, 16, RxMsg.ExtId, FONT_SIZE_8);         // 显示接收扩展 ID
            }

            if (RxMsg.RTR == CAN_RTR_Data) {
                OLED_ShowString(64, 0, FONT_SIZE_8, "DATA  ");
                OLED_ShowNum(48, 32, RxMsg.DLC, FONT_SIZE_8);        // 显示数据长度
                OLED_ShowHexNum(48, 48, RxMsg.Data[0], FONT_SIZE_6);    // 显示数据字节0
                OLED_ShowHexNum(78, 48, RxMsg.Data[1], FONT_SIZE_6);    // 显示数据字节1
                OLED_ShowHexNum(48, 56, RxMsg.Data[2], FONT_SIZE_6);    // 显示数据字节2
                OLED_ShowHexNum(78, 56, RxMsg.Data[3], FONT_SIZE_6);    // 显示数据字节3
            } else if (RxMsg.RTR == CAN_RTR_Remote) {
                OLED_ShowString(64, 0, FONT_SIZE_8, "REMOTE");
                OLED_ShowNum(48, 32, RxMsg.DLC, FONT_SIZE_8);        // 显示数据长度
                OLED_ClearArea(48, 48, 54, 16);    // 清除数据区域
            }

            OLED_Update();  // 更新显示
        }
    }
}
