/****************************************************************************/ /**
 * @file   MPU6050.c
 * @brief  MPU6050 IMU Sensor Driver Implementation
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
    // Initialize I2C interface
    MyI2C_Init();
    // Configure MPU6050 registers
    MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);     // Set clock source to PLL with X-axis gyro referen
    MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);     // Enable all sensors
    MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x01);     // Set sample rate divider
    MPU6050_WriteReg(MPU6050_CONFIG, 0x06);         // Configure DLPF (Digital Low Pass Filter)
    MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x10);    // Set gyroscope full scale range to ±1000°/s
    MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x08);   // Set accelerometer full scale range to ±4g
}

/**
 * @brief Write data to MPU6050 register
 * 
 * @param regAddr Register address to write to
 * @param data Data to write
 */
void MPU6050_WriteReg(uint8_t regAddr, uint8_t data)
{
    MyI2C_Start();

    // Send device address in write mode
    MyI2C_SendByte(MPU6050_ADDRESS);
    if (MyI2C_ReceiveAck() == 1) {
        OLED_ShowString(4, 1, "                ");
        OLED_ShowString(4, 1, "S_MPU NOT ACK");
    }

    // Send register address
    MyI2C_SendByte(regAddr);
    if (MyI2C_ReceiveAck() == 1) {
        OLED_ShowString(4, 1, "                ");
        OLED_ShowString(4, 1, "S_REG NOT ACK");
    }

    // Send data to register
    MyI2C_SendByte(data);
    if (MyI2C_ReceiveAck() == 1) {
        OLED_ShowString(4, 1, "                ");
        OLED_ShowString(4, 1, "S_D NOT ACK");
    }

    MyI2C_Stop();
}

/**
 * @brief Read single byte from MPU6050 register
 * 
 * @param regAddr Register address to read from
 * @return uint8_t Data read from register
 */
uint8_t MPU6050_ReadReg(uint8_t regAddr)
{
    MyI2C_Start();

    // Send device address in write mode to set register pointer
    MyI2C_SendByte(MPU6050_ADDRESS);
    if (MyI2C_ReceiveAck() == 1) {
        OLED_ShowString(4, 1, "                ");
        OLED_ShowString(4, 1, "RS_MPU NOT ACK");
    }

    // Send register address to read from
    MyI2C_SendByte(regAddr);
    if (MyI2C_ReceiveAck() == 1) {
        OLED_ShowString(4, 1, "                ");
        OLED_ShowString(4, 1, "R_SReg NOT ACK");
    }

    // Restart I2C communication in read mode
    MyI2C_Start();
    MyI2C_SendByte(MPU6050_ADDRESS | 0x01);     // Set read mode bit
    if (MyI2C_ReceiveAck() == 1) {
        OLED_ShowString(4, 1, "                ");
        OLED_ShowString(4, 1, "R_MPU NOT ACK");
    }

    // Read data byte
    uint8_t byteData = MyI2C_ReceiveByte();
    MyI2C_SendAck(1);   // Send NACK to terminate read
    MyI2C_Stop();
    
    return byteData;
}

/**
 * @brief Read multiple bytes from MPU6050 registers
 * 
 * @param regAddr Starting register address to read from
 * @param data Pointer to buffer for storing read data
 * @param len Number of bytes to read
 */
void MPU6050_ReadRegs(uint8_t regAddr, uint8_t *data, uint8_t len)
{
    MyI2C_Start();

    // Send device address in write mode to set register pointer
    MyI2C_SendByte(MPU6050_ADDRESS);
    if (MyI2C_ReceiveAck() == 1) {
        OLED_ShowString(4, 1, "                ");
        OLED_ShowString(4, 1, "RS_MPU NOT ACK");
        MyI2C_Stop();
        return;
    }

    // Send starting register address
    MyI2C_SendByte(regAddr);
    if (MyI2C_ReceiveAck() == 1) {
        OLED_ShowString(4, 1, "                ");
        OLED_ShowString(4, 1, "R_SReg NOT ACK");
        MyI2C_Stop();
        return;
    }

    // Restart I2C communication in read mode
    MyI2C_Start();
    MyI2C_SendByte(MPU6050_ADDRESS | 0x01);     // Set read mode bit
    if (MyI2C_ReceiveAck() == 1) {
        OLED_ShowString(4, 1, "                ");
        OLED_ShowString(4, 1, "R_MPU NOT ACK");
        MyI2C_Stop();
        return;
    }

    // Read multiple bytes sequentially
    for (uint8_t i = 0; i < len; i++) {
        data[i] = MyI2C_ReceiveByte();

        // Send ACK for all bytes except the last one
        if (i == len - 1) {
            MyI2C_SendAck(1); // NACK
        } else {
            MyI2C_SendAck(0); // ACK
        }
    }

    MyI2C_Stop();
}

/**
 * @brief Read MPU6050 device ID
 * 
 * @return uint8_t Device ID (should be 0x68)
 */
uint8_t MPU6050_GetID(void)
{
    return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

/**
 * @brief Read accelerometer and gyroscope data from MPU6050
 * 
 * @param attitude Pointer to Attitude structure to store sensor data
 */
void MPU6050_GetAttitudeData(Attitude *attitude)
{
    // Buffer for 14 bytes of sensor data
    uint8_t data[14];

    // Read 14 bytes starting from accelerometer X-axis high byte
    MPU6050_ReadRegs(MPU6050_ACCEL_XOUT_H, data, 14);

    // Combine high and low bytes for accelerometer data
    attitude->AccelX = (data[0] << 8) | data[1];
    attitude->AccelY = (data[2] << 8) | data[3];
    attitude->AccelZ = (data[4] << 8) | data[5];

    // Combine high and low bytes for gyroscope data
    attitude->GyroX = (data[8] << 8) | data[9];
    attitude->GyroY = (data[10] << 8) | data[11];
    attitude->GyroZ = (data[12] << 8) | data[13];
}
