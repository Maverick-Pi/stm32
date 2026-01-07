/****************************************************************************/ /**
 * @file   Font_Programmer.c
 * @brief  
 * 
 * @author Maverick Pi
 * @date   2025-12-16 16:52:05
 ********************************************************************************/

#include "Font_Programmer.h"
#include <string.h>

void Font_Programmer_CH(void)
{
    uint32_t dataLength = 0;
    uint8_t buffer[FONT_PROGRAMMER_BUFFER_SIZE];
    uint32_t address = FONT_PROGRAMMER_W25Q64_START_ADDR;
    
    Serial_Init();
    // 初始化W25Q64
    W25Q64_Init();
    
    // 发送开始信号给PC
    Serial_SendString("READY\r\n");
    
    // 接收数据长度（4字节，大端格式）
    for (uint8_t i = 0; i < 4; i++) {
        while (Serial_GetRxFlag() == 0);  // 等待数据
        uint8_t byte = Serial_GetRxData();
        dataLength = (dataLength << 8) | byte;
    }
    
    // 发送ACK确认长度接收成功
    Serial_SendByte('A');
    
    // 计算需要接收的块数
    uint32_t totalBlocks = dataLength / FONT_PROGRAMMER_BUFFER_SIZE;
    uint32_t remainingBytes = dataLength % FONT_PROGRAMMER_BUFFER_SIZE;
    
    // 擦除需要的扇区（每个扇区4KB）
    uint32_t sectorsToErase = (dataLength + 4095) / 4096;  // 向上取整
    for (uint32_t i = 0; i < sectorsToErase; i++) {
        W25Q64_EraseSector(address + i * 4096);
    }

    // ☆ 告诉 PC：我擦完了，可以发数据了
    Serial_SendString("ERASE_DONE\r\n");
    
    // 接收并写入完整的数据块
    for (uint32_t block = 0; block < totalBlocks; block++) {
        // 接收一个数据块
        for (uint16_t i = 0; i < FONT_PROGRAMMER_BUFFER_SIZE; i++) {
            while (Serial_GetRxFlag() == 0);  // 等待数据
            buffer[i] = Serial_GetRxData();
        }
        
        // 写入到Flash
        W25Q64_PageProgram(address, buffer, FONT_PROGRAMMER_BUFFER_SIZE);
        address += FONT_PROGRAMMER_BUFFER_SIZE;
        
        // 发送ACK
        Serial_SendByte('A');
    }
    
    // 接收并写入剩余的数据
    if (remainingBytes > 0) {
        // 接收剩余数据
        for (uint16_t i = 0; i < remainingBytes; i++) {
            while (Serial_GetRxFlag() == 0);  // 等待数据
            buffer[i] = Serial_GetRxData();
        }
        
        // 写入到Flash
        W25Q64_PageProgram(address, buffer, remainingBytes);
        
        // 发送ACK
        Serial_SendByte('A');
    }
    
    // 发送完成信号
    Serial_SendString("DONE\r\n");
    Serial_Printf("Font data programmed successfully! Total bytes: %lu\r\n", dataLength);
}