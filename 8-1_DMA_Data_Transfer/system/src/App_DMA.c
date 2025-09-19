/****************************************************************************/ /**
 * @file   App_DMA.c
 * @brief  DMA (Direct Memory Access) application layer implementation
 * 
 * Provides initialization and control functions for DMA-based memory transfers
 * 
 * @author Maverick Pi
 * @date   2025-09-19 15:29:24
 ********************************************************************************/

#include "App_DMA.h"

static uint16_t AppDMA_Size; ///< Stores the configured transfer size for DMA operations

/**
 * @brief Initialize DMA controller for memory-to-memory transfers
 * 
 * Configures DMA1 Channel 1 for byte-sized transfers between two memory address
 * with automatic address increment and very high priority.
 * 
 * @param addrA Source address for DMA transfer
 * @param addrB Destination address for DMA transfer
 * @param size Number of data units to transfer (in bytes)
 */
void App_DMA_Init(uint32_t addrA, uint32_t addrB, uint16_t size)
{
    AppDMA_Size = size;

    // Enable clock for DMA1 peripheral
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    // Configure DMA initialization structure
    DMA_InitTypeDef DMAInitStructure;
    DMAInitStructure.DMA_PeripheralBaseAddr = addrA;
    DMAInitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMAInitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    DMAInitStructure.DMA_MemoryBaseAddr = addrB;
    DMAInitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte ;
    DMAInitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMAInitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMAInitStructure.DMA_BufferSize = size;
    DMAInitStructure.DMA_Mode = DMA_Mode_Normal;
    DMAInitStructure.DMA_M2M = DMA_M2M_Enable;
    DMAInitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_Init(DMA1_Channel1, &DMAInitStructure);

    // Keep DMA channel disabled until explicitly started
    DMA_Cmd(DMA1_Channel1, DISABLE);
}

/**
 * @brief Execute a DMA transfer with configured parameters
 * 
 * Starts a DMA transfer and waits for completion. Clears transfer complete flag
 * after operation finishes.
 */
void App_DMA_Transfer(void)
{
    DMA_Cmd(DMA1_Channel1, DISABLE);    // Ensure DMA channel is disable
    DMA_SetCurrDataCounter(DMA1_Channel1, AppDMA_Size);     // Reload data counter
    DMA_Cmd(DMA1_Channel1, ENABLE);     // Start DMA transfer

    // Wait until transfer complete flag is set
    while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
    DMA_ClearFlag(DMA1_FLAG_TC1);   // Clear transfer complete flag
}
