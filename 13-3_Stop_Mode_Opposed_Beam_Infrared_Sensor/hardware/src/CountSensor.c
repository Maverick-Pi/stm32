/****************************************************************************/ /**
 * @file   CountSensor.c
 * @brief  Implementation of Count Sensor module
 * 
 * @author Maverick Pi
 * @date   2025-08-04 18:58:40
 ********************************************************************************/

#include "CountSensor.h"

uint16_t CountSensor_Count;     // Global variable to store pulse count

/**
 * @brief Initialize GPIO, EXTI, and NVIC for count sensor
 * 
 * Configures PB14 as input with pull-down and sets up falling edge interrupt
 */
void CountSensor_Init(void)
{
    // Enable GPIOB and AFIO clocks
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    // Configure PB14 as input with pull-up
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // Map EXTI14 to PB14
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);

    // Configure EXTI for falling edge trigger
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line14;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStructure);

    // Set NVIC priority grouping
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    // Configure EXTI15_10 interrupt channel
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief Get current pulse count value
 * 
 * @return uint16_t Current count value
 */
uint16_t CountSensor_GetCount(void)
{
    return CountSensor_Count;
}

/**
 * @brief EXTI15_10 interrupt handler
 * 
 * Processes falling edge events on PB14 with debouncing delay
 */
void EXTI15_10_IRQHandler(void)
{
    // Check if EXTI Line14 interrupt occurred
    if (EXTI_GetITStatus(EXTI_Line14) == SET) {
        CountSensor_Count++;    // Increment pulse counter

        // Clear interrupt pending bit
        EXTI_ClearITPendingBit(EXTI_Line14);
    }
}
