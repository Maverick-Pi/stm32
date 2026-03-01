/****************************************************************************/ /**
 * @file   i2c_hardware.h
 * @brief  I2C Hardware Abstraction Layer Header File
 * 
 * @author Maverick Pi
 * @date   2025-12-10 15:45:13
 ********************************************************************************/

#ifndef __I2C_HARDWARE_H__
#define __I2C_HARDWARE_H__

#include "stm32f10x.h"
#include <stdbool.h>

// I2C Hardware Pins defines
#define I2C_HARDWARE_PORT               GPIOB
#define I2C_HARDWARE_SCL_PIN            GPIO_Pin_8
#define I2C_HARDWARE_SDA_PIN            GPIO_Pin_9
#define I2C_HARDWARE_CLOCK              RCC_APB1Periph_I2C1
#define I2C_HARDWARE_GPIO_CLOCK         RCC_APB2Periph_GPIOB
#define I2C_HARDWARE_AFIO_CLOCK         RCC_APB2Periph_AFIO

// I2C Hardware Remap defines
#define I2C_HARDWARE_REMAP              GPIO_Remap_I2C1

// I2C Hardware Instance defines
#define I2C_HARDWARE                    I2C1

// I2C Hardware Speed defines
#define I2C_HARDWARE_SPEED_STRANDARD    100000
#define I2C_HARDWARE_SPEED_FAST         400000

// I2C Hardware Timeout defines
#define I2C_HARDWARE_TIMEOUT_MAX        10000

// I2C Hardware Status defines
typedef enum {
    I2C_HARDWARE_OK = 0,
    I2C_HARDWARE_ERROR = 1,
    I2C_HARDWARE_TIMEOUT = 2,
    I2C_HARDWARE_BUSY = 3,
    I2C_HARDWARE_NACK = 4,
    I2C_HARDWARE_ARBITRATION_LOST = 5,
    I2C_HARDWARE_BUS_ERROR = 6,
    I2C_HARDWARE_OVERRUN = 7
} I2C_Hardware_Status;

// Function declaration
void I2C_Hardware_Init(uint32_t speed);
void I2C_Hardware_DeInit(void);
I2C_Hardware_Status I2C_Hardware_WriteByte(uint8_t devAddr, uint8_t regAddr, uint8_t data);
I2C_Hardware_Status I2C_Hardware_ReadByte(uint8_t devAddr, uint8_t regAddr, uint8_t* data);
I2C_Hardware_Status I2C_Hardware_WriteBytes(uint8_t devAddr, uint8_t regAddr, uint8_t* data, uint32_t length);
I2C_Hardware_Status I2C_Hardware_ReadBytes(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint16_t length);
bool I2C_Hardware_DeviceReady(uint8_t devAddr);
I2C_Hardware_Status I2C_Hardware_ScanBus(uint8_t *foundDevices, uint8_t maxDevices);
void I2C_Hardware_ResetBus(void);

#endif // !__I2C_HARDWARE_H__
