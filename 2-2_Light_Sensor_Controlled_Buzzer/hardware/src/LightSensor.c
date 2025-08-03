/****************************************************************************/ /**
 * @file   LightSensor.c
 * @brief  Light Sensor Module Implementation
 * 
 * Implements GPIO initialization and reading functions for light sensing.
 * Configures specified pin on GPIOB as input pull-up for ambient light detection.
 * 
 * @author Maverick Pi
 * @date   2025-08-03 14:49:30
 ********************************************************************************/

#include "LightSensor.h"

/**
 * @brief Initializes the light sensor GPIO pin as input pull-up
 * 
 * @param GPIO_Pin Specifies the GPIO pin to initialize
 */
void LightSensor_Init(uint16_t GPIO_Pin)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   // Input pull-up mode
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
 * @brief Reads the current state of the light sensor
 * 
 * @param GPIO_Pin Specifies the GPIO pin to read
 * @return uint8_t Current light sensor state (0=bright, 1=dark)
 */
uint8_t LightSensor_Get(uint16_t GPIO_Pin)
{
    return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin);
}
