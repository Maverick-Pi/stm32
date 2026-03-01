/****************************************************************************/ /**
 * @file   I2C_Hardware.c
 * @brief  I2C Hardware Abstraction Layer Source File
 * 
 * @author Maverick Pi
 * @date   2025-12-10 16:05:16
 ********************************************************************************/

#include "I2C_Hardware.h"

static void I2C_HARDWARE_GPIO_Init(void);
static I2C_Hardware_Status I2C_Hardware_WaitEvent(uint32_t event);
static I2C_Hardware_Status I2C_Hardware_WaitForFlag(uint32_t flag, FlagStatus status);

/**
 * @brief Initialize I2C hardware interface
 * 
 * This function initializes the I2C hardware interface with specified speed.
 * It enables necessary clocks, configures GPIO pins for I2C functionality,
 * and sets up the I2C peripheral with the given communication speed.
 * 
 * @param speed I2C communication speed in Hz (e.g., 100000 for 100kHz)
 */
void I2C_Hardware_Init(uint32_t speed)
{
    RCC_APB1PeriphClockCmd(I2C_HARDWARE_CLOCK, ENABLE);
    RCC_APB2PeriphClockCmd(I2C_HARDWARE_GPIO_CLOCK | I2C_HARDWARE_AFIO_CLOCK, ENABLE);

    I2C_HARDWARE_GPIO_Init();

    I2C_DeInit(I2C_HARDWARE);

    I2C_InitTypeDef I2C_InitStructure;
    I2C_InitStructure.I2C_ClockSpeed = speed;
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_16_9;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C_HARDWARE, &I2C_InitStructure);
}

/**
 * @brief Initialize GPIO pins for I2C communication
 * 
 * This function configures the GPIO pins for I2C communication by enabling
 * pin remapping and setting up the appropriate pin modes and speeds.
 * PB8 is configured as SCL and PB9 as SDA with alternate function open-drain mode.
 */
static void I2C_HARDWARE_GPIO_Init(void)
{
    GPIO_PinRemapConfig(I2C_HARDWARE_REMAP, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = I2C_HARDWARE_SCL_PIN | I2C_HARDWARE_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(I2C_HARDWARE_PORT, &GPIO_InitStructure);
}

/**
 * @brief Deinitialize I2C hardware interface
 * 
 * This function deinitializes the I2C hardware interface by disabling the I2C peripheral,
 * resetting pin remapping, and disabling the associated clocks to conserve power.
 */
void I2C_Hardware_DeInit(void)
{
    I2C_Cmd(I2C_HARDWARE, DISABLE);
    I2C_DeInit(I2C_HARDWARE);
    GPIO_PinRemapConfig(I2C_HARDWARE_REMAP, DISABLE);
    RCC_APB1PeriphClockCmd(I2C_HARDWARE_CLOCK, DISABLE);
    RCC_APB2PeriphClockCmd(I2C_HARDWARE_GPIO_CLOCK | I2C_HARDWARE_AFIO_CLOCK, DISABLE);
}

/**
 * @brief Wait for specific I2C event with timeout
 * 
 * This function waits for a specified I2C event to occur within a timeout period.
 * It continuously checks for the event while monitoring for error conditions.
 * If the event doesn't occur within the timeout, the function returns a timeout error.
 * 
 * @param event I2C event to wait for (e.g., I2C_EVENT_MASTER_MODE_SELECT)
 * @return I2C_Hardware_Status Status of the operation (OK, timeout, or error)
 */
static I2C_Hardware_Status I2C_Hardware_WaitEvent(uint32_t event)
{
    uint32_t timeout = I2C_HARDWARE_TIMEOUT_MAX;

    while (!I2C_CheckEvent(I2C_HARDWARE, event))
    {
        if (--timeout <= 0)
        {
            I2C_GenerateSTOP(I2C_HARDWARE, ENABLE);
            return I2C_HARDWARE_TIMEOUT;
        }

        if (I2C_GetFlagStatus(I2C_HARDWARE, I2C_FLAG_AF)) {
            I2C_ClearFlag(I2C_HARDWARE, I2C_FLAG_AF);
            I2C_GenerateSTOP(I2C_HARDWARE, ENABLE);
            return I2C_HARDWARE_NACK;
        }

        if (I2C_GetFlagStatus(I2C_HARDWARE, I2C_FLAG_BERR)) {
            I2C_ClearFlag(I2C_HARDWARE, I2C_FLAG_BERR);
            I2C_GenerateSTOP(I2C_HARDWARE, ENABLE);
            return I2C_HARDWARE_BUS_ERROR;
        }

        if (I2C_GetFlagStatus(I2C_HARDWARE, I2C_FLAG_OVR)) {
            I2C_ClearFlag(I2C_HARDWARE, I2C_FLAG_OVR);
            I2C_GenerateSTOP(I2C_HARDWARE, ENABLE);
            return I2C_HARDWARE_OVERRUN;
        }
    }

    return I2C_HARDWARE_OK;
}

/**
 * @brief Wait for specific I2C flag to reach desired state
 * 
 * This function waits for a specified I2C flag to reach a desired state (SET or RESET)
 * within a timeout period. It's used for checking busy state or other flag conditions.
 * 
 * @param flag I2C flag to monitor (e.g., I2C_FLAG_BUSY)
 * @param status Desired state of the flag (SET or RESET)
 * @return I2C_Hardware_Status Status of the operation (OK or timeout)
 */
static I2C_Hardware_Status I2C_Hardware_WaitForFlag(uint32_t flag, FlagStatus status)
{
    uint32_t timeout = I2C_HARDWARE_TIMEOUT_MAX;

    while (I2C_GetFlagStatus(I2C_HARDWARE, flag) != status)
    {
        if (--timeout <= 0)
        {
            I2C_GenerateSTOP(I2C_HARDWARE, ENABLE);
            return I2C_HARDWARE_TIMEOUT;
        }
    }

    return I2C_HARDWARE_OK;
}

/**
 * @brief Write a single byte to a specific register of an I2C device
 * 
 * This function writes a single byte of data to a specified register address
 * of an I2C device. It handles the complete I2C communication sequence including
 * start condition, device addressing, register addressing, data transfer, and stop condition.
 * 
 * @param devAddr I2C device address (7-bit, left-aligned)
 * @param regAddr Register address within the device
 * @param data Data byte to write
 * @return I2C_Hardware_Status Status of the write operation
 */
I2C_Hardware_Status I2C_Hardware_WriteByte(uint8_t devAddr, uint8_t regAddr, uint8_t data)
{
    I2C_Hardware_Status status;

    // Wait until I2C is not busy
    status = I2C_Hardware_WaitForFlag(I2C_FLAG_BUSY, RESET);
    if (status != I2C_HARDWARE_OK) return status;

    // Generate START condition
    I2C_GenerateSTART(I2C_HARDWARE, ENABLE);
    status = I2C_Hardware_WaitEvent(I2C_EVENT_MASTER_MODE_SELECT);
    if (status != I2C_HARDWARE_OK) return status;

    // Send device address for write
    I2C_Send7bitAddress(I2C_HARDWARE, devAddr, I2C_Direction_Transmitter);
    status = I2C_Hardware_WaitEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
    if (status != I2C_HARDWARE_OK) return status;

    // Send register address
    I2C_SendData(I2C_HARDWARE, regAddr);
    status = I2C_Hardware_WaitEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED);
    if (status != I2C_HARDWARE_OK) return status;

    // Send data byte
    I2C_SendData(I2C_HARDWARE, data);
    status = I2C_Hardware_WaitEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED);
    if (status != I2C_HARDWARE_OK) return status;

    // Generate STOP condition
    I2C_GenerateSTOP(I2C_HARDWARE, ENABLE);

    return I2C_HARDWARE_OK;
}

/**
 * @brief Read a single byte from a specific register of an I2C device
 * 
 * This function reads a single byte of data from a specified register address
 * of an I2C device. It implements the standard I2C read sequence with restart.
 * 
 * @param devAddr I2C device address (7-bit, left-aligned)
 * @param regAddr Register address within the device
 * @param data Pointer to store the read data byte
 * @return I2C_Hardware_Status Status of the read operation
 */
I2C_Hardware_Status I2C_Hardware_ReadByte(uint8_t devAddr, uint8_t regAddr, uint8_t* data)
{
    I2C_Hardware_Status status;

    // Wait until I2C is not busy
    status = I2C_Hardware_WaitForFlag(I2C_FLAG_BUSY, RESET);
    if (status != I2C_HARDWARE_OK) return status;

    // Generate START condition
    I2C_GenerateSTART(I2C_HARDWARE, ENABLE);
    status = I2C_Hardware_WaitEvent(I2C_EVENT_MASTER_MODE_SELECT);
    if (status != I2C_HARDWARE_OK) return status;

    // Send device address for write
    I2C_Send7bitAddress(I2C_HARDWARE, devAddr, I2C_Direction_Transmitter);
    status = I2C_Hardware_WaitEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
    if (status != I2C_HARDWARE_OK) return status;

    // Send register address
    I2C_SendData(I2C_HARDWARE, regAddr);
    status = I2C_Hardware_WaitEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED);
    if (status != I2C_HARDWARE_OK) return status;

    // Generate RESTART condition
    I2C_GenerateSTART(I2C_HARDWARE, ENABLE);
    status = I2C_Hardware_WaitEvent(I2C_EVENT_MASTER_MODE_SELECT);
    if (status != I2C_HARDWARE_OK) return status;

    // Send device address for read
    I2C_Send7bitAddress(I2C_HARDWARE, devAddr, I2C_Direction_Receiver);
    status = I2C_Hardware_WaitEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
    if (status != I2C_HARDWARE_OK) return status;

    // Disable Acknowledgement for single byte reception
    I2C_AcknowledgeConfig(I2C_HARDWARE, DISABLE);

    // Generate STOP condition
    I2C_GenerateSTOP(I2C_HARDWARE, ENABLE);
    status = I2C_Hardware_WaitEvent(I2C_EVENT_MASTER_BYTE_RECEIVED);
    if (status != I2C_HARDWARE_OK) return status;

    // Read data byte
    *data = I2C_ReceiveData(I2C_HARDWARE);
    // Re-enable Acknowledgement
    I2C_AcknowledgeConfig(I2C_HARDWARE, ENABLE);

    return I2C_HARDWARE_OK;
}

/**
 * @brief Write multiple bytes to a specific register of an I2C device
 * 
 * This function writes multiple bytes of data to a specified register address
 * of an I2C device. It supports sequential writes for block data transfer.
 * 
 * @param devAddr I2C device address (7-bit, left-aligned)
 * @param regAddr Starting register address within the device
 * @param data Pointer to the data buffer to write
 * @param length Number of bytes to write
 * @return I2C_Hardware_Status Status of the write operation
 */
I2C_Hardware_Status I2C_Hardware_WriteBytes(uint8_t devAddr, uint8_t regAddr, uint8_t* data, uint32_t length)
{
    I2C_Hardware_Status status;

    if (length == 0) return I2C_HARDWARE_OK;

    // Wait until I2C is not busy
    status = I2C_Hardware_WaitForFlag(I2C_FLAG_BUSY, RESET);
    if (status != I2C_HARDWARE_OK) return status;

    // Generate START condition
    I2C_GenerateSTART(I2C_HARDWARE, ENABLE);
    status = I2C_Hardware_WaitEvent(I2C_EVENT_MASTER_MODE_SELECT);
    if (status != I2C_HARDWARE_OK) return status;

    // Send device address for write
    I2C_Send7bitAddress(I2C_HARDWARE, devAddr, I2C_Direction_Transmitter);
    status = I2C_Hardware_WaitEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
    if (status != I2C_HARDWARE_OK) return status;

    // Send register address
    I2C_SendData(I2C_HARDWARE, regAddr);
    status = I2C_Hardware_WaitEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED);
    if (status != I2C_HARDWARE_OK) return status;

    // Send data bytes
    for (int i = 0; i < length; ++i) {
        I2C_SendData(I2C_HARDWARE, data[i]);
        status = I2C_Hardware_WaitEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED);
        if (status != I2C_HARDWARE_OK) return status;
    }

    I2C_GenerateSTOP(I2C_HARDWARE, ENABLE);

    return I2C_HARDWARE_OK;
}

/**
 * @brief Read multiple bytes from a specific register of an I2C device
 * 
 * This function reads multiple bytes of data from a specified register address
 * of an I2C device. It handles both single-byte and multi-byte reads with
 * proper ACK/NACK management.
 * 
 * @param devAddr I2C device address (7-bit, left-aligned)
 * @param regAddr Starting register address within the device
 * @param data Pointer to buffer to store read data
 * @param length Number of bytes to read
 * @return I2C_Hardware_Status Status of the read operation
 */
I2C_Hardware_Status I2C_Hardware_ReadBytes(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint16_t length)
{
    I2C_Hardware_Status status;

    if (length == 0) return I2C_HARDWARE_OK;

    // Wait until I2C is not busy
    status = I2C_Hardware_WaitForFlag(I2C_FLAG_BUSY, RESET);
    if (status != I2C_HARDWARE_OK) return status;

    // Generate START condition
    I2C_GenerateSTART(I2C_HARDWARE, ENABLE);
    status = I2C_Hardware_WaitEvent(I2C_EVENT_MASTER_MODE_SELECT);
    if (status != I2C_HARDWARE_OK) return status;

    // Send device address for write
    I2C_Send7bitAddress(I2C_HARDWARE, devAddr, I2C_Direction_Transmitter);
    status = I2C_Hardware_WaitEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
    if (status != I2C_HARDWARE_OK) return status;
    
    // Send register address
    I2C_SendData(I2C_HARDWARE, regAddr);
    status = I2C_Hardware_WaitEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED);
    if (status != I2C_HARDWARE_OK) return status;

    if (length > 1) {
        // Generate RESTART condition
        I2C_GenerateSTART(I2C_HARDWARE, ENABLE);
        status = I2C_Hardware_WaitEvent(I2C_EVENT_MASTER_MODE_SELECT);
        if (status != I2C_HARDWARE_OK) return status;

        // Send device address for read
        I2C_Send7bitAddress(I2C_HARDWARE, devAddr, I2C_Direction_Receiver);
        status = I2C_Hardware_WaitEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);

        // Read data bytes
        for (int i = 0; i < length; ++i) {
            if (i == length -1) {
                // Disable ACK for the lastest byte
                I2C_AcknowledgeConfig(I2C_HARDWARE, DISABLE);
            }

            // Wait data received
            status = I2C_Hardware_WaitEvent(I2C_EVENT_MASTER_BYTE_RECEIVED);
            if (status != I2C_HARDWARE_OK) {
                I2C_AcknowledgeConfig(I2C_HARDWARE, ENABLE);
                return status;
            }
            
            // Read data
            data[i] = I2C_ReceiveData(I2C_HARDWARE);
        }
    } else {
        // Generate RESTART condition for read if only one byte
        I2C_GenerateSTART(I2C_HARDWARE, ENABLE);
        status = I2C_Hardware_WaitEvent(I2C_EVENT_MASTER_MODE_SELECT);
        if (status != I2C_HARDWARE_OK) return status;

        // Send device address for read
        I2C_Send7bitAddress(I2C_HARDWARE, devAddr, I2C_Direction_Receiver);
        status = I2C_Hardware_WaitEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
        if (status != I2C_HARDWARE_OK) return status;

        // Disable ACK for receive the lastest data byte
        I2C_AcknowledgeConfig(I2C_HARDWARE, DISABLE);

        // Waiting for data received
        status = I2C_Hardware_WaitEvent(I2C_EVENT_MASTER_BYTE_RECEIVED);
        if (status != I2C_HARDWARE_OK) {
            I2C_AcknowledgeConfig(I2C_HARDWARE, ENABLE);
            return status;
        }

        // Read data
        data[0] = I2C_ReceiveData(I2C_HARDWARE);
    }

    // Generate STOP condition
    I2C_GenerateSTOP(I2C_HARDWARE, ENABLE);

    // Restart ACK
    I2C_AcknowledgeConfig(I2C_HARDWARE, ENABLE);

    return I2C_HARDWARE_OK;
}

/**
 * @brief Check if an I2C device is ready to communicate
 * 
 * This function checks if an I2C device at the specified address is present
 * and ready to communicate by attempting to address it.
 * 
 * @param devAddr I2C device address (7-bit, left-aligned)
 * @return true Device responded with ACK
 * @return false Device did not respond (NACK or timeout)
 */
bool I2C_Hardware_DeviceReady(uint8_t devAddr)
{
    I2C_Hardware_Status status;

    // Wait until I2C no busy
    if (I2C_Hardware_WaitForFlag(I2C_FLAG_BUSY, RESET) != I2C_HARDWARE_OK) return false;

    I2C_GenerateSTART(I2C_HARDWARE, ENABLE);
    status = I2C_Hardware_WaitEvent(I2C_EVENT_MASTER_MODE_SELECT);
    if (status != I2C_HARDWARE_OK) return false;

    I2C_Send7bitAddress(I2C_HARDWARE, devAddr, I2C_Direction_Transmitter);
    status = I2C_Hardware_WaitEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);

    I2C_GenerateSTOP(I2C_HARDWARE, ENABLE);

    return (status == I2C_HARDWARE_OK);
}

/**
 * @brief Scan I2C bus for connected devices
 * 
 * This function scans all possible I2C addresses (1-127) to detect connected devices.
 * It returns a list of addresses where devices responded with ACK.
 * 
 * @param foundDevices Pointer to array to store found device addresses
 * @param maxDevices Maximum number of devices to store in the array
 * @return I2C_Hardware_Status Status of the scan operation
 */
I2C_Hardware_Status I2C_Hardware_ScanBus(uint8_t *foundDevices, uint8_t maxDevices)
{
    uint8_t addr;
    uint8_t count = 0;

    for (addr = 1; addr < 127; addr++) {
        if (I2C_Hardware_DeviceReady(addr << 1)) {
            if (foundDevices && count < maxDevices) {
                foundDevices[count] = addr;
            }
            count++;
        }
    }

    return I2C_HARDWARE_OK;
}

/**
 * @brief Reset I2C bus to clear error conditions
 * 
 * This function resets the I2C bus by disabling and re-enabling the I2C peripheral
 * and clearing any error flags. It can be used to recover from bus error conditions.
 */
void I2C_Hardware_ResetBus(void)
{
    // Disable I2C
    I2C_Cmd(I2C_HARDWARE, DISABLE);
    // Generate STOP condition
    I2C_GenerateSTOP(I2C_HARDWARE, ENABLE);
    // Clear all error flag
    I2C_ClearFlag(I2C_HARDWARE, I2C_FLAG_AF | I2C_FLAG_BERR | I2C_FLAG_OVR);
    // RE-ENABLE I2C
    I2C_Cmd(I2C_HARDWARE, ENABLE);
}
