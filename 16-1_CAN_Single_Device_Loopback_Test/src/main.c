/****************************************************************************/ /**
 * @file   main.c
 * @brief  基于 STM32 的 CAN 总线通信测试程序
 *         通过 OLED 显示发送/接收的 CAN 报文信息，按键控制数据发送
 * 
 * @author Maverick Pi
 * @date   2026-02-03 01:17:06
 ********************************************************************************/

#include "stm32f10x.h"
#include "OLED.h"
#include "Key.h"
#include "CAN_Hardware.h"

uint8_t KeyNum = 0;     // 按键键值变量

uint32_t TxID = 0x111;  // 发送帧标准 ID
uint8_t TxLength = 4;   // 发送数据长度
uint8_t TxData[8] = { 0x11, 0x22, 0x33, 0x44};  // 发送数据缓冲区

uint32_t RxID;      // 接收帧 ID
uint8_t RxLength;   // 接收数据长度
uint8_t RxData[8];  // 接收数据缓冲区

int main(void)
{
    /* 外设初始化 */
    OLED_Init();    // OLED 显示屏初始化
    OLED_Clear();   // 清屏
    OLED_Update();  // 更新显示
    Key_Init();     // 按键初始化
    CAN_Hardware_Init();    // CAN 硬件初始化

    /* 显示固定界面元素 */
    OLED_ShowString(0, 0, FONT_SIZE_8, "TxID:");    // 显示发送 ID 标签
    OLED_ShowHexNum(48, 0, TxID, FONT_SIZE_8);      // 显示发送 ID 值
    OLED_ShowString(0, 16, FONT_SIZE_8, "RxID:");   // 显示接收 ID 标签
    OLED_ShowString(0, 32, FONT_SIZE_8, "Leng:");   // 显示数据长度标签
    OLED_ShowString(0, 48, FONT_SIZE_8, "Data:");   // 显示数据标签
    OLED_Update();  // 更新显示

    while (1) {
        /* 检测按键输入 */
        KeyNum = Key_GetNum();
        
        /* 按键 1 按下时发送 CAN 数据并递增数据内容 */
        if (KeyNum == 1) {
            /* 递增发送数据缓冲区内容 */
            for (int i = 0; i < TxLength; ++i) {
                TxData[i]++;
            }
            /* 发送 CAN 报文 */
            CAN_Hardware_Transmit(TxID, TxData, TxLength);
        }

        /* 检测是否有 CAN 数据接收 */
        if (CAN_Hardware_ReceiveFlag()) {
            /* 读取接收到的 CAN 报文 */
            CAN_Hardware_Receive(&RxID, RxData, &RxLength);

            /* 在 OLED 上显示接收信息 */
            OLED_ShowHexNum(48, 16, RxID, FONT_SIZE_8);         // 显示接收 ID
            OLED_ShowNum(48, 32, RxLength, FONT_SIZE_8);        // 显示数据长度
            OLED_ShowHexNum(48, 48, RxData[0], FONT_SIZE_6);    // 显示数据字节0
            OLED_ShowHexNum(78, 48, RxData[1], FONT_SIZE_6);    // 显示数据字节1
            OLED_ShowHexNum(48, 56, RxData[2], FONT_SIZE_6);    // 显示数据字节2
            OLED_ShowHexNum(78, 56, RxData[3], FONT_SIZE_6);    // 显示数据字节3
            OLED_Update();  // 更新显示
        }
    }
}
