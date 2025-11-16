/****************************************************************************/ /**
 * @file   MyI2C.c
 * @brief  Software I2C Implementation
 * 
 * @author Maverick Pi
 * @date   2025-11-14 16:16:09
 ********************************************************************************/

#include "MyI2C.h"

/**
 * @brief Write value to SCL (Serial Clock Line)
 * 
 * @param bitValue Bit value to write (0 or 1)
 */
void MyI2C_W_SCL(uint8_t bitValue)
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)bitValue);
    Delay_us(10);
}

/**
 * @brief Write value to SDA (Serial Data Line)
 * 
 * @param bitValue Bit value to write (0 or 1)
 */
void MyI2C_W_SDA(uint8_t bitValue)
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)bitValue);
    Delay_us(10);
}

/**
 * @brief Read value from SDA (Serial Data Line)
 * 
 * @return uint8_t Bit value read from SDA (0 or 1)
 */
uint8_t MyI2C_R_SDA(void)
{
    uint8_t bitValue;
    bitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
    Delay_us(10);
    return bitValue;
}

/**
 * @brief Initialize I2C GPIO pins
 * 
 */
void MyI2C_Init(void)
{
    // Enable GPIOB clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // Configure GPIO pins for I2C
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;    // Open-drain output
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;    // SCL and SDA pins
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // Set SCL and SDA to high (idle state)
    GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);
}

/**
 * @brief Generate I2C start condition
 * 
 */
void MyI2C_Start(void)
{
    MyI2C_W_SDA(1);     // SDA high
    MyI2C_W_SCL(1);     // SCL high
    MyI2C_W_SDA(0);     // SDA low while SCL high (start condition)
    MyI2C_W_SCL(0);     // SCL low
}

/**
 * @brief Generate I2C stop condition
 * 
 */
void MyI2C_Stop(void)
{
    MyI2C_W_SDA(0);     // SDA low
    MyI2C_W_SCL(1);     // SCL high
    MyI2C_W_SDA(1);     // SDA high while SCL high (stop condition)
}

/**
 * @brief Send one byte over I2C
 * 
 * @param byte Byte to send
 */
void MyI2C_SendByte(uint8_t byte)
{
    // Send each bit from MSB to LSB
    for (int i = 0; i < 8; ++i) {
        MyI2C_W_SDA(!!(byte & (0x80 >> i)));    // Extract and send current bit
        MyI2C_W_SCL(1);     // Clock high - data is sampled
        MyI2C_W_SCL(0);    // Clock low - prepare for next bit 
    }
}

/**
 * @brief Receive one byte over I2C
 * 
 * @return uint8_t Received byte
 */
uint8_t MyI2C_ReceiveByte(void)
{
    uint8_t byte = 0x00;

    // Receive each bit from MSB to LSB
    for (int i = 7; i >= 0; --i) {
        MyI2C_W_SDA(1);     // Release SDA for input
        MyI2C_W_SCL(1);     // Clock high - read data bit
        byte |= MyI2C_R_SDA() << i;     // Read bit and store in byte
        MyI2C_W_SCL(0);     // Clock low - prepare for next bit
    }
    return byte;
}

/**
 * @brief Send ACK or NACK bit
 * 
 * @param ack 0 for ACK, 1 for NACK
 */
void MyI2C_SendAck(uint8_t ack)
{
    MyI2C_W_SDA(ack);   // Set SDA to ACK/NACK value
    MyI2C_W_SCL(1);     // Clock high - ACK/NACK is sampled
    MyI2C_W_SCL(0);     // Clock low
}

/**
 * @brief Receive ACK bit from slave
 * 
 * @return uint8_t 0 for ACK, 1 for NACK
 */
uint8_t MyI2C_ReceiveAck(void)
{
    uint8_t ack;
    MyI2C_W_SDA(1);     // Release SDA for input
    MyI2C_W_SCL(1);     // Clock high - read ACK bit
    ack = MyI2C_R_SDA();    // Read ACK/NACK from slave
    MyI2C_W_SCL(0);     // Clock low
    return ack;
}
