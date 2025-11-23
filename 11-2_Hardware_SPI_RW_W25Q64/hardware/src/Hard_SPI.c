/****************************************************************************/ /**
 * @file   Hard_SPI.c
 * @brief  Hardware SPI driver for STM32F10x - Source file
 * 
 * @author Maverick Pi
 * @date   2025-11-23 15:37:04
 ********************************************************************************/

#include "Hard_SPI.h"

/**
 * @brief Initialize SPI1 peripheral with GPIO configuration
 * 
 * Configures:
 * - GPIO clocks and SPI1 clock
 * - SCK and MOSI as alternate function push-pull
 * - CS as output push-pull
 * - MISO as input pull-up
 * - SPI1 in master mode, full duplex, 8-bit data
 * - CPOL=0, CPHA=1 edge, MSB first
 * - Baud rate prescaler 16
 * - Software NSS management
 */
void Hard_SPI_Init(void)
{
    /* Enable peripheral clocks */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    /* Configure SCK and MOSI pins as alternate function push-pull */
    GPIO_Init(GPIOA, &(GPIO_InitTypeDef) {
        .GPIO_Pin = SPI1_SCK_PIN | SPI1_MOSI_PIN,
        .GPIO_Mode = GPIO_Mode_AF_PP,
        .GPIO_Speed = GPIO_Speed_50MHz
    });

    /* Configure CS pin as output push-pull */
    GPIO_Init(GPIOA, &(GPIO_InitTypeDef) {
        .GPIO_Pin = SPI1_CS_PIN,
        .GPIO_Mode = GPIO_Mode_Out_PP,
        .GPIO_Speed = GPIO_Speed_50MHz
    });

    /* Configure MISO pin as input pull-up */
    GPIO_Init(GPIOA, &(GPIO_InitTypeDef) {
        .GPIO_Pin = SPI1_MISO_PIN,
        .GPIO_Mode = GPIO_Mode_IPU,
        .GPIO_Speed = GPIO_Speed_50MHz
    });

    /* Configure SPI1 peripheral */
    SPI_Init(SPI1, &(SPI_InitTypeDef) {
        .SPI_Direction = SPI_Direction_2Lines_FullDuplex,
        .SPI_Mode = SPI_Mode_Master,
        .SPI_DataSize = SPI_DataSize_8b,
        .SPI_CPOL = SPI_CPOL_Low,
        .SPI_CPHA = SPI_CPHA_1Edge,
        .SPI_NSS = SPI_NSS_Soft,
        .SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16,
        .SPI_FirstBit = SPI_FirstBit_MSB,
        .SPI_CRCPolynomial = 7
    });

    /* Enable SPI1 peripheral */
    SPI_Cmd(SPI1, ENABLE);

    /* Set CS high to deselect SPI device (W25Q64) */
    GPIO_SetBits(GPIOA, SPI1_CS_PIN); // Deselect W25Q64
}

/**
 * @brief Start SPI transaction by activating chip select
 * Sets CS pin low to select the SPI slave device
 */
void Hard_SPI_Start(void)
{
    GPIO_ResetBits(GPIOA, SPI1_CS_PIN); // Select W25Q64
}

/**
 * @brief End SPI transaction by deactivating chip select
 * Sets CS pin high to deselect the SPI slave device
 */
void Hard_SPI_Stop(void)
{
    GPIO_SetBits(GPIOA, SPI1_CS_PIN); // Deselect W25Q64
}

/**
 * @brief Transfer a single byte over SPI (full duplex)
 * @param data: Byte to transmit over MOSI
 * @return Byte received from MISO during transmission
 * 
 * This function handles the complete SPI transfer sequence:
 * 1. Wait until transmit buffer is empty
 * 2. Write data to transmit register
 * 3. Wait until receive buffer is not empty
 * 4. Read and return received data
 */
uint8_t Hard_SPI_TransferByte(uint8_t data)
{
    /* Wait until transmit buffer is empty */
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    /* Send data byte */
    SPI_I2S_SendData(SPI1, data);
    /* Wait until receive buffer is not empty */
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

    return SPI_I2S_ReceiveData(SPI1);
}
