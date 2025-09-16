/****************************************************************************/ /**
 * @file   ADC.c
 * @brief  STM32F10x StdPeriph implementation for a single ADC read on CH0.
 * 
 * The ADC is configured for single, software-triggered conversions. The ADC
 * clock is derived from PCLK2 divided by 6 to satisfy the <= 14 MHz limit.
 * A sample time of 55.5 cycles is chosen, giving a total conversion time of
 * 12.5 + 55.5 = 68 cycles. At 12 MHz ADCCLK this is ~5.67 μs per conversion.
 * 
 * @author Maverick Pi
 * @date   2025-09-15 21:32:16
 ********************************************************************************/

#include "ADC.h"

/**
 * @brief Initialize GPIOA.0 and ADC1 for single-shot conversions.
 * 
 */
void AD_Init(void)
{
    /* 1) Enable peripheral clocks */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   // GPIOA clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);    // ADC1 clock

    /* 2) Set ADC clock = PCLK2/6 (<= 14MHz per datasheet) */
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    /* 3) Configure PA0 as analog input (ADC1_IN0) */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;       // pure analog
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;           // PA0
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   // don't care for analog
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* 4) Select regular channel 0, rank 1, sample time 55.5 cycles */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);

    /* 5) Configure ADC1 core */
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;     // single conversion
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  // 12-bit right aligned
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;     // software trigger
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  // single ADC
    ADC_InitStructure.ADC_NbrOfChannel = 1;     // one rank
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;   // 1 channel only
    ADC_Init(ADC1, &ADC_InitStructure);

    /* 6) Enable ADC and perform calibration (mandatory after power-on) */
    ADC_Cmd(ADC1, ENABLE);

    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1) == SET);
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1) == SET);

    /* Optional: throw away the first dummy conversion after enabling, if desired.
       Not strictly required here since we calibrate and do on-demand reads */
}

/**
 * @brief Perform one blocking software-triggered conversion on ADC1.
 * 
 * @return uint16_t 12-bit raw ADC code (0..4095)
 */
uint16_t AD_GetValue(void)
{
    // Start conversion on the regular group
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    // Wait until End-Of-Conversion flag is set (~5.67 μs with current timing)
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    // Read and return the conversion result
    return ADC_GetConversionValue(ADC1);
}
