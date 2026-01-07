/****************************************************************************/ /**
 * @file   OLED.c
 * @brief  SSD1306 OLED显示驱动库，支持中英文显示、图形绘制、缓存管理
 * 
 * 本模块提供基于SSD1306 OLED的完整驱动功能，包括：
 * 1. 基本显示控制（初始化、清屏、刷新）
 * 2. 字符显示（ASCII、中文UTF-8）
 * 3. 数字格式化显示（整数、浮点数、十六进制、二进制）
 * 4. 图形绘制（点、线、矩形、圆形、椭圆、圆弧）
 * 5. 显示缓存管理
 * 6. 中文字库外部存储(W25Q64)与缓存管理
 * 
 * @note 显示分辨率为128x64像素，采用8页(Page)×128列(Column)结构
 *       每页包含8行像素，通过页寻址模式进行数据写入
 * 
 * @author Maverick Pi
 * @date   2025-12-11 18:46:17
 *******************************************************************************/

#include "OLED.h"

/**************************** 全局变量 ****************************/
static uint8_t OLED_BUFFER[8][128];     // 显示缓存数组 [页索引][列地址]
                                        // 每页对应8行像素，每列8位表示垂直方向8个像素

static CH_FontCache_t ch_cache[CH_CACHE_SIZE];  // 中文字符缓存，采用循环替换策略
static uint8_t cache_index = 0;         // 缓存当前写入位置索引

/**************************** 静态工具函数声明 ****************************/

/* 硬件接口函数 */
static void OLED_WriteCommands(uint8_t *commands, uint8_t len);  // 批量写入命令
static void OLED_WriteData(uint8_t *dat, uint8_t len);           // 批量写入显示数据

/* 显示控制函数 */
static void OLED_SetCursor(uint8_t col, uint8_t page);           // 设置显示起始位置

/* 中文字符处理函数 */
static void OLED_CH_Cache_Init(void);                            // 初始化中文字库缓存
static int16_t OLED_FindInCache(uint16_t unicode);               // 在缓存中查找字符
static void OLED_AddToCache(uint16_t unicode, const uint8_t *fontData); // 添加字符到缓存
static int16_t OLED_Find_CH_Index(uint16_t unicode);             // 在字库索引表中查找
static uint8_t* OLED_Get_CH_FontData(uint16_t unicode, uint8_t *buffer); // 获取字模数据
static uint16_t UTF8_to_Unicode(const char *utf8_str);           // UTF-8转Unicode

/* 图形绘制辅助函数 */
static void OLED_DrawLineH(int16_t x0, int16_t y0, int16_t x1, int16_t y1); // 绘制水平倾向直线
static void OLED_DrawLineV(int16_t x0, int16_t y0, int16_t x1, int16_t y1); // 绘制垂直倾向直线


/*******************************************************************************
 * @brief  批量写入命令到SSD1306
 * 
 * @param  commands 命令数组指针
 * @param  len      命令数量
 * 
 * @note   通过I2C接口发送控制字节(0x00)后发送命令序列
 ******************************************************************************/
static void OLED_WriteCommands(uint8_t *commands, uint8_t len)
{
    // 发送控制字节(0x00表示命令模式)，后跟命令序列
    I2C_Hardware_WriteBytes(OLED_SSD1306_ADDRESS, OLED_SSD1306_CONTROL_CMD, commands, len);
}

/*******************************************************************************
 * @brief  批量写入显示数据到SSD1306
 * 
 * @param  dat 显示数据数组指针
 * @param  len 数据长度
 * 
 * @note   通过I2C接口发送控制字节(0x40)后发送显示数据
 ******************************************************************************/
static void OLED_WriteData(uint8_t *dat, uint8_t len)
{
    // 发送控制字节(0x40表示数据模式)，后跟显示数据
    I2C_Hardware_WriteBytes(OLED_SSD1306_ADDRESS, OLED_SSD1306_CONTROL_DATA, dat, len);
}

/*******************************************************************************
 * @brief  设置OLED显示起始位置（光标）
 * 
 * @param  col  列地址(0-127)
 * @param  page 页地址(0-7)
 * 
 * @note   发送三个命令：低4位列地址、高4位列地址、页地址
 *         页地址模式中，每页对应8行像素，垂直方向通过页内偏移定位
 ******************************************************************************/
static void OLED_SetCursor(uint8_t col, uint8_t page)
{
    uint8_t commands[3] = { 
        0x00 | (col & 0x0F),              // 设置列地址低4位 (0x00-0x0F)
        0x10 | ((col & 0xF0) >> 4),       // 设置列地址高4位 (0x10-0x1F)
        0xB0 | (page & 0x0F)              // 设置页地址 (0xB0-0xB7)
    };
    OLED_WriteCommands(commands, 3);     
}

/*******************************************************************************
 * @brief  初始化中文字库缓存
 * 
 * @note   将缓存数组所有条目标记为未使用，重置缓存索引
 *         采用循环替换策略管理有限缓存空间
 ******************************************************************************/
static void OLED_CH_Cache_Init(void)
{
    for (uint8_t i = 0; i < CH_CACHE_SIZE; ++i) {
        ch_cache[i].used = 0;     // 标记为未使用
        ch_cache[i].unicode = 0;  // 清除Unicode编码
    }

    cache_index = 0;  // 重置缓存索引
}

/*******************************************************************************
 * @brief  在缓存中查找指定Unicode字符
 * 
 * @param  unicode 目标字符的Unicode编码
 * @return int16_t 找到返回缓存索引(0-255)，未找到返回-1
 ******************************************************************************/
static int16_t OLED_FindInCache(uint16_t unicode)
{
    // 线性遍历缓存数组
    for (uint8_t i = 0; i < CH_CACHE_SIZE; ++i) {
        if (ch_cache[i].used && ch_cache[i].unicode == unicode) {
            return i;   // 找到缓存，返回索引
        }
    }

    return -1;  // 未找到目标字符
}

/*******************************************************************************
 * @brief  添加字符字模数据到缓存
 * 
 * @param  unicode  字符的Unicode编码
 * @param  fontData 字模数据指针
 * 
 * @note   使用循环替换策略：当缓存满时，新字符覆盖最旧的字符
 *         每次添加后更新缓存索引
 ******************************************************************************/
static void OLED_AddToCache(uint16_t unicode, const uint8_t *fontData)
{
    // 使用循环替换策略
    ch_cache[cache_index].used = 1;  // 标记为已使用
    ch_cache[cache_index].unicode = unicode;  // 存储Unicode编码
    // 复制字模数据到缓存
    memcpy(ch_cache[cache_index].data, fontData, CH_FONT_BYTES_PER_CHAR);

    // 更新缓存索引，循环覆盖
    cache_index = (cache_index + 1) % CH_CACHE_SIZE;
}

/*******************************************************************************
 * @brief  在外部字库索引表中查找字符
 * 
 * @param  unicode 目标字符的Unicode编码
 * @return int16_t 找到返回字库中的索引偏移量，未找到返回-1
 * 
 * @note   索引表需预先定义在OLED_CH_FontIndex数组中
 *         每个条目包含Unicode编码和在字库中的偏移量
 ******************************************************************************/
static int16_t OLED_Find_CH_Index(uint16_t unicode)
{
    // 线性遍历索引表查找字符
    for (uint16_t i = 0; i < CH_FONT_COUNT; ++i) {
        if (OLED_CH_FontIndex[i].unicode == unicode) {
            return OLED_CH_FontIndex[i].index;  // 返回字库中的偏移量
        }
    }

    return -1;  // 未找到字符
}

/*******************************************************************************
 * @brief  获取中文字符的字模数据
 * 
 * @param  unicode 字符的Unicode编码
 * @param  buffer  临时缓冲区，用于从外部存储读取数据
 * @return uint8_t* 字模数据指针，失败返回NULL
 * 
 * @note   优先从缓存查找，缓存未命中则从外部Flash(W25Q64)读取
 *         读取后会添加到缓存以便下次快速访问
 ******************************************************************************/
static uint8_t* OLED_Get_CH_FontData(uint16_t unicode, uint8_t *buffer)
{
    // 1. 先在缓存中查找
    int16_t cache_idx = OLED_FindInCache(unicode);

    if (cache_idx >= 0) {
        return ch_cache[cache_idx].data; // 缓存命中，直接返回
    }

    // 2. 缓存未命中，在索引表中查找
    int16_t index = OLED_Find_CH_Index(unicode);

    if (index < 0) {
        return NULL;    // 字库中不存在该字符
    }

    // 3. 从W25Q64 Flash读取字模数据
    uint32_t fontAddr = CH_FONT_BASE_ADDR + index * CH_FONT_BYTES_PER_CHAR;
    W25Q64_ReadData(fontAddr, buffer, CH_FONT_BYTES_PER_CHAR);

    // 4. 添加到缓存
    OLED_AddToCache(unicode, buffer);

    return buffer;
}

/*******************************************************************************
 * @brief  将3字节UTF-8编码转换为16位Unicode编码
 * 
 * @param  utf8_str UTF-8字符串指针（需包含完整3字节）
 * @return uint16_t Unicode编码，转换失败返回0
 * 
 * @note   仅支持3字节UTF-8编码（对应U+0800-U+FFFF范围）
 *         UTF-8编码格式：1110xxxx 10xxxxxx 10xxxxxx
 ******************************************************************************/
static uint16_t UTF8_to_Unicode(const char *utf8_str)
{
    // 检查首字节是否大于等于0xE0（3字节UTF-8起始标志）
    if ((uint8_t)utf8_str[0] < 0xE0) {
        return 0; // 不是3字节UTF-8编码
    }

    // 3字节UTF-8到Unicode转换
    uint16_t unicode = 0;
    unicode = ((uint8_t)utf8_str[0] & 0x0F) << 12;  // 第一个字节的低4位
    unicode |= ((uint8_t)utf8_str[1] & 0x3F) << 6;  // 第二个字节的低6位
    unicode |= ((uint8_t)utf8_str[2] & 0x3F);       // 第三个字节的低6位

    return unicode;
}

/*******************************************************************************
 * @brief  OLED初始化函数
 * 
 * @note   初始化流程：
 *         1. 初始化I2C硬件接口
 *         2. 发送SSD1306初始化命令序列
 *         3. 初始化外部字库Flash
 *         4. 初始化中文字符缓存
 ******************************************************************************/
void OLED_Init(void)
{
    // 1. 初始化I2C硬件（快速模式）
    I2C_Hardware_Init(I2C_HARDWARE_SPEED_FAST);

    // 等待OLED上电稳定
    Delay_ms(100);

    // 2. SSD1306初始化命令序列
    uint8_t commands[] = {
        OLED_SSD1306_DISPLAY_OFF,           // 关闭显示
        OLED_SSD1306_CLK_DIV_OSC_FREQ,      // 设置时钟分频和振荡频率
        OLED_SSD1306_CLK_DIV_OSC_FREQ_RESET,
        OLED_SSD1306_MULTIPLEX_RATIO,       // 设置多路复用比率
        OLED_SSD1306_MULTIPLEX_RATIO_RESET,
        OLED_SSD1306_DISPLAY_OFFSET,        // 设置显示偏移
        OLED_SSD1306_DISPLAY_OFFSET_RESET,
        OLED_SSD1306_DISPLAY_START_LINE,    // 设置显示起始行
        OLED_SSD1306_SEGMENT_REMAP,         // 段重映射
        OLED_SSD1306_COM_OUTPUT_SCAN_DIRECTION, // COM扫描方向
        OLED_SSD1306_COM_PINS_HARDWARE,     // COM引脚硬件配置
        OLED_SSD1306_COM_PINS_HARDWARE_RESET,
        OLED_SSD1306_CONSTRAST_CONTROL,     // 对比度控制
        OLED_SSD1306_CONSTRAST_CONTROL_VALUE,
        OLED_SSD1306_PRECHARGE_PERIOD,      // 预充电周期
        OLED_SSD1306_PRECHARGE_PERIOD_VALUE,
        OLED_SSD1306_VCOMH_DESELECT_LEVEL,  // VCOMH反选电平
        OLED_SSD1306_VCOMH_DESELECT_LEVEL_VALUE,
        OLED_SSD1306_RAM_CONTENT_DISPLAY,   // 显示RAM内容
        OLED_SSD1306_NORMAL_DISPLAY,        // 正常显示（非反色）
        OLED_SSD1306_CHARGE_PUMP,           // 电荷泵设置
        OLED_SSD1306_CHARGE_PUMP_ENABLE,    // 启用电荷泵
        OLED_SSD1306_DISPLAY_ON             // 开启显示
    };

    OLED_WriteCommands(commands, sizeof(commands) / sizeof(commands[0]));

    // 等待显示稳定
    Delay_ms(100);

    // 3. 初始化外部字库Flash和中文字符缓存
    W25Q64_Init();
    OLED_CH_Cache_Init();
}

/*******************************************************************************
 * @brief  清空整个显示缓存
 * 
 * @note   将所有缓存数据置0，清屏后需要调用OLED_Update()刷新显示
 ******************************************************************************/
void OLED_Clear(void)
{
    for (uint8_t j = 0; j < OLED_MAX_PAGE; ++j) {
        for (uint8_t i = 0; i < OLED_MAX_COLUMN; ++i) {
            OLED_BUFFER[j][i] = 0x00;
        }
    }
}

/*******************************************************************************
 * @brief  清除指定矩形区域
 * 
 * @param  col    区域左上角列坐标(0-127)
 * @param  row    区域左上角行坐标(0-63)
 * @param  width  区域宽度(像素)
 * @param  height 区域高度(像素)
 * 
 * @note   支持跨页清除，自动处理页边界和位掩码
 *         清除方式：将对应像素位清0（黑色）
 ******************************************************************************/
void OLED_ClearArea(int16_t col, int16_t row, uint8_t width, uint8_t height)
{
    // 计算区域涉及的页范围（每页8行）
    int16_t start_page = row / 8;
    int16_t end_page = (row + height - 1) / 8;
    
    // 计算起始页和结束页的位掩码
    uint8_t start_mask = 0xFF << (row % 8);  // 起始页需要清除的位
    uint8_t end_mask = 0xFF >> (7 - ((row + height - 1) % 8)); // 结束页需要清除的位
    
    // 逐页处理
    for (int16_t page = start_page; page <= end_page; page++) {
        for (uint8_t x = col; x < col + width; x++) {
            if (page == start_page && page == end_page) {
                // 区域完全在同一页内
                OLED_BUFFER[page][x] &= ~(start_mask & end_mask);
            }
            else if (page == start_page) {
                // 区域起始页
                OLED_BUFFER[page][x] &= ~start_mask;
            }
            else if (page == end_page) {
                // 区域结束页
                OLED_BUFFER[page][x] &= ~end_mask;
            }
            else {
                // 中间完整页，整列清空
                OLED_BUFFER[page][x] = 0x00;
            }
        }
    }
}

/*******************************************************************************
 * @brief  反色整个显示缓存
 * 
 * @note   将缓存中所有数据位取反，实现反色显示效果
 *         需要调用OLED_Update()刷新显示
 ******************************************************************************/
void OLED_Reverse(void)
{
    for (uint8_t j = 0; j < OLED_MAX_PAGE; ++j) {
        for (uint8_t i = 0; i < OLED_MAX_COLUMN; ++i) {
            OLED_BUFFER[j][i] ^= 0xFF;  // 逐字节异或取反
        }
    }
}

/*******************************************************************************
 * @brief  反色指定矩形区域
 * 
 * @param  col    区域左上角列坐标
 * @param  row    区域左上角行坐标
 * @param  width  区域宽度
 * @param  height 区域高度
 * 
 * @note   将区域内每个像素取反，实现局部反色效果
 ******************************************************************************/
void OLED_ReverseArea(int16_t col, int16_t row, uint8_t width, uint8_t height)
{
    for (int16_t j = row; j < row + height; ++j) {    // 遍历行
        for (int16_t i = col; i < col + width; ++i) { // 遍历列
            if (i >= 0 && i <= OLED_MAX_COLUMN - 1 && j >= 0 && j <= OLED_MAX_PAGE * 8 - 1) {
                // 计算对应缓存位并取反
                OLED_BUFFER[j / 8][i] ^= 0x01 << (j % 8);
            }
        }
    }
}

/*******************************************************************************
 * @brief  将显示缓存内容刷新到OLED
 * 
 * @note   逐页将缓存数据写入OLED RAM，更新整个显示内容
 *         采用页寻址模式，每页对应8行像素
 ******************************************************************************/
void OLED_Update(void)
{
    for (uint8_t i = 0; i < 8; ++i) {
        OLED_SetCursor(0, i);  // 设置到每页起始位置
        OLED_WriteData(OLED_BUFFER[i], 128);  // 写入整页128列数据
    }
}

/*******************************************************************************
 * @brief  显示单个ASCII字符
 * 
 * @param  col      字符左上角列坐标
 * @param  row      字符左上角行坐标
 * @param  c        要显示的ASCII字符
 * @param  fontSize 字体大小：6或8点阵
 * 
 * @note   支持6x8和8x16两种字体
 *         字符数据从预定义字体数组中获取
 ******************************************************************************/
void OLED_ShowChar(int16_t col, int16_t row, char c, uint8_t fontSize)
{
    if (fontSize == 6) {
        // 6x8字体，字符间距6像素，高度8像素
        OLED_ShowImage(col, row, fontSize, 8, OLED_F6x8[c - ' '], true);
    } else if (fontSize == 8) {
        // 8x16字体，字符间距8像素，高度16像素
        OLED_ShowImage(col, row, fontSize, 16, OLED_F8x16[c - ' '], true);
    }
}

/*******************************************************************************
 * @brief  显示单个中文字符
 * 
 * @param  col 字符左上角列坐标
 * @param  row 字符左上角行坐标
 * @param  ch  UTF-8编码的中文字符串（3字节）
 * 
 * @note   中文固定为16x16点阵
 *         先转换为Unicode，再获取字模数据
 *         未找到字符时显示两个问号替代
 ******************************************************************************/
void OLED_ShowChineseChar(int16_t col, int16_t row, const char *ch)
{
    // 1. UTF-8转Unicode
    uint16_t unicode = UTF8_to_Unicode(ch);

    if (unicode == 0) {
        // 转换失败，显示两个问号替代
        OLED_ShowChar(col, row, '?', FONT_SIZE_8);
        OLED_ShowChar(col + FONT_SIZE_8, row, '?', FONT_SIZE_8);
        return;
    }

    // 2. 获取字模数据
    uint8_t fontData[CH_FONT_BYTES_PER_CHAR];
    uint8_t *fontPtr = OLED_Get_CH_FontData(unicode, fontData);

    if (fontPtr == NULL) {
        // 未找到字模，显示两个问号替代
        OLED_ShowChar(col, row, '?', FONT_SIZE_8);
        OLED_ShowChar(col + FONT_SIZE_8, row, '?', FONT_SIZE_8);
        return;
    }

    // 3. 显示中文字符
    OLED_ShowImage(col, row, CH_FONT_WIDTH, CH_FONT_HEIGHT, fontPtr, true);
}

/*******************************************************************************
 * @brief  显示字符串（支持中英混合）
 * 
 * @param  col      起始列坐标
 * @param  row      起始行坐标
 * @param  fontSize 字体大小（仅支持6或8，中文仅支持8）
 * @param  str      要显示的字符串（UTF-8编码）
 * 
 * @note   自动处理换行和边界检查
 *         中文为3字节UTF-8编码，英文为单字节ASCII
 *         行高：8点阵字体为8像素，16点阵字体为16像素
 ******************************************************************************/
void OLED_ShowString(int16_t col, int16_t row, uint8_t fontSize, const char *str)
{
    // 根据字体大小确定行高
    uint8_t lineHeight = (fontSize == FONT_SIZE_8) ? 16 : 8;
    
    while (*str) {
        // 判断字符类型：UTF-8首字节>=0xE0为中文字符
        if ((uint8_t)*str >= 0xE0) {
            // 中文显示仅支持8点阵字体
            if (fontSize == FONT_SIZE_8) {
                // 提取3字节UTF-8编码
                char ch[4] = { str[0], str[1], str[2], '\0' };
                
                // 边界检查：是否需要换行
                if (col + CH_FONT_WIDTH > OLED_MAX_COLUMN) {
                    col = 0;
                    row += lineHeight;
                }
                
                // 边界检查：是否超出屏幕高度
                if (row / 8 >= OLED_MAX_PAGE - 1) {
                    break;  // 超出屏幕，停止显示
                }
                
                // 显示中文字符
                OLED_ShowChineseChar(col, row, ch);
                col += CH_FONT_WIDTH;  // 中文宽度为16像素
            }
            str += 3;   // 跳过UTF-8编码的3个字节
        } else {
            // 英文/数字字符
            // 边界检查：是否需要换行
            if (col + fontSize > OLED_MAX_COLUMN) {
                col = 0;
                row += lineHeight;
            }
            
            // 边界检查：是否超出屏幕高度
            if (row / 8 >= OLED_MAX_PAGE - 1) {
                break;  // 超出屏幕，停止显示
            }
            
            // 显示ASCII字符
            OLED_ShowChar(col, row, *str, fontSize);
            col += fontSize;  // 英文宽度等于字体大小
            str += 1;   // 跳过单字节
        }
    }
}

/*******************************************************************************
 * @brief  显示无符号整数
 * 
 * @param  col      起始列坐标
 * @param  row      起始行坐标
 * @param  num      要显示的无符号整数
 * @param  fontSize 字体大小
 ******************************************************************************/
void OLED_ShowNum(int16_t col, int16_t row, uint32_t num, uint8_t fontSize)
{
    char str[12];  // 32位无符号整数最大10位数字 + 结束符
    sprintf(str, "%lu", num);
    OLED_ShowString(col, row, fontSize, str);
}

/*******************************************************************************
 * @brief  显示有符号整数
 * 
 * @param  col      起始列坐标
 * @param  row      起始行坐标
 * @param  num      要显示的有符号整数
 * @param  fontSize 字体大小
 ******************************************************************************/
void OLED_ShowSignedNum(int16_t col, int16_t row, int32_t num, uint8_t fontSize)
{
    char str[12];
    sprintf(str, "%ld", num);
    OLED_ShowString(col, row, fontSize, str);
}

/*******************************************************************************
 * @brief  显示十六进制数
 * 
 * @param  col      起始列坐标
 * @param  row      起始行坐标
 * @param  num      要显示的数值
 * @param  fontSize 字体大小
 ******************************************************************************/
void OLED_ShowHexNum(int16_t col, int16_t row, uint32_t num, uint8_t fontSize)
{
    char str[12];
    sprintf(str, "0x%X", num);
    OLED_ShowString(col, row, fontSize, str);
}

/*******************************************************************************
 * @brief  显示二进制数
 * 
 * @param  col      起始列坐标
 * @param  row      起始行坐标
 * @param  num      要显示的数值
 * @param  len      显示的二进制位数
 * @param  fontSize 字体大小
 * 
 * @note   格式为"0bxxxxxx"，如"0b101010"
 ******************************************************************************/
void OLED_ShowBinNum(int16_t col, int16_t row, uint32_t num, uint8_t len, uint8_t fontSize)
{
    char str[35];  // "0b" + 最大32位 + 结束符
    char *p = str;
    *p++ = '0';
    *p++ = 'b';
    // 从高位到低位依次处理每一位
    for (int8_t i = len - 1; i >= 0; --i) {
        *p++ = (num & (1 << i)) ? '1' : '0';
    }
    *p = '\0';
    OLED_ShowString(col, row, fontSize, str);
}

/*******************************************************************************
 * @brief  显示浮点数
 * 
 * @param  col      起始列坐标
 * @param  row      起始行坐标
 * @param  num      要显示的浮点数
 * @param  fracLen  小数部分位数
 * @param  fontSize 字体大小
 ******************************************************************************/
void OLED_ShowFloatNum(int16_t col, int16_t row, double num, uint8_t fracLen, uint8_t fontSize)
{
    char str[32];
    char format[10];
    // 动态生成格式字符串，如"%.2f"
    sprintf(format, "%%.%df", fracLen);
    sprintf(str, format, num);
    OLED_ShowString(col, row, fontSize, str);
}

/*******************************************************************************
 * @brief  显示位图图像
 * 
 * @param  col    图像左上角列坐标
 * @param  row    图像左上角行坐标
 * @param  width  图像宽度（像素）
 * @param  height 图像高度（像素）
 * @param  image  图像数据指针（按列存储）
 * @param  clear  是否先清除该区域
 * 
 * @note   图像数据按列优先存储，每列数据从上到下表示垂直方向像素
 *         支持跨页图像显示，自动处理页边界
 ******************************************************************************/
void OLED_ShowImage(int16_t col, int16_t row, uint8_t width, uint8_t height, const uint8_t *image, bool clear)
{
    // 可选：先清除显示区域
    if (clear) OLED_ClearArea(col, row, width, height);

    // 计算图像占用的页数（每页8行）
    for (uint8_t j = 0; j < (height - 1) / 8 + 1; ++j) {
        // 逐列处理
        for (uint8_t i = 0; i < width; ++i) {
            // 当前列的图像数据
            uint8_t imageData = image[i + j * width];
            
            // 处理跨页情况：
            // 1. 当前页部分：imageData左移row%8位
            OLED_BUFFER[row / 8 + j][col + i] |= imageData << (row % 8);
            
            // 2. 下一页部分：imageData右移(8-row%8)位
            OLED_BUFFER[row / 8 + j + 1][col + i] |= imageData >> (8 - (row % 8));
        }
    }
}

/*******************************************************************************
 * @brief  格式化输出（类似printf）
 * 
 * @param  col      起始列坐标
 * @param  row      起始行坐标
 * @param  fontSize 字体大小
 * @param  format   格式化字符串
 * @param  ...      可变参数列表
 * 
 * @note   使用vsprintf进行格式化，支持所有标准格式说明符
 *         注意缓冲区大小限制为256字节
 ******************************************************************************/
void OLED_Printf(int16_t col, int16_t row, uint8_t fontSize, char *format, ...)
{
    char str[256];
    va_list arg;
    va_start(arg, format);
    vsprintf(str, format, arg);
    va_end(arg);

    OLED_ShowString(col, row, fontSize, str);
}

/*******************************************************************************
 * @brief  绘制单个像素点
 * 
 * @param  x 像素点X坐标(0-127)
 * @param  y 像素点Y坐标(0-63)
 * 
 * @note   通过设置缓存中对应位为1来点亮像素
 ******************************************************************************/
void OLED_DrawPoint(int16_t x, int16_t y)
{
    OLED_BUFFER[y / 8][x] |= 0x01 << (y % 8);
}

/*******************************************************************************
 * @brief  获取指定位置像素状态
 * 
 * @param  x 像素点X坐标
 * @param  y 像素点Y坐标
 * @return true  像素点亮
 * @return false 像素熄灭
 ******************************************************************************/
bool OLED_GetPoint(int16_t x, int16_t y)
{
    return OLED_BUFFER[y / 8][x] & (0x01 << (y % 8));
}

/*******************************************************************************
 * @brief  绘制水平倾向直线（Bresenham算法）
 * 
 * @param  x0 起点X坐标
 * @param  y0 起点Y坐标
 * @param  x1 终点X坐标
 * @param  y1 终点Y坐标
 * 
 * @note   当|dx| > |dy|时调用此函数
 *         通过误差项D决定是否在Y方向移动
 ******************************************************************************/
static void OLED_DrawLineH(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
    // 确保x0 < x1，便于从左到右绘制
    if (x0 > x1) {
        int16_t temp = x0;
        x0 = x1;
        x1 = temp;

        temp = y0;
        y0 = y1;
        y1 = temp;
    }

    int16_t dx = (int16_t)x1 - (int16_t)x0;
    int16_t dy = (int16_t)y1 - (int16_t)y0;

    // 确定Y方向移动方向
    int8_t dir = (dy >= 0) ? 1 : -1;
    dy *= dir;  // 取绝对值

    if (dx != 0) {
        int16_t y = y0;
        int16_t D = 2 * dy - dx;  // 初始误差项

        for (int16_t i = 0; i <= dx; ++i) {
            OLED_DrawPoint(x0 + i, y);

            // 误差项决策
            if (D >= 0) {
                y += dir;      // Y方向移动一步
                D = D - 2 * dx; // 误差项更新
            }
            D = D + 2 * dy;    // 误差项累积
        }
    }
}

/*******************************************************************************
 * @brief  绘制垂直倾向直线（Bresenham算法）
 * 
 * @param  x0 起点X坐标
 * @param  y0 起点Y坐标
 * @param  x1 终点X坐标
 * @param  y1 终点Y坐标
 * 
 * @note   当|dy| > |dx|时调用此函数
 *         通过误差项D决定是否在X方向移动
 ******************************************************************************/
static void OLED_DrawLineV(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
    // 确保y0 < y1，便于从上到下绘制
    if (y0 > y1) {
        int16_t temp = y0;
        y0 = y1;
        y1 = temp;

        temp = x0;
        x0 = x1;
        x1 = temp;
    }

    int16_t dx = (int16_t)x1 - (int16_t)x0;
    int16_t dy = (int16_t)y1 - (int16_t)y0;

    // 确定X方向移动方向
    int8_t dir = (dx >= 0) ? 1 : -1;
    dx *= dir;  // 取绝对值

    if (dy != 0) {
        int16_t x = x0;
        int16_t D = 2 * dx - dy;  // 初始误差项

        for (int16_t i = 0; i <= dy; ++i) {
            OLED_DrawPoint(x, y0 + i);

            // 误差项决策
            if (D >= 0) {
                x += dir;      // X方向移动一步
                D = D - 2 * dy; // 误差项更新
            }
            D = D + 2 * dx;    // 误差项累积
        }
    }
}

/*******************************************************************************
 * @brief  绘制直线（自动选择最优算法）
 * 
 * @param  x0 起点X坐标
 * @param  y0 起点Y坐标
 * @param  x1 终点X坐标
 * @param  y1 终点Y坐标
 * 
 * @note   根据直线斜率自动选择水平倾向或垂直倾向算法
 *         使用Bresenham算法实现整数运算，避免浮点数
 ******************************************************************************/
void OLED_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
    // 根据斜率选择绘制算法
    if (abs(x1 - x0) > abs(y1 - y0)) {
        // 水平倾向直线（|dx| > |dy|）
        OLED_DrawLineH(x0, y0, x1, y1);
    } else {
        // 垂直倾向直线（|dy| > |dx|）
        OLED_DrawLineV(x0, y0, x1, y1);
    }
}

/*******************************************************************************
 * @brief  绘制矩形
 * 
 * @param  x      矩形左上角X坐标
 * @param  y      矩形左上角Y坐标
 * @param  width  矩形宽度
 * @param  height 矩形高度
 * @param  filled 是否填充
 * 
 * @note   空心矩形绘制四条边，填充矩形绘制所有水平线
 ******************************************************************************/
void OLED_DrawRectangle(int16_t x, int16_t y, uint8_t width, uint8_t height, bool filled)
{
    if (filled) {
        // 填充矩形：逐行绘制水平线
        for (uint8_t i = 0; i < height; ++i) {
            OLED_DrawLine(x, y + i, x + width - 1, y + i);
        }
    } else {
        // 空心矩形：绘制四条边
        OLED_DrawLine(x, y, x + width - 1, y);                     // 上边
        OLED_DrawLine(x, y + height - 1, x + width - 1, y + height - 1); // 下边
        OLED_DrawLine(x, y, x, y + height - 1);                   // 左边
        OLED_DrawLine(x + width - 1, y, x + width - 1, y + height - 1); // 右边
    }
}

/*******************************************************************************
 * @brief  绘制三角形
 * 
 * @param  x0     顶点0 X坐标
 * @param  y0     顶点0 Y坐标
 * @param  x1     顶点1 X坐标
 * @param  y1     顶点1 Y坐标
 * @param  x2     顶点2 X坐标
 * @param  y2     顶点2 Y坐标
 * @param  filled 是否填充
 * 
 * @note   填充三角形使用扫描线算法，空心三角形绘制三条边
 ******************************************************************************/
void OLED_DrawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, bool filled)
{
    if (filled) {
        // 填充三角形 - 扫描线算法
        // 1. 计算三角形的Y轴边界
        int16_t minY = y0 < y1 ? (y0 < y2 ? y0 : y2) : (y1 < y2 ? y1 : y2);
        int16_t maxY = y0 > y1 ? (y0 > y2 ? y0 : y2) : (y1 > y2 ? y1 : y2);

        // 2. 逐扫描线处理
        for (int16_t y = minY; y <= maxY; ++y) {
            int16_t xStart = OLED_MAX_COLUMN, xEnd = -1;

            // 3. 检查每条边与当前扫描线的交点
            int16_t edges[3][4] = { {x0, y0, x1, y1}, {x1, y1, x2, y2}, {x2, y2, x0, y0} };

            for (uint8_t i = 0; i < 3; ++i) {
                int16_t xA = edges[i][0], yA = edges[i][1];
                int16_t xB = edges[i][2], yB = edges[i][3];

                // 处理水平边特殊情况
                if (yA == yB) {
                    if (y == yA) {
                        // 水平边上的所有点都是交点
                        int16_t left  = (xA < xB) ? xA : xB;
                        int16_t right = (xA > xB) ? xA : xB;
                        if (left < xStart) xStart = left;
                        if (right > xEnd) xEnd = right;
                    }
                    continue;
                }

                // 检查扫描线是否穿过这条边
                if ((yA <= y && yB >= y) || (yB <= y && yA >= y)) {
                    // 计算交点X坐标（线性插值）
                    int16_t xIntersect = xA + (xB - xA) * (y - yA) / (yB - yA);
                    if (xIntersect < xStart) xStart = xIntersect;
                    if (xIntersect > xEnd) xEnd = xIntersect;
                }
            }

            // 4. 绘制当前扫描线段的填充部分
            if (xStart <= xEnd) {
                OLED_DrawLine(xStart, y, xEnd, y);
            }
        }
    } else {
        // 空心三角形：绘制三条边
        OLED_DrawLine(x0, y0, x1, y1);
        OLED_DrawLine(x1, y1, x2, y2);
        OLED_DrawLine(x2, y2, x0, y0);
    }
}

/*******************************************************************************
 * @brief  绘制圆形
 * 
 * @param  cx     圆心X坐标
 * @param  cy     圆心Y坐标
 * @param  r      半径
 * @param  filled 是否填充
 * 
 * @note   使用中点圆算法，基于八分对称性
 *         填充圆形通过绘制水平线实现
 ******************************************************************************/
void OLED_DrawCircle(int16_t cx, int16_t cy, uint8_t r, bool filled)
{
    int16_t x = 0;
    int16_t y = -r;          // 从顶部开始
    int16_t D = -r;          // 初始误差项

    while (x < -y) {         // 只计算45度圆弧（八分之一）
        // 误差项决策
        if (D > 0) {
            y += 1;
            D += 2 * y;      // 更新误差项
        }
        D += 2 * x + 1;      // 误差项累积

        if (filled) {
            // 填充圆形：绘制对称的水平线
            OLED_DrawLine(cx - x, cy + y, cx + x, cy + y); // 下水平线
            OLED_DrawLine(cx - x, cy - y, cx + x, cy - y); // 上水平线
            OLED_DrawLine(cx - y, cy + x, cx + y, cy + x); // 右水平线
            OLED_DrawLine(cx - y, cy - x, cx + y, cy - x); // 左水平线
        } else {
            // 空心圆形：绘制八个对称点
            OLED_DrawPoint(cx + x, cy + y);  // 右下
            OLED_DrawPoint(cx - x, cy + y);  // 左下
            OLED_DrawPoint(cx + x, cy - y);  // 右上
            OLED_DrawPoint(cx - x, cy - y);  // 左上
            OLED_DrawPoint(cx + y, cy + x);  // 下右
            OLED_DrawPoint(cx - y, cy + x);  // 下左
            OLED_DrawPoint(cx + y, cy - x);  // 上右
            OLED_DrawPoint(cx - y, cy - x);  // 上左
        }

        x += 1;  // 始终增加x
    }
}

/*******************************************************************************
 * @brief  绘制椭圆
 * 
 * @param  x      椭圆中心X坐标
 * @param  y      椭圆中心Y坐标
 * @param  a      长半轴（水平半径）
 * @param  b      短半轴（垂直半径）
 * @param  filled 是否填充
 * 
 * @note   使用中点椭圆算法，分两个区域绘制
 *         区域1：从上顶点到右顶点（斜率>=-1）
 *         区域2：从右顶点到下顶点（斜率<-1）
 ******************************************************************************/
void OLED_DrawEllipse(int16_t x, int16_t y, uint8_t a, uint8_t b, bool filled)
{
    long long a2 = (long long)a * a;
    long long b2 = (long long)b * b;
    
    // 区域1：从上顶点开始，向右侧绘制
    int16_t px = 0;
    int16_t py = b;
    long long delta = 2 * b2 + a2 * (1 - 2 * b);
    
    // 绘制直到斜率=-1的位置 (b²x <= a²y)
    while (b2 * px <= a2 * py) {
        if (filled) {
            // 填充椭圆：绘制对称水平线
            OLED_DrawLine(x - px, y - py, x + px, y - py);  // 上方线
            OLED_DrawLine(x - px, y + py, x + px, y + py);  // 下方线
        } else {
            // 空心椭圆：绘制四个对称点
            OLED_DrawPoint(x + px, y - py);  // 右上
            OLED_DrawPoint(x - px, y - py);  // 左上
            OLED_DrawPoint(x + px, y + py);  // 右下
            OLED_DrawPoint(x - px, y + py);  // 左下
        }
        
        // 误差项决策
        if (delta < 0) {
            delta = delta + 2 * b2 * (2 * px + 3);
        } else {
            delta = delta + 2 * a2 * (3 - 2 * py) + 2 * b2 * (2 * px + 3);
            py -= 1;  // y减小
        }
        px += 1;  // x增加
    }
    
    // 区域2：从右顶点开始，向下绘制
    px = a;
    py = 0;
    delta = 2 * a2 + b2 * (1 - 2 * a);
    
    // 绘制直到斜率=-1的位置 (a²y <= b²x)
    while (a2 * py <= b2 * px) {
        if (filled) {
            // 填充椭圆：绘制对称水平线
            OLED_DrawLine(x - px, y - py, x + px, y - py);  // 上方线
            OLED_DrawLine(x - px, y + py, x + px, y + py);  // 下方线
        } else {
            // 空心椭圆：绘制四个对称点
            OLED_DrawPoint(x + px, y - py);  // 右上
            OLED_DrawPoint(x - px, y - py);  // 左上
            OLED_DrawPoint(x + px, y + py);  // 右下
            OLED_DrawPoint(x - px, y + py);  // 左下
        }
        
        // 误差项决策
        if (delta < 0) {
            delta = delta + 2 * a2 * (2 * py + 3);
        } else {
            delta = delta + 2 * b2 * (3 - 2 * px) + 2 * a2 * (2 * py + 3);
            px -= 1;  // x减小
        }
        py += 1;  // y增加
    }
}

/*******************************************************************************
 * @brief  绘制圆弧/扇形
 * 
 * @param  x          圆心X坐标
 * @param  y          圆心Y坐标
 * @param  radius     半径
 * @param  startAngle 起始角度（度，0度为右侧水平方向，逆时针增加）
 * @param  endAngle   结束角度（度）
 * @param  filled     是否填充（true为扇形，false为圆弧）
 * 
 * @note   角度标准化为0-360度
 *         填充模式绘制从圆心到圆弧的射线实现扇形
 *         空心模式使用圆周点筛选实现圆弧
 ******************************************************************************/
void OLED_DrawArc(int16_t x, int16_t y, uint8_t radius, int16_t startAngle, int16_t endAngle, bool filled)
{
    // 1. 角度标准化到0-360度范围
    startAngle = startAngle % 360;
    endAngle = endAngle % 360;
    
    if (startAngle < 0) startAngle += 360;
    if (endAngle < 0) endAngle += 360;
    
    // 2. 填充模式：绘制扇形
    if (filled) {
        int16_t angleStep = 1;  // 角度步长（可调整精度）
        
        int16_t angle = startAngle;
        while (angle != endAngle) {
            // 计算圆周点坐标（极坐标转直角坐标）
            double rad = angle * 3.14159265359 / 180.0;
            int16_t px = (int16_t)(radius * cos(rad) + 0.5);  // 四舍五入
            int16_t py = (int16_t)(radius * sin(rad) + 0.5);
            
            // 绘制从圆心到圆周点的射线
            OLED_DrawLine(x, y, x + px, y + py);
            
            // 角度递增
            angle += angleStep;
            if (angle > 360) angle -= 360;
            
            // 循环终止条件
            if ((startAngle < endAngle && angle > endAngle) || 
                (startAngle > endAngle && angle > endAngle && angle < startAngle)) {
                break;
            }
        }
    } else {
        // 3. 空心模式：绘制圆弧
        int16_t px = 0, py = radius;
        int16_t D = 3 - 2 * radius;  // 中点圆算法误差项
        
        // 生成整个圆周的点
        while (px <= py) {
            // 圆周上的八个对称点
            int16_t points[8][2] = {
                {px, py}, {py, px}, {py, -px}, {px, -py},
                {-px, -py}, {-py, -px}, {-py, px}, {-px, py}
            };
            
            // 检查每个对称点是否在角度范围内
            for (int i = 0; i < 8; ++i) {
                int16_t ptx = x + points[i][0];
                int16_t pty = y + points[i][1];
                
                // 计算当前点的角度（弧度）
                double angle_rad = atan2(points[i][1], points[i][0]);
                if (angle_rad < 0) angle_rad += 2 * 3.14159265359;
                double angle_deg = angle_rad * 180.0 / 3.14159265359;
                
                // 判断角度是否在指定范围内
                bool inRange = false;
                if (startAngle <= endAngle) {
                    // 正常范围：startAngle ≤ angle ≤ endAngle
                    inRange = (angle_deg >= startAngle && angle_deg <= endAngle);
                } else {
                    // 跨360度范围：angle ≥ startAngle 或 angle ≤ endAngle
                    inRange = (angle_deg >= startAngle || angle_deg <= endAngle);
                }
                
                // 在范围内则绘制该点
                if (inRange) {
                    OLED_DrawPoint(ptx, pty);
                }
            }
            
            // 更新误差项和坐标
            if (D < 0) {
                D = D + 4 * px + 6;
            } else {
                D = D + 4 * (px - py) + 10;
                py -= 1;
            }
            px += 1;
        }
    }
}
