/****************************************************************************/ /**
 * @file   main.c
 * @brief  0.96寸OLED显示驱动库测试程序
 * 
 * 本程序用于测试OLED.c中所有对外暴露的显示功能，包括：
 * 1. 基本显示控制函数
 * 2. 字符和字符串显示函数
 * 3. 数字格式化显示函数
 * 4. 图形绘制函数
 * 5. 缓存管理函数
 * 
 * @note 显示分辨率为128x64像素，所有坐标单位为像素
 *       测试程序按功能模块分组，每组测试后有2秒延时
 * 
 * @author Maverick Pi
 * @date   2025-12-25 17:47:52 更新
 ********************************************************************************/

#include "stm32f10x.h"
#include "OLED.h"
#include "Delay.h"

/* 测试用中文字符串（UTF-8编码） */
const char *chineseTest = "测试中文显示功能";

/**
 * @brief  测试基本显示控制功能
 * @note   包括清屏、区域清除、反色等基础操作
 */
void Test_BasicDisplay(void)
{
    OLED_Clear();               // 清屏
    OLED_Update();              // 刷新显示
    Delay_ms(1000);
    
    // 绘制一个矩形区域
    OLED_DrawRectangle(20, 20, 40, 20, true);  // 填充矩形
    OLED_Update();
    Delay_ms(1000);
    
    // 清除部分区域
    OLED_ClearArea(30, 25, 20, 10);
    OLED_Update();
    Delay_ms(1000);
    
    // 反色显示
    OLED_Reverse();
    OLED_Update();
    Delay_ms(1000);
    
    // 反色恢复
    OLED_Reverse();
    OLED_Update();
    Delay_ms(1000);
    
    // 局部反色
    OLED_ReverseArea(40, 20, 20, 10);
    OLED_Update();
    Delay_ms(2000);
    
    OLED_Clear();
    OLED_Update();
}

/**
 * @brief  测试字符和字符串显示功能
 * @note   测试ASCII字符、中文字符和混合字符串显示
 */
void Test_CharAndString(void)
{
    OLED_Clear();
    
    // 测试6x8字体（ASCII字符）
    OLED_ShowString(0, 0, FONT_SIZE_6, "6x8 Font Test:");
    OLED_ShowChar(0, 10, 'A', FONT_SIZE_6);
    OLED_ShowChar(8, 10, 'B', FONT_SIZE_6);
    OLED_ShowString(16, 10, FONT_SIZE_6, "ABCDEFG");
    OLED_ShowString(0, 20, FONT_SIZE_6, "Hello World!");
    
    // 测试8x16字体（ASCII字符）
    OLED_ShowString(0, 32, FONT_SIZE_8, "8x16 Font Test:");
    OLED_ShowChar(0, 48, 'A', FONT_SIZE_8);
    OLED_ShowChar(10, 48, 'B', FONT_SIZE_8);
    OLED_ShowString(20, 48, FONT_SIZE_8, "STM32 OLED");
    
    OLED_Update();
    Delay_ms(3000);
    
    OLED_Clear();
    
    // 测试中文字符显示（需确保中文字库已正确加载）
    OLED_ShowString(0, 0, FONT_SIZE_8, "中文测试:");
    OLED_ShowChineseChar(0, 16, "测");
    OLED_ShowChineseChar(16, 16, "试");
    OLED_ShowChineseChar(32, 16, "中");
    OLED_ShowChineseChar(48, 16, "文");
    
    // 测试中英混合字符串
    OLED_ShowString(0, 40, FONT_SIZE_8, "Mixed 中英文 Test");
    
    OLED_Update();
    Delay_ms(3000);
    
    OLED_Clear();
    OLED_Update();
}

/**
 * @brief  测试数字格式化显示功能
 * @note   包括整数、浮点数、十六进制、二进制等多种格式
 */
void Test_NumberDisplay(void)
{
    OLED_Clear();
    
    // 测试无符号整数
    OLED_ShowString(0, 0, FONT_SIZE_8, "Unsigned Num:");
    OLED_ShowNum(0, 16, 1234567890, FONT_SIZE_8);
    
    // 测试有符号整数
    OLED_ShowString(0, 32, FONT_SIZE_8, "Signed Num:");
    OLED_ShowSignedNum(0, 48, -123456, FONT_SIZE_8);
    
    OLED_Update();
    Delay_ms(2000);
    
    OLED_Clear();
    
    // 测试十六进制数
    OLED_ShowString(0, 0, FONT_SIZE_8, "Hex Num:");
    OLED_ShowHexNum(0, 16, 0xABCDEF12, FONT_SIZE_8);
    
    // 测试二进制数
    OLED_ShowString(0, 32, FONT_SIZE_8, "Bin Num:");
    OLED_ShowBinNum(0, 48, 911, 12, FONT_SIZE_8);
    
    OLED_Update();
    Delay_ms(2000);
    
    OLED_Clear();
    
    // 测试浮点数
    OLED_ShowString(0, 0, FONT_SIZE_8, "Float Num:");
    OLED_ShowFloatNum(0, 16, 3.1415926, 4, FONT_SIZE_8);
    OLED_ShowFloatNum(0, 32, -2.71828, 3, FONT_SIZE_8);
    
    OLED_Update();
    Delay_ms(3000);
    
    OLED_Clear();
    OLED_Update();
}

/**
 * @brief  测试格式化输出功能
 * @note   测试OLED_Printf函数，类似标准printf
 */
void Test_FormatOutput(void)
{
    OLED_Clear();
    
    // 测试多种格式输出
    OLED_ShowString(0, 0, FONT_SIZE_8, "Format Output Test:");
    
    // 整数格式化
    OLED_Printf(0, 16, FONT_SIZE_8, "Int: %d", 12345);
    OLED_Printf(0, 32, FONT_SIZE_8, "Hex: 0x%04X", 0xABCD);
    
    // 浮点数格式化
    OLED_Printf(0, 48, FONT_SIZE_8, "Float: %.3f", 3.14159);
    
    OLED_Update();
    Delay_ms(2000);
    
    OLED_Clear();
    
    // 测试复杂格式化
    OLED_ShowString(0, 0, FONT_SIZE_8, "Complex Format:");
    OLED_Printf(0, 16, FONT_SIZE_8, "Value: %d, Rate: %.1f%%", 85, 98.5);
    OLED_Printf(0, 32, FONT_SIZE_8, "Temp: %.1fC, Hum: %d%%", 25.5, 60);
    OLED_Printf(0, 48, FONT_SIZE_8, "0x%02X = %c", 65, 65);
    
    OLED_Update();
    Delay_ms(3000);
    
    OLED_Clear();
    OLED_Update();
}

/**
 * @brief  测试基本图形绘制功能
 * @note   测试点、线、矩形、三角形等基本图形
 */
void Test_BasicGraphics(void)
{
    OLED_Clear();
    
    // 测试绘制点
    OLED_ShowString(0, 0, FONT_SIZE_6, "Draw Points:");
    for (int i = 0; i < 10; i++) {
        OLED_DrawPoint(30 + i * 2, 15 + i);
    }
    
    // 测试绘制直线
    OLED_ShowString(0, 25, FONT_SIZE_6, "Draw Lines:");
    OLED_DrawLine(20, 35, 100, 35);     // 水平线
    OLED_DrawLine(60, 30, 60, 50);      // 垂直线
    OLED_DrawLine(20, 50, 100, 30);     // 斜线
    
    OLED_Update();
    Delay_ms(2000);
    
    OLED_Clear();
    
    // 测试绘制矩形（空心和填充）
    OLED_ShowString(0, 0, FONT_SIZE_6, "Draw Rectangles:");
    OLED_DrawRectangle(10, 15, 30, 20, false);  // 空心矩形
    OLED_DrawRectangle(50, 15, 30, 20, true);   // 填充矩形
    
    // 测试绘制三角形（空心和填充）
    OLED_ShowString(0, 40, FONT_SIZE_6, "Draw Triangles:");
    OLED_DrawTriangle(31, 45, 95, 45, 63, 60, false);  // 空心三角形
    OLED_DrawTriangle(63, 45, 50, 55, 76, 55, true);   // 填充三角形
    
    OLED_Update();
    Delay_ms(3000);
    
    OLED_Clear();
    OLED_Update();
}

/**
 * @brief  测试高级图形绘制功能
 * @note   测试圆、椭圆、圆弧等复杂图形
 */
void Test_AdvancedGraphics(void)
{
    OLED_Clear();
    
    // 测试绘制圆（空心和填充）
    OLED_ShowString(0, 0, FONT_SIZE_6, "Draw Circles:");
    OLED_DrawCircle(30, 20, 15, false);  // 空心圆
    OLED_DrawCircle(90, 20, 10, true);   // 填充圆
    
    // 测试绘制椭圆（空心和填充）
    OLED_ShowString(0, 40, FONT_SIZE_6, "Draw Ellipses:");
    OLED_DrawEllipse(30, 50, 25, 10, false);  // 空心椭圆
    OLED_DrawEllipse(90, 50, 15, 8, true);    // 填充椭圆
    
    OLED_Update();
    Delay_ms(2000);
    
    OLED_Clear();
    
    // 测试绘制圆弧（空心和填充）
    OLED_ShowString(0, 0, FONT_SIZE_6, "Draw Arcs:");
    
    // 空心圆弧
    OLED_DrawArc(40, 30, 15, 30, 120, false);   // 30°到120°圆弧
    OLED_DrawArc(40, 30, 20, 150, 330, false);  // 150°到330°圆弧
    
    // 填充扇形
    OLED_DrawArc(90, 30, 15, 0, 90, true);      // 0°到90°扇形
    OLED_DrawArc(90, 30, 12, 180, 270, true);   // 180°到270°扇形
    
    OLED_Update();
    Delay_ms(3000);
    
    OLED_Clear();
    OLED_Update();
}

/**
 * @brief  综合测试：所有功能的组合演示
 * @note   展示OLED库的完整功能，创建一个小型UI界面
 */
void Test_Comprehensive(void)
{
    OLED_Clear();
    
    // 绘制UI边框
    OLED_DrawRectangle(0, 0, 128, 64, false);
    OLED_DrawLine(0, 16, 128, 16);
    
    // 显示标题
    OLED_ShowString(40, 4, FONT_SIZE_8, "OLED Demo");
    
    // 显示系统信息
    OLED_ShowString(5, 20, FONT_SIZE_6, "System Status:");
    OLED_ShowString(5, 28, FONT_SIZE_6, "STM32F103C8T6");
    OLED_ShowString(5, 36, FONT_SIZE_6, "RAM: 20KB");
    OLED_ShowString(5, 44, FONT_SIZE_6, "Flash: 64KB");
    
    // 显示实时数据
    OLED_ShowString(70, 20, FONT_SIZE_6, "Real-time Data:");
    OLED_Printf(70, 28, FONT_SIZE_6, "Temp: %.1fC", 25.5);
    OLED_Printf(70, 36, FONT_SIZE_6, "Hum: %d%%", 60);
    OLED_Printf(70, 44, FONT_SIZE_6, "Volt: %.2fV", 3.30);
    
    // 绘制进度条
    OLED_ShowString(5, 52, FONT_SIZE_6, "Progress:");
    OLED_DrawRectangle(45, 52, 78, 6, false);  // 进度条外框
    OLED_DrawRectangle(46, 53, 38, 4, true);   // 进度条填充（50%）
    
    // 显示百分比
    OLED_ShowString(110, 52, FONT_SIZE_6, "50%");
    
    OLED_Update();
    Delay_ms(5000);
    
    OLED_Clear();
    OLED_Update();
}

/**
 * @brief  主函数 - 测试程序入口
 * @note   依次执行所有测试用例，每个测试后有延时
 */
int main(void)
{
    /* 初始化OLED */
    OLED_Init();
    OLED_Clear();
    OLED_Update();
    Delay_ms(500);
    
    /* 显示启动信息 */
    OLED_ShowString(0, 20, FONT_SIZE_8, "OLED Test Start");
    OLED_Update();
    Delay_ms(2000);
    
    /* 执行所有测试用例 */
    Test_BasicDisplay();       // 测试基本显示控制
    Test_CharAndString();      // 测试字符和字符串显示
    Test_NumberDisplay();      // 测试数字格式化显示
    Test_FormatOutput();       // 测试格式化输出
    Test_BasicGraphics();      // 测试基本图形绘制
    Test_AdvancedGraphics();   // 测试高级图形绘制
    Test_Comprehensive();      // 综合测试
    
    /* 显示测试完成信息 */
    OLED_Clear();
    OLED_ShowString(0, 20, FONT_SIZE_8, "All Tests Passed");
    OLED_Update();
    
    /* 主循环 */
    while (1)
    {
        /* 可在此处添加持续运行的代码 */
    }
}
