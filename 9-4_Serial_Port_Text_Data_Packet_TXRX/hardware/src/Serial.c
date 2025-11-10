/****************************************************************************/ /**
 * @file   Serial.c
 * @brief  USART serial communication implementation for STM32
 * 
 * @author Maverick Pi
 * @date   2025-10-13 14:55:55
 ********************************************************************************/

#include "Serial.h"

char Serial_RxPacket[SERIAL_PACKET_SIZE];
uint8_t Serial_RxFlag = 0;  // Flag indicating new data received

/**
 * @brief Initialize USART1 for serial communication
 * 
 * Configures GPIO pins and USART1 peripheral for TX and RX operation
 * at 9600 baud rate, 8 data bits, 1 stop bit, no parity
 * Enables USART1 reception interrupt
 */
void Serial_Init(void)
{
    // Enable clock for USART1 and GPIOA peripherals
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // Configure GPIO Pin A9 as alternate function push-pull (USART1_TX)
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // Configure GPIO Pin A10 as input pull up
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Configure USART1 parameters
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART1, &USART_InitStructure);

    // Configure USART1 Reception Interrupt
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    // Configure NVIC for USART1 interrupt
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);

    // Enable USART1 peripheral
    USART_Cmd(USART1, ENABLE);
}

/**
 * @brief Send a single byte via USART
 * 
 * @param b Byte to be transmitted
 */
void Serial_SendByte(uint8_t b)
{
    // Load data into transmit data register
    USART_SendData(USART1, b);

    // Wait until transmit data register is empty
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

/**
 * @brief Send an array of 16-bit values via USART
 * 
 * @param arr Pointer to the array of 8-bit values
 * @param len Number of elements in the array
 */
void Serial_SendArray(uint8_t *arr, uint16_t len)
{
    // Iterate through array and send each element
    for (uint16_t i = 0; i < len; ++i) {
        Serial_SendByte(arr[i]);
    }
}

/**
 * @brief Send a null-terminated string via USART
 * 
 * @param str Pointer to the null-terminated string
 */
void Serial_SendString(char *str)
{
    // Send each character until null terminator is reached
    for (uint16_t i = 0; str[i] != '\0'; ++i) {
        Serial_SendByte(str[i]);
    }
}

/**
 * @brief Calculate x raised to the power of y
 * 
 * @param x Base value
 * @param y Exponent value
 * @return uint32_t Result of x^y calculation
 */
uint32_t Serial_Pow(uint32_t x, uint32_t y)
{
    uint32_t result = 1;
    // Multiply result by x for y times
    while (y--) {
        result *= x;
    }
    return result;
}

/**
 * @brief Send a numeric value as ASCII digits via USART
 * 
 * @param num Numeric value to be sent
 * @param len Number of digits to display (with leading zeros if needed)
 */
void Serial_SendNumber(uint32_t num, uint8_t len)
{
    // Extract and send each digit from left to right
    for (uint8_t i = 0; i < len; ++i) {
        Serial_SendByte(num / Serial_Pow(10, len - i - 1) % 10 + '0');
    }
}

/**
 * @brief Redirect standard library's fputc to USART
 * 
 * @param ch Character to be sent
 * @param f File pointer (unused in this implementation)
 * @return int The character that was sent
 */
int fputc(int ch, FILE *f)
{
    Serial_SendByte(ch);
    return ch;
}

/**
 * @brief Custom printf implementation using variable arguments
 * 
 * @param format Format string (same as standard printf)
 * @param ... Variable arguments to be formatted
 */
void Serial_Printf(char *format, ...)
{
    char str[100];
    va_list arg;    // Variable argument list
    // Initialize argument list and format string
    va_start(arg, format);
    vsprintf(str, format, arg);
    va_end(arg);

    // Send the formatted string
    Serial_SendString(str);
}

/**
 * @brief Check if new data has been received
 * 
 * @return uint8_t 1 if new data available, 0 otherwise
 */
uint8_t Serial_GetRxFlag(void)
{
    if (Serial_RxFlag == 1) {
        Serial_RxFlag = 0;  // Clear flag after reading
        return 1;
    }
    return 0;
}

/**
 * @brief Send a data packet with predefined structure
 * 
 * Packet structure: 0xFF header + num data bytes + 0xEF footer
 * @param txPacket Pointer to the num-byte data array to be sent
 * @param num The numbers of data
 */
void Serial_SendPacket(uint8_t *txPacket, uint16_t num)
{
    Serial_SendByte(0xFF);
    Serial_SendArray(txPacket, 4);
    Serial_SendByte(0xEF);
}

/**
 * @brief USART1 Interrupt Service Routine
 * 
 * Handles USART1 receive interrupt, implements packet-based communication protocol
 * Packet format: 0xFF (header) + data + 0xEF (footer)
 * Sets receive flag when complete packet is received
 */
void USART1_IRQHandler(void)
{
    static uint8_t RxStatus = 0;    // Current state in packet reception state machine
    static uint8_t p_RxPacket = 0;  // Current position in receive packet buffer

    // Check if receive interrupt occurred
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) {
        uint8_t RxData = USART_ReceiveData(USART1);

        switch (RxStatus) {
            case 0:     // Waiting for packet header
                if (RxData == '@') {
                    RxStatus   = 1;
                    p_RxPacket = 0;
                }
                break;
            case 1:     // Receiving packet data
                if (RxData == '\r') {
                    RxStatus = 2;
                } else {
                    Serial_RxPacket[p_RxPacket++] = RxData;
                }
                break;
            case 2:     // Waiting for packet end
                if (RxData == '\n') {
                    Serial_RxPacket[p_RxPacket] = '\0';
                    RxStatus = 0;
                    Serial_RxFlag = 1;
                }
                break;
        }

        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}
