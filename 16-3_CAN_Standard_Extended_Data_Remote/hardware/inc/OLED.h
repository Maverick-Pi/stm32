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
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "OLED_Font.h"
#include "I2C_Hardware.h"
#include "Delay.h"
#include "W25Q64.h"
#include "CH_Font_Index.h"


/* OLED 内置 SSD1306 芯片命令 */
#define OLED_SSD1306_ADDRESS                        0x78
#define OLED_SSD1306_CONTROL_CMD                    0x00
#define OLED_SSD1306_CONTROL_DATA                   0x40
#define OLED_SSD1306_CONTINUATION_CMD               0x80
#define OLED_SSD1306_CONTINUATION_DATA              0xC0
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


/* 0.96寸 OLED 显示屏分辨率 */
#define OLED_MAX_COLUMN         128
#define OLED_MAX_PAGE           8

/* ASCII 字符大小 (以宽为基准) */
#define FONT_SIZE_6             6
#define FONT_SIZE_8             8


/*********************************** 函数声明 ***********************************/

/* 初始化函数 */
void OLED_Init(void);

/* 显存控制函数 */
void OLED_Clear(void);
void OLED_ClearArea(int16_t col, int16_t row, uint8_t width, uint8_t height);
void OLED_Reverse(void);
void OLED_ReverseArea(int16_t col, int16_t row, uint8_t width, uint8_t height);

/* 更新函数 */
void OLED_Update(void);

/* 显示函数 */
void OLED_ShowChar(int16_t col, int16_t row, char c, uint8_t fontSize);
void OLED_ShowChineseChar(int16_t col, int16_t row, const char *ch);
void OLED_ShowString(int16_t col, int16_t row, uint8_t fontSize, const char* str);
void OLED_ShowNum(int16_t col, int16_t row, uint32_t num, uint8_t fontSize);
void OLED_ShowSignedNum(int16_t col, int16_t row, int32_t num, uint8_t fontSize);
void OLED_ShowHexNum(int16_t col, int16_t row, uint32_t num, uint8_t fontSize);
void OLED_ShowBinNum(int16_t col, int16_t row, uint32_t num, uint8_t len, uint8_t fontSize);
void OLED_ShowFloatNum(int16_t col, int16_t row, double num, uint8_t fracLen, uint8_t fontSize);
void OLED_ShowImage(int16_t col, int16_t row, uint8_t width, uint8_t height, const uint8_t *image, bool clear);
void OLED_Printf(int16_t col, int16_t row, uint8_t fontSize, char *format, ...);

/* 绘图函数 */
void OLED_DrawPoint(int16_t x, int16_t y);
bool OLED_GetPoint(int16_t x, int16_t y);
void OLED_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
void OLED_DrawRectangle(int16_t x, int16_t y, uint8_t width, uint8_t height, bool filled);
void OLED_DrawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, bool filled);
void OLED_DrawCircle(int16_t cx, int16_t cy, uint8_t r, bool filled);
void OLED_DrawEllipse(int16_t x, int16_t y, uint8_t a, uint8_t b, bool filled);
void OLED_DrawArc(int16_t x, int16_t y, uint8_t radius, int16_t startAngle, int16_t endAngle, bool filled);

#endif // !__OLED_H__
