/****************************************************************************/ /**
 * @file   main.c
 * @brief  Light Sensor Controlled Buzzer Application
 * 
 * Main application that links light sensor input to buzzer output. When light
 * is detected (bright conditions), triggers a double-beep alarm pattern with
 * timed delays. Maintains silence in dark conditions.
 * 
 * Hardware Configuration:
 * - Buzzer: GPIOB Pin 12 (active low)
 * - Light Sensor: GPIOB Pin 13 (pull-up, 0=bright, 1=dark)
 * 
 * Behavior:
 * - Bright: Two 100ms beeps separated by 100ms pause, then 700ms silence
 * - Dark: Continuous silence
 * 
 * @author Maverick Pi
 * @date   2025-08-03 13:39:50
 ********************************************************************************/

#include "Buzzer.h"
#include "LightSensor.h"
#include "Delay.h"

/**
 * @brief Main application: Activates buzzer pattern when light is detected
 * 
 * Initializes buzzer (PB12) and light sensor (PB13). Continuously monitors
 * light sensor. When bright conditions are detected (luminance=1), triggers
 * a double-beep pattern (100ms ON, 100ms OFF, 100ms ON, 700ms OFF)
 * 
 * @return int Standard return (not used in embedded loop)
 */
int main(void)
{
    uint8_t luminance;  // Light sensor reading

    Buzzer_Init(GPIO_Pin_12);
    LightSensor_Init(GPIO_Pin_13);

    while (1) {
        
        luminance = LightSensor_Get(GPIO_Pin_13);

        if (luminance) {    // Double beep pattern
            Buzzer_Control(BUZZER_ON, GPIO_Pin_12);
            Delay_ms(100);
            Buzzer_Control(BUZZER_OFF, GPIO_Pin_12);
            Delay_ms(100);
            Buzzer_Control(BUZZER_ON, GPIO_Pin_12);
            Delay_ms(100);
            Buzzer_Control(BUZZER_OFF, GPIO_Pin_12);
            Delay_ms(700);
        } else {    // Keep buzzer off in bright conditions
            Buzzer_Control(BUZZER_OFF, GPIO_Pin_12);
        }
    }
}
