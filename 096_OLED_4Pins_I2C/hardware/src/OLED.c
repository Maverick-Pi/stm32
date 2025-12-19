/****************************************************************************/ /**
 * @file   OLED.c
 * @brief  
 * 
 * @author Maverick Pi
 * @date   2025-12-11 18:46:17
 ********************************************************************************/

#include "OLED.h"

static CH_FontCache_t ch_cache[CH_CACHE_SIZE];
static uint8_t cache_index = 0;

static void OLED_SetCursor(uint8_t col, uint8_t page);
static void OLED_WriteCommand(uint8_t command);
static void OLED_WriteData(uint8_t dat);
static void OLED_CH_Cache_Init(void);
static int16_t OLED_FindInCache(uint16_t unicode);
static void OLED_AddToCache(uint16_t unicode, const uint8_t *fontData);
static int16_t OLED_Find_CH_Index(uint16_t unicode);
static uint8_t* OLED_Get_CH_FontData(uint16_t unicode, uint8_t *buffer);
static uint16_t UTF8_to_Unicode(const char *utf8_str);

void OLED_Init(void)
{
    I2C_Hardware_Init(I2C_HARDWARE_SPEED_FAST);

    Delay_ms(100);

    OLED_WriteCommand(OLED_SSD1306_DISPLAY_OFF);

    OLED_WriteCommand(OLED_SSD1306_CLK_DIV_OSC_FREQ);
    OLED_WriteCommand(OLED_SSD1306_CLK_DIV_OSC_FREQ_RESET);

    OLED_WriteCommand(OLED_SSD1306_MULTIPLEX_RATIO);
    OLED_WriteCommand(OLED_SSD1306_MULTIPLEX_RATIO_RESET);

    OLED_WriteCommand(OLED_SSD1306_DISPLAY_OFFSET);
    OLED_WriteCommand(OLED_SSD1306_DISPLAY_OFFSET_RESET);

    OLED_WriteCommand(OLED_SSD1306_DISPLAY_START_LINE);

    OLED_WriteCommand(OLED_SSD1306_SEGMENT_REMAP);

    OLED_WriteCommand(OLED_SSD1306_COM_OUTPUT_SCAN_DIRECTION);

    OLED_WriteCommand(OLED_SSD1306_COM_PINS_HARDWARE);
    OLED_WriteCommand(OLED_SSD1306_COM_PINS_HARDWARE_RESET);

    OLED_WriteCommand(OLED_SSD1306_CONSTRAST_CONTROL);
    OLED_WriteCommand(OLED_SSD1306_CONSTRAST_CONTROL_VALUE);

    OLED_WriteCommand(OLED_SSD1306_PRECHARGE_PERIOD);
    OLED_WriteCommand(OLED_SSD1306_PRECHARGE_PERIOD_VALUE);

    OLED_WriteCommand(OLED_SSD1306_VCOMH_DESELECT_LEVEL);
    OLED_WriteCommand(OLED_SSD1306_VCOMH_DESELECT_LEVEL_VALUE);

    OLED_WriteCommand(OLED_SSD1306_RAM_CONTENT_DISPLAY);

    OLED_WriteCommand(OLED_SSD1306_NORMAL_DISPLAY);

    OLED_WriteCommand(OLED_SSD1306_CHARGE_PUMP);
    OLED_WriteCommand(OLED_SSD1306_CHARGE_PUMP_ENABLE);

    OLED_WriteCommand(OLED_SSD1306_DISPLAY_ON);

    Delay_ms(100);

    W25Q64_Init();
    OLED_CH_Cache_Init();
}

void OLED_Clear(void)
{
    for (uint8_t j = 0; j < OLED_MAX_PAGE; ++j) {
        OLED_SetCursor(0, j);

        for (uint8_t i = 0; i < OLED_MAX_COLUMN; ++i) {
            OLED_WriteData(0x00);
        }
    }
}

void OLED_ShowChar(uint8_t col, uint8_t page, char c, uint8_t fontSize)
{
    if (fontSize == 6) {
        OLED_SetCursor(col, page);

        for (uint8_t i = 0; i < 6; ++i) {
            OLED_WriteData(OLED_F6x8[c - ' '][i]);
        }
    } else if (fontSize == 8) {
        OLED_SetCursor(col, page);

        for (uint8_t i = 0; i < 8; ++i) {
            OLED_WriteData(OLED_F8x16[c - ' '][i]);
        }

        OLED_SetCursor(col, page + 1);
        for (uint8_t i = 8; i < 16; ++i) {
            OLED_WriteData(OLED_F8x16[c - ' '][i]);
        }
    }
}

void OLED_ShowString(uint8_t col, uint8_t page, char* str, uint8_t fontSize)
{
	for (uint8_t i = 0; str[i] != '\0'; ++i) {
		OLED_ShowChar(col + i * fontSize, page, str[i], fontSize);
	}
}

void OLED_Printf(char *format, ...)
{
	char str[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(str, format, arg);
	va_end(arg);

	OLED_ShowString(0, 4, str, 8);
}

void OLED_ShowImage(uint8_t col, uint8_t page, uint8_t width, uint8_t height, const uint8_t *image)
{
    for (uint8_t j = 0; j < height; ++j) {
        OLED_SetCursor(col, page + j);

		for (uint8_t i = 0; i < width; ++i) {
			OLED_WriteData(image[width * j + i]);
		}
    }
}

void OLED_ShowChineseChar(uint8_t col, uint8_t page, const char *ch)
{
    // 将 UFT-8 转换为 Unicode
    uint16_t unicode = UTF8_to_Unicode(ch);

    if (unicode == 0) {
        // 显示两个问号
        OLED_ShowChar(col, page, '?', FONT_SIZE_8);
        OLED_ShowChar(col + FONT_SIZE_8, page, '?', FONT_SIZE_8);
        return;
    }

    uint8_t fontData[CH_FONT_BYTES_PER_CHAR];
    uint8_t *fontPtr = OLED_Get_CH_FontData(unicode, fontData);

    if (fontPtr == NULL) {
        // 显示两个问号
        OLED_ShowChar(col, page, '?', FONT_SIZE_8);
        OLED_ShowChar(col + FONT_SIZE_8, page, '?', FONT_SIZE_8);
        return;
    }

    // 直接使用 ShowImage 显示中文
    OLED_ShowImage(col, page, CH_FONT_WIDTH, CH_FONT_HEIGHT / 8, fontPtr);
}

void OLED_ShowMixedString(uint8_t col, uint8_t page, const char *str)
{
    uint8_t x = col;
    uint8_t y = page;

    while (*str) {
        // 判断是否为中文（UTF-8 首字节 >= 0xE0）
        if ((uint8_t)*str >= 0xE0) {
            // 中文占 2 字节（GBK 编码）或 3 字节（UTF-8）
            char ch[4] = { str[0], str[1], str[2], '\0' };
            OLED_ShowChineseChar(x, y, ch);
            x += CH_FONT_WIDTH;
            str += 3;   // 跳过 3 个字节
        } else {
            // 英文或数字
            OLED_ShowChar(x, y, *str, FONT_SIZE_8);
            x += FONT_SIZE_8;
            str += 1;   // 跳过 1 个字节
        }

        // 换行处理
        if (x >= OLED_MAX_COLUMN - CH_FONT_WIDTH) {
            x = 0;
            y += 2;     // 中文高度为 2 页
        }

        // 防止超出屏幕
        if (y >= OLED_MAX_PAGE - 1) {
            break;
        }
    }
}

static void OLED_SetCursor(uint8_t col, uint8_t page)
{
    // Set column
    OLED_WriteCommand(0x00 | (col & 0x0F));     // Set lower column
    OLED_WriteCommand(0x10 | ((col & 0xF0) >> 4));  // Set higher column
    // Set page
    OLED_WriteCommand(0xB0 | (page & 0x0F));    // Set page
}

static void OLED_WriteCommand(uint8_t command)
{
    I2C_Hardware_WriteByte(OLED_SSD1306_ADDRESS, OLED_SSD1306_CONTROL_CMD, command);
}

static void OLED_WriteData(uint8_t dat)
{
    I2C_Hardware_WriteByte(OLED_SSD1306_ADDRESS, OLED_SSD1306_CONTROL_DATA, dat);
}

/**
 * @brief 初始化中文字库缓存
 * 
 */
static void OLED_CH_Cache_Init(void)
{
    for (uint8_t i = 0; i < CH_CACHE_SIZE; ++i) {
        ch_cache[i].used = 0;
        ch_cache[i].unicode = 0;
    }

    cache_index = 0;
}

/**
 * @brief 查找中文字符在缓存中的索引
 * 
 * @param unicode Unicode编码 
 * @return int16_t 缓存索引，-1 表示未找到
 */
static int16_t OLED_FindInCache(uint16_t unicode)
{
    for (uint8_t i = 0; i < CH_CACHE_SIZE; ++i) {
        if (ch_cache[i].used && ch_cache[i].unicode == unicode) {
            return i;   // 找到缓存
        }
    }

    return -1;  // 未找到
}

/**
 * @brief 添加字模到缓存
 * 
 * @param unicode Unicode编码 
 * @param fontData 字模数据
 */
static void OLED_AddToCache(uint16_t unicode, const uint8_t *fontData)
{
    // 使用循环替换策略
    ch_cache[cache_index].used = 1;
    ch_cache[cache_index].unicode = unicode;
    memcpy(ch_cache[cache_index].data, fontData, CH_FONT_BYTES_PER_CHAR);

    cache_index = (cache_index + 1) % CH_CACHE_SIZE;
}

/**
 * @brief 查找中文字符索引
 * 
 * @param unicode Unicode编码 
 * @return int16_t 索引偏移量，-1 表示未找到
 */
static int16_t OLED_Find_CH_Index(uint16_t unicode)
{
    // 遍历索引表查找字符
    for (uint16_t i = 0; i < CH_FONT_COUNT; ++i) {
        if (OLED_CH_FontIndex[i].unicode == unicode) {
            return OLED_CH_FontIndex[i].index;
        }
    }

    return -1;  // 未找到
}

/**
 * @brief 获取中文字符的字模数据
 * 
 * @param unicode Unicode编码 
 * @param buffer 缓冲区
 * @return uint8_t* 字模数据指针
 */
static uint8_t* OLED_Get_CH_FontData(uint16_t unicode, uint8_t *buffer)
{
    // 先在缓存中查找
    int16_t cache_idx = OLED_FindInCache(unicode);

    if (cache_idx >= 0) {
        return ch_cache[cache_idx].data; // 直接返回缓存数据
    }

    // 在索引表中查找
    int16_t index = OLED_Find_CH_Index(unicode);

    if (index < 0) {
        return NULL;    // 未找到字符
    }

    // 从 W25Q64 读取字模数据
    uint32_t fontAddr = CH_FONT_BASE_ADDR + index * CH_FONT_BYTES_PER_CHAR;
    W25Q64_ReadData(fontAddr, buffer, CH_FONT_BYTES_PER_CHAR);

    // 添加到缓存
    OLED_AddToCache(unicode, buffer);

    return buffer;
}

/**
 * @brief 将 UTF-8 编码转换为 Unicode 编码
 * 
 * @param utf8_str UTF-8 字符串（3字节）
 * @return uint16_t Unicode编码，0表示转换失败
 */
static uint16_t UTF8_to_Unicode(const char *utf8_str)
{
    if ((uint8_t)utf8_str[0] < 0xE0) {
        return 0; // 不是 3 字节 UTF-8 编码
    }

    // 3 字节 UTF-8 转换为 Unicode
    uint16_t unicode = 0;
    unicode = ((uint8_t)utf8_str[0] & 0x0F) << 12;
    unicode |= ((uint8_t)utf8_str[1] & 0x3F) << 6;
    unicode |= ((uint8_t)utf8_str[2] & 0x3F);

    return unicode;
}
