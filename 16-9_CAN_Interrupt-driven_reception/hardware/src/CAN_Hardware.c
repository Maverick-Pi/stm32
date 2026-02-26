/****************************************************************************/ /**
 * @file   CAN_Hardware.c
 * @brief  CAN 硬件驱动源文件
 *         实现 CAN 控制器初始化、发送、接收等功能
 * 
 * @author Maverick Pi
 * @date   2026-02-02 19:19:17
 ********************************************************************************/

#include "CAN_Hardware.h"


CanRxMsg CAN_Hardware_RxMsg;
uint8_t CAN_Hardware_RxFlag;

/**
 * @brief CAN 硬件初始化函数
 *        配置 GPIO、CAN 控制器工作模式和波特率，初始化 CAN 过滤器
 * 
 */
void CAN_Hardware_Init(void)
{
    /* 开启外设时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    /* 配置 CAN 引脚：PA11 为 RX（上拉输入），PA12 为 TX（复用推挽输出）*/
    GPIO_Init(GPIOA, &(GPIO_InitTypeDef) {
        .GPIO_Pin = GPIO_Pin_11,        // CAN_RX 引脚
        .GPIO_Speed = GPIO_Speed_50MHz,
        .GPIO_Mode = GPIO_Mode_IPU      // 上拉输入模式
    });

    GPIO_Init(GPIOA, &(GPIO_InitTypeDef) {
        .GPIO_Pin = GPIO_Pin_12,        // CAN_TX 引脚
        .GPIO_Speed = GPIO_Speed_50MHz,
        .GPIO_Mode = GPIO_Mode_AF_PP    // 复用推挽s输出模式
    });

    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_Init(&(NVIC_InitTypeDef) {
        .NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn,
        .NVIC_IRQChannelCmd = ENABLE,
        .NVIC_IRQChannelPreemptionPriority = 1,
        .NVIC_IRQChannelSubPriority = 1
    });

    /* CAN 控制器初始化配置 */
    CAN_Init(CAN1, &(CAN_InitTypeDef) {
        .CAN_Mode = CAN_Mode_LoopBack,  // 环回模式（自测试）
        .CAN_Prescaler = 48,            // 预分频器：波特率 = 36M / 48 / (1+2+3) = 125Kbps
        .CAN_BS1 = CAN_BS1_2tq,         // 时间段1 为 2 个时间单位
        .CAN_BS2 = CAN_BS2_3tq,         // 时间段2 为 3 个时间单位
        .CAN_SJW = CAN_SJW_2tq,         // 同步跳转宽度为 2 个时间单位
        .CAN_TTCM = DISABLE,            // 禁用时间触发通信模式
        .CAN_ABOM = DISABLE,            // 禁用自动总线关闭管理
        .CAN_AWUM = DISABLE,            // 禁用自动唤醒模式
        .CAN_NART = DISABLE,            // 禁用非自动重传模式
        .CAN_RFLM = DISABLE,            // 禁用接收 FIFO 锁定模式
        .CAN_TXFP = DISABLE             // 禁用发送 FIFO 优先级
    });

    /* CAN 过滤器初始化（配置为接收所有报文）*/
    CAN_FilterInit(&(CAN_FilterInitTypeDef) {
        .CAN_FilterNumber = 0,                      // 过滤器组0
        .CAN_FilterIdHigh = 0x0000,                 // 过滤器 ID 高 16 位
        .CAN_FilterIdLow = 0x0000,                  // 过滤器 ID 低 16 位
        .CAN_FilterMaskIdHigh = 0x0000,             // 过滤器掩码高 16 位
        .CAN_FilterMaskIdLow = 0x0000,              // 过滤器掩码低 16 位
        .CAN_FilterScale = CAN_FilterScale_32bit,   // 32 位过滤器
        .CAN_FilterMode = CAN_FilterMode_IdMask,    // 掩码模式
        .CAN_FilterFIFOAssignment = CAN_Filter_FIFO0,   // 分配到 FIFO 0
        .CAN_FilterActivation = ENABLE                  // 激活过滤器
    });
}

/**
 * @brief CAN 发送函数
 *        将指定的CAN报文发送到总线上，并等待发送完成或超时
 * 
 * @param txMsg 指向待发送CAN报文的指针
 */
void CAN_Hardware_Transmit(CanTxMsg* txMsg)
{
    /* 配置发送报文结构体并发送 */
    uint8_t transmitMailbox = CAN_Transmit(CAN1, txMsg);

    /* 等待发送完成或超时 */
    uint32_t timeout = 0x186A0;     // 超时计数
    while (CAN_TransmitStatus(CAN1, transmitMailbox) != CAN_TxStatus_Ok) {
        if (--timeout == 0) {   // 超时判断
            break;
        }
    }
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
    if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) == SET) {
        CAN_Receive(CAN1, CAN_FIFO0, &CAN_Hardware_RxMsg);
        CAN_Hardware_RxFlag = 1;    // 设置接收标志
    }
}
