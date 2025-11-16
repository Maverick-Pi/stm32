/****************************************************************************/ /**
 * @file   MPU6050.h
 * @brief  MPU6050 IMU Sensor Driver Header File
 * 
 * @author Maverick Pi
 * @date   2025-11-14 22:51:29
 ********************************************************************************/
#ifndef __MPU6050_H__
#define __MPU6050_H__

#include "stm32f10x.h"
#include "MyI2C.h"
#include "MPU6050_Reg.h"
#include "OLED.h"

#define MPU6050_ADDRESS 0xD0    // MPU6050 I2C device address (8-bit format)

/**
 * @brief Structure to store IMU sensor data
 * 
 */
typedef struct {
    int16_t AccelX;     // Accelerometer X-axis data
    int16_t AccelY;     // Accelerometer Y-axis data
    int16_t AccelZ;     // Acceleromter Z-axis data
    int16_t GyroX;      // Gyroscope X-axis data
    int16_t GyroY;      // Gyroscope Y-axis data
    int16_t GyroZ;      // Gyroscope Z-axis data
} Attitude;

void MPU6050_Init(void);
void MPU6050_WriteReg(uint8_t regAddr, uint8_t data);
uint8_t MPU6050_ReadReg(uint8_t regAddr);
void MPU6050_ReadRegs(uint8_t regAddr, uint8_t *data, uint8_t len);
uint8_t MPU6050_GetID(void);
void MPU6050_GetAttitudeData(Attitude *attitude);

#endif // !__MPU6050_H__
