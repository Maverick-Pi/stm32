/****************************************************************************/ /**
 * @file   ADC.c
 * @brief  Source file for Analog-to-Digital Converter (ADC) driver.
 * 
 * This file provides the implementation for initializing the ADC peripheral
 * and reading analog values from specified channels.
 * 
 * @author Maverick Pi
 * @date   2025-09-15 21:32:16
 ********************************************************************************/

#include "ADC.h"

/**
 * @brief Initializes the ADC peripheral and GPIO pins for analog input.
 * 
 * This function enables the necessary clocks, configures the GPIO pins for
 * analog mode, sets up the ADC in independent mode with right-aligned data,
 * and performs ADC calibration to ensure accurate conversions.
 * 
 * @note The ADC clock is configured to PCLK2 divided by 6 to meet the maximum
 *       ADC clock frequency requirement (<= 14 MHz)
 */
void AD_Init(void)
{
    /* 1) Enable peripheral clocks */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   // GPIOA clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);    // ADC1 clock

    /* 2) Set ADC clock = PCLK2 / 6 (<= 14MHz per datasheet) */
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    /* 3) Configure PA0, PA1, PA2, PA3 as analog input (ADC1_IN0 to ADC1_IN3) */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;       // pure analog
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   // irrelevant for analog
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* 4) Configure ADC1 core */
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;     // Single conversion mode
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  // 12-bit right aligned
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;     // Software trigger
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  // Independent ADC mode
    ADC_InitStructure.ADC_NbrOfChannel = 1;     // One conversion in sequence
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;   // Disable scan mode
    ADC_Init(ADC1, &ADC_InitStructure);

    /* 5) Enable ADC1 and perform calibration (mandatory after power-on) */
    ADC_Cmd(ADC1, ENABLE);

    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1) == SET);
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1) == SET);

    /* Optional: throw away the first dummy conversion after enabling, if desired.
       Not strictly required here since we calibrate and do on-demand reads */
}

/**
 * @brief Reads the analog vlaue from a specified ADC channel.
 * 
 * This function configures the specified ADC channel, starts a conversion,
 * waits for the conversion to complete, and returns the 12-bit result.
 * 
 * @param ADC_Channel The ADC channel to read from (eg. ADC_Channel_0)
 * @return uint16_t The 12-bit analog conversion result (0..4095)
 */
uint16_t AD_GetValue(uint8_t ADC_Channel)
{
    // Configure the channel and sample time for the next conversion
    ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_55Cycles5);
    // Start conversion on the regular group
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    // Wait until End-Of-Conversion flag is set (~5.67 μs with current timing)
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    // Read and return the conversion result
    return ADC_GetConversionValue(ADC1);
}
