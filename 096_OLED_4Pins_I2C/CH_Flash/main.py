#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
字体烧录工具 - 主程序
"""

import sys
import os
from flash_programmer import send_font_data
from config import *

def main():
    print("=== 字体烧录工具 ===")
    print("")
    
    # 检查文件是否存在
    if not os.path.exists(INPUT_FONT_BIN):
        print(f"错误: 文件 '{INPUT_FONT_BIN}' 不存在")
        return
    
    print(f"准备烧录文件: {INPUT_FONT_BIN}")
    print(f"串口: {SERIAL_PORT}, 波特率: {BAUD_RATE}")
    print("")
    
    try:
        # 开始烧录
        send_font_data(SERIAL_PORT, BAUD_RATE, INPUT_FONT_BIN)
        print("")
        print("=== 烧录完成 ===")
    except KeyboardInterrupt:
        print("\n用户中断")
    except Exception as e:
        print(f"\n错误: {e}")
        import traceback
        traceback.print_exc()

if __name__ == "__main__":
    main()
