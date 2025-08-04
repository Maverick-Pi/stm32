/****************************************************************************/ /**
 * @file   Rotary_Encoder.c
 * @brief  Driver implementation for rotary encoder using GPIO interrupts
 * 
 * Uses pin PB0 (CLK) and PB1 (DT) with falling edge interrupts for decoding.
 * Implements quadrature decoding in interrupt handlers.
 * 
 * @author Maverick Pi
 * @date   2025-08-04 22:19:53
 ********************************************************************************/

#include "Rotary_Encoder.h"

// Global counter for encoder pulses (volatile recommended for ISR access)
int16_t RotaryEncoder_Counter;

/**
 * @brief Initialize GPIOs, external interrupts and NVIC for rotary encoder
 * 
 * Configures PB0 and PB1 as pull-up inputs with falling edge interrupts.
 * Sets up EXTI0 and EXTI1 interrupt handlers with NVIC priority.
 */
void RotaryEncoder_Init(void)
{
    // Enable GPIOB and AFIO clocks
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    // Configure PB0 and PB1 as input pull-up
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // Map GPIO pins to EXTI lines
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);

    // Configure EXTI for falling edge detection
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line1;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStructure);

    // Set NVIC priority grouping
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    // Configure EXTI0 interrupt (PB0)
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);

    // Configure EXTI1 interrupt (PB1)
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief Read and reset accumulated encoder count
 * 
 * @return int16_t Number of pulses since last read (positive for CW, negative for CCW)
 */
int16_t RotaryEncoder_GetCount(void)
{
    int16_t temp = RotaryEncoder_Counter;
    RotaryEncoder_Counter = 0;  // Reset after reading
    return temp;
}

/**
 * @brief EXTI0 interrupt handler (PB0/CLK pin)
 * 
 * Handles falling edge on CLK pin. Checks DT pin state to determine direction.
 */
void EXTI0_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line0) == SET) {
        // When CLK falls, check DT state to determine direction
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == RESET) {
            RotaryEncoder_Counter--;    // DT low = Counter-clockwise
        }

        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

/**
 * @brief EXTI1 interrupt handler (PB1/DT pin)
 * 
 * Handles falling edge on DT pin. Checks CLK pin state to determine direction.
 */
void EXTI1_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line1) == SET) {
        // When DT falls, check CLK state to determine direction
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == RESET) {
            RotaryEncoder_Counter++;    // CLK low = Clockwise
        }
        
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}
