/****************************************************************************/ /**
 * @file   MPU6050.c
 * @brief  MPU6050 IMU Sensor Driver Implementation
 * 
 * This file contains the implementation of functions for initializing and 
 * communicating with the MPU6050 6-axis motion tracking sensor via I2C interface.
 * 
 * @author Maverick Pi
 * @date   2025-11-14 22:51:24
 ********************************************************************************/

#include "MPU6050.h"

/**
 * @brief  Initialize MPU6050 sensor with default configuration
 * 
 */
void MPU6050_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; // I2C2 SCL and SDA
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    I2C_InitTypeDef I2C_InitStructure;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 300000; // 300kHz
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_16_9;
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_Init(I2C2, &I2C_InitStructure);

    // Configure MPU6050 registers
    MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);     // Set clock source to PLL with X-axis gyro referen
    MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);     // Enable all sensors
    MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x01);     // Set sample rate divider
    MPU6050_WriteReg(MPU6050_CONFIG, 0x06);         // Configure DLPF (Digital Low Pass Filter)
    MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x10);    // Set gyroscope full scale range to ±1000°/s
    MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x08);   // Set accelerometer full scale range to ±4g
}

/**
 * @brief Wait for a specific I2C event with timeout
 * 
 * @param I2Cx I2C peripheral (e.g., I2C2)
 * @param I2C_Event I2C event to wait for
 */
void MPU6050_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_Event)
{
    uint32_t timeout = 3000;
    while (I2C_CheckEvent(I2Cx, I2C_Event) != SUCCESS) {
        if ((timeout--) == 0) {
            break;
        }
    }
}

/**
 * @brief Write data to MPU6050 register
 * 
 * This function performs a complete I2C write transaction to write a single byte
 * to the specified MPU6050 register.
 * 
 * @param regAddr Register address to write to
 * @param data Data byte to write to the register
 */
void MPU6050_WriteReg(uint8_t regAddr, uint8_t data)
{
    I2C_GenerateSTART(I2C2, ENABLE);
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);

    I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);

    I2C_SendData(I2C2, regAddr);
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);

    I2C_SendData(I2C2, data);
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);

    I2C_GenerateSTOP(I2C2, ENABLE);
}

/**
 * @brief Read single byte from MPU6050 register
 * 
 * This function performs a complete I2C read transaction to read a single byte
 * from the specified MPU6050 register.
 * 
 * @param regAddr Register address to read from
 * @return uint8_t Data byte read from the register
 */
uint8_t MPU6050_ReadReg(uint8_t regAddr)
{
    I2C_GenerateSTART(I2C2, ENABLE);
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);

    I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);

    I2C_SendData(I2C2, regAddr);
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);

    I2C_GenerateSTART(I2C2, ENABLE);
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);

    I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Receiver);
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);

    I2C_AcknowledgeConfig(I2C2, DISABLE);
    I2C_GenerateSTOP(I2C2, ENABLE);

    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);
    uint8_t data = I2C_ReceiveData(I2C2);

    I2C_AcknowledgeConfig(I2C2, ENABLE);

    return data;
}

/**
 * @brief Read multiple bytes from MPU6050 registers
 * 
 * This function performs a burst read operation to read multiple consecutive
 * bytes starting from the specified register address.
 * 
 * @param regAddr Starting register address to read from
 * @param data Pointer to buffer for storing read data
 * @param len Number of bytes to read
 */
void MPU6050_ReadRegs(uint8_t regAddr, uint8_t *data, uint8_t len)
{
    I2C_GenerateSTART(I2C2, ENABLE);
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);

    I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);

    I2C_SendData(I2C2, regAddr);
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);

    I2C_GenerateSTART(I2C2, ENABLE);
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);

    I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Receiver);
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);

    for (uint8_t i = 0; i < len; i++) {
        if (i == (len - 1)) {
            I2C_AcknowledgeConfig(I2C2, DISABLE);
            I2C_GenerateSTOP(I2C2, ENABLE);
        }

        MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);
        data[i] = I2C_ReceiveData(I2C2);
    }

    I2C_AcknowledgeConfig(I2C2, ENABLE);
}

/**
 * @brief Read MPU6050 device ID
 * 
 * Reads the WHO_AM_I register to verify device identity and communication.
 * 
 * @return uint8_t Device ID (should be 0x68 for MPU6050)
 */
uint8_t MPU6050_GetID(void)
{
    return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

/**
 * @brief Read accelerometer and gyroscope data from MPU6050
 * 
 * Reads all 6-axis sensor data (3-axis accelerometer and 3-axis gyroscope)
 * in a single burst read operation for efficiency.
 * 
 * @param attitude Pointer to Attitude structure to store sensor data
 */
void MPU6050_GetAttitudeData(Attitude *attitude)
{
    // Buffer for 14 bytes of sensor data (6 accel + 2 temp + 6 gyro)
    uint8_t data[14];

    // Read 14 bytes starting from accelerometer X-axis high byte
    MPU6050_ReadRegs(MPU6050_ACCEL_XOUT_H, data, 14);

    // Combine high and low bytes for accelerometer data (16-bit values)
    attitude->AccelX = (data[0] << 8) | data[1];
    attitude->AccelY = (data[2] << 8) | data[3];
    attitude->AccelZ = (data[4] << 8) | data[5];

    // Combine high and low bytes for gyroscope data (16-bit values)
    attitude->GyroX = (data[8] << 8) | data[9];
    attitude->GyroY = (data[10] << 8) | data[11];
    attitude->GyroZ = (data[12] << 8) | data[13];
}
