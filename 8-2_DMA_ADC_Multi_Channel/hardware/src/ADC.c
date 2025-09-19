/****************************************************************************/ /**
 * @file   ADC.c
 * @brief  Implementation of Analog-to-Digital Converter (ADC) module
 * 
 * This file contains the implementation of ADC initialization and DMA configuration
 * for continuous multi-channel analog data acquisition on STM32F10x processors.
 * 
 * @author Maverick Pi
 * @date   2025-09-19 22:14:12
 *******************************************************************************/

#include "ADC.h"

/** Global array to store ADC conversion results for four channels */
uint16_t AD_Value[4];

/**
 * @brief Initializes ADC peripheral and DMA for continuous multi-channel conversion
 * 
 * This function performs the following configurations:
 * 1. Enables clocks for GPIOA, ADC1, and DMA1 peripherals
 * 2. Configures four GPIO pins (PA0-PA3) as analog inputs
 * 3. Sets up ADC regular channels with specified sampling time
 * 4. Configures ADC for continuous conversion mode with right-aligned data
 * 5. Initializes DMA for automatic transfer of conversion results to memory
 * 6. Performs ADC calibration and starts continuous conversion
 * 
 * @note DMA is configured in circular mode for continuous data transfer
 * @note ADC is set to independent mode with software trigger
 */
void AD_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   // GPIOA clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);    // ADC1 clock
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;       // pure analog
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   // irrelevant for analog
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);

    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  // 12-bit right aligned
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;     // Software trigger
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  // Independent ADC mode
    ADC_InitStructure.ADC_NbrOfChannel = 4;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_Init(ADC1, &ADC_InitStructure);

    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &ADC1->DR;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) AD_Value;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStructure.DMA_BufferSize = 4;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    DMA_Cmd(DMA1_Channel1, ENABLE);

    ADC_DMACmd(ADC1, ENABLE);

    ADC_Cmd(ADC1, ENABLE);

    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1) == SET);
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1) == SET);

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
