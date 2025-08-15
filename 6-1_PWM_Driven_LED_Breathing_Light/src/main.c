/****************************************************************************/ /**
 * @file   main.c
 * @brief  PWM-driven LED breathing light with gamma correction
 * 
 * Implementation of a breathing LED effect using PWM output with gamma correction
 * for perceptually linear brightness transitions. Uses timer-based PWM and delay
 * functions.
 * 
 * @author Maverick Pi
 * @date   2025-08-13 14:23:56
 ********************************************************************************/

#include "PWM.h"
#include "Delay.h"

/**
 * @brief Gamma correction lookup table for LED brightness
 * 
 * Maps linear brightness values (0-100) to gamma-corrected PWM duty cycles to
 * achieve perceptually linear brightness transitions. Values range from 0 to 100
 * where 100 represents maximum brightness (100% duty cycle).
 */
uint8_t GammaTable[101] = {
    0, 0, 0, 0, 0, 0, 0, 1, 1, 1,                         // 0-9
    1, 1, 2, 2, 2, 3, 3, 4, 4, 5,                         // 10-19
    5, 6, 7, 8, 9, 10, 11, 12, 13, 14,                    // 20-29
    15, 16, 18, 19, 21, 22, 24, 25, 27, 29,               // 30-39
    31, 33, 35, 37, 39, 41, 43, 46, 48, 51,               // 40-49
    53, 56, 59, 61, 64, 67, 70, 73, 76, 79,               // 50-59
    82, 85, 88, 91, 94, 97, 100, 100, 100, 100,           // 60-69
    100, 100, 100, 100, 100, 100, 100, 100, 100, 100,     // 70-79
    100, 100, 100, 100, 100, 100, 100, 100, 100, 100,     // 80-89
    100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 // 90-100
};

/**
 * @brief Main application entry point
 * 
 * Initializes PWM hardware and runs infinite loop implementing breathing LED effect:
 *  1. Gradually increases LED brightness (0% -> 100% with gamma correction)
 *  2. Gradually decreases LED brightness (100% -> 0% with gamma correction)
 * Each brightness step lasts 10ms resulting in ~2 second full cycle.
 * 
 * @return int Program status (not used in embedded context)
 */
int main(void)
{
    PWM_Init();     // Initialize PWM hardware

    while (1) {
        // Fade in: 0% to 100% brightness
        for (uint8_t i = 0; i <= 100; ++i) {
            PWM_SetCompare1(GammaTable[i]);     // Set gamma-corrected duty cycle
            Delay_ms(10);   // Maintain step for 10ms
        }

        // Fade out: 100% to 0% brightness
        for (uint8_t i = 0; i <= 100; ++i) {
            PWM_SetCompare1(GammaTable[100 - i]);   // Apply inverse gamma correction
            Delay_ms(10);   // Maintain step for 10ms
        }
    }
}
