/****************************************************************************/ /**
 * @file   main.c
 * @brief  Main application for key-controlled LED system
 * 
 * Implements key scanning and LED control logic where:
 * - Key1 toggles LED1 (PA1)
 * - Key2 toggles LED2 (PA2)
 * 
 * @author Maverick Pi
 * @date   2025-08-02 19:16:50
 ********************************************************************************/

#include "LED.h"
#include "Key.h"

/**
 * @brief Main application entry point
 * 
 * Initializes peripherals and runs infinite key scanning loop.
 * 
 * @return int Unused (compliance with C standard)
 */
int main(void)
{
    uint8_t keyNum;

    LED_Init();     // Initialize LED GPIOs
    Key_Init();     // Initialize key GPIOs

    // Main superloop
    while (1) {
        keyNum = Key_GetNum();  // Get pressed key
        if (keyNum == 1) LED_Turn(GPIO_Pin_1);  // Toggle LED1 (PA1)
        if (keyNum == 2) LED_Turn(GPIO_Pin_2);  // Toggle LED2 (PA2)
    }
}
