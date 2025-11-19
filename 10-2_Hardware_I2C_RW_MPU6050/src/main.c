/****************************************************************************/ /**
 * @file   main.c
 * @brief  Hardware I2C Read/Write MPU6050
 * 
 * @author Maverick Pi
 * @date   2025-11-18 19:32:11
 ********************************************************************************/

#include "stm32f10x.h"
#include "MPU6050.h"
#include "OLED.h"

int main(void)
{
    // Initialize OLED display and MPU6050 sensor
    OLED_Init();
    MPU6050_Init();
    
    // Structure to store sensor data
    Attitude attitude;

    // Display device ID on OLED
    OLED_ShowString(1, 1, "ID:");
    OLED_ShowHexNum(1, 4, MPU6050_GetID(), 2);

    while (1) {
        // Read attitude data from MPU6050
        MPU6050_GetAttitudeData(&attitude);

        // Display accelerometer data on OLED
        OLED_ShowSignedNum(2, 1, attitude.AccelX, 5);
        OLED_ShowSignedNum(3, 1, attitude.AccelY, 5);
        OLED_ShowSignedNum(4, 1, attitude.AccelZ, 5);
        // Display gyroscope data on OLED
        OLED_ShowSignedNum(2, 8, attitude.GyroX, 5);
        OLED_ShowSignedNum(3, 8, attitude.GyroY, 5);
        OLED_ShowSignedNum(4, 8, attitude.GyroZ, 5);
    }
}
