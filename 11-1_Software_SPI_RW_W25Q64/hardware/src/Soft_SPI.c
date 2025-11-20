/****************************************************************************/ /**
 * @file   Soft_SPI.c
 * @brief  Software-based SPI communication driver implementation
 * 
 * @author Maverick Pi
 * @date   2025-11-20 15:55:12
 ********************************************************************************/

#include "Soft_SPI.h"

/**
 * @brief Control Chip Select (CS) line
 * 
 * @param bitValue 0 for active low, 1 for inactive high
 */
void Soft_SPI_CS(uint8_t bitValue)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)bitValue);
}

/**
 * @brief Control Serial Clock (SCK) line
 * 
 * @param bitValue Clock state (0 or 1)
 */
void Soft_SPI_SCK(uint8_t bitValue)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction)bitValue);
}

/**
 * @brief Control Master Out Slave In (MOSI) line
 * 
 * @param bitValue Data bit to transmit (0 or 1)
 */
void Soft_SPI_MOSI(uint8_t bitValue)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_7, (BitAction)bitValue);
}

/**
 * @brief Read Master In Slave Out (MISO) line
 * 
 * @return uint8_t Received data bit (0 or 1)
 */
uint8_t Soft_SPI_MISO(void)
{
    return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6);
}

/**
 * @brief Initialize Software SPI GPIO pins and default states
 */
void Soft_SPI_Init(void)
{
    // Enable GPIOA clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    // Configure CS, SCK, MOSI as output push-pull
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Configure MISO as input pull-up
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Set default states: CS high (inactive), SCK low (idle)
    Soft_SPI_CS(1);
    Soft_SPI_SCK(0);
}

/**
 * @brief Start SPI transaction (activate CS line)
 */
void Soft_SPI_Start(void)
{
    Soft_SPI_CS(0);
}

/**
 * @brief Stop SPI transaction (deactivate CS line)
 */
void Soft_SPI_Stop(void)
{
    Soft_SPI_CS(1);
}

/**
 * @brief Transfer a single byte over SPI
 * 
 * @param data Byte to transmit
 * @return uint8_t Byte received during transfer
 */
uint8_t Soft_SPI_TransferByte(uint8_t data)
{
    // Transfer 8 bits MSB first
    for (int i = 0; i < 8; i++) {
        // Output MSB on MOSI
        Soft_SPI_MOSI(data & 0x80);
        data <<= 1;
        // Clock rising edge - slave samples data
        Soft_SPI_SCK(1);
        // Read MISO on clock high
        if (Soft_SPI_MISO()) {
            data |= 0x01;   // Set LSB if MISO is high
        }
        // Clock falling edge
        Soft_SPI_SCK(0);
    }

    return data;
}
