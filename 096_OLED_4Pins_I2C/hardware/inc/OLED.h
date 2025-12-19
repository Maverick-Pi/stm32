/****************************************************************************/ /**
 * @file   OLED.h
 * @brief  
 * 
 * @author Maverick Pi
 * @date   2025-12-11 18:46:03
 ********************************************************************************/

#ifndef __OLED_H__
#define __OLED_H__

#include "stm32f10x.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "OLED_Font.h"
#include "I2C_Hardware.h"
#include "Delay.h"
#include "W25Q64.h"
#include "CH_Font_Index.h"

// 0.96寸 OLED 显示屏分辨率
#define OLED_MAX_COLUMN         128
#define OLED_MAX_PAGE           8

// OLED 内置 SSD1306 芯片命令
#define OLED_SSD1306_ADDRESS                        0x78
#define OLED_SSD1306_CONTROL_CMD                    0x00
#define OLED_SSD1306_CONTROL_DATA                   0x40
#define OLED_SSD1306_CONTINUATION_CMD               0x80
#define OLED_SSD1306_CONTINUATION_DATA              0xC0

// Command
#define OLED_SSD1306_DISPLAY_OFF                    0xAE
#define OLED_SSD1306_DISPLAY_ON                     0xAF
#define OLED_SSD1306_CLK_DIV_OSC_FREQ               0xD5
#define OLED_SSD1306_CLK_DIV_OSC_FREQ_RESET         0x80
#define OLED_SSD1306_MULTIPLEX_RATIO                0xA8
#define OLED_SSD1306_MULTIPLEX_RATIO_RESET          0x3F
#define OLED_SSD1306_DISPLAY_OFFSET                 0xD3
#define OLED_SSD1306_DISPLAY_OFFSET_RESET           0x00
#define OLED_SSD1306_DISPLAY_START_LINE             0x40
#define OLED_SSD1306_SEGMENT_REMAP                  0xA1
#define OLED_SSD1306_COM_OUTPUT_SCAN_DIRECTION      0xC8
#define OLED_SSD1306_COM_PINS_HARDWARE              0xDA
#define OLED_SSD1306_COM_PINS_HARDWARE_RESET        0x12
#define OLED_SSD1306_CONSTRAST_CONTROL              0x81
#define OLED_SSD1306_CONSTRAST_CONTROL_VALUE        0xCF
#define OLED_SSD1306_PRECHARGE_PERIOD               0xD9
#define OLED_SSD1306_PRECHARGE_PERIOD_VALUE         0xF1
#define OLED_SSD1306_VCOMH_DESELECT_LEVEL           0xDB
#define OLED_SSD1306_VCOMH_DESELECT_LEVEL_VALUE     0x30
#define OLED_SSD1306_ENTIRE_DISPLAY_ON              0xA5
#define OLED_SSD1306_RAM_CONTENT_DISPLAY            0xA4
#define OLED_SSD1306_NORMAL_DISPLAY                 0xA6
#define OLED_SSD1306_INVERSE_DISPLAY                0xA7
#define OLED_SSD1306_CHARGE_PUMP                    0x8D
#define OLED_SSD1306_CHARGE_PUMP_ENABLE             0x14
#define OLED_SSD1306_MEMORY_ADDR_MODE               0x20
#define OLED_SSD1306_MEMORY_ADDR_MODE_HORIZONTAL    0x00
#define OLED_SSD1306_MEMORY_ADDR_MODE_VERTICAL      0x01
#define OLED_SSD1306_MEMORY_ADDR_MODE_PAGE          0x02
#define OLED_SSD1306_COLUMN_ADDR                    0x21
#define OLED_SSD1306_PAGE_ADDR                      0x22

// ASCII 字符大小 (以宽为基准)
#define FONT_SIZE_6                     6
#define FONT_SIZE_8                     8


void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t col, uint8_t page, char c, uint8_t fontSize);
void OLED_ShowString(uint8_t col, uint8_t page, char* str, uint8_t fontSize);
void OLED_Printf(char *format, ...);
void OLED_ShowImage(uint8_t col, uint8_t page, uint8_t width, uint8_t height, const uint8_t *image);
void OLED_ShowMixedString(uint8_t col, uint8_t page, const char *str);

#endif // !__OLED_H__
