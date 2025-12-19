from pathlib import Path

BASE_DIR = Path(__file__).resolve().parent

INPUT_FONT_BIN = BASE_DIR / "CH_Font.bin"   # 字体 BIN 文件路径

SERIAL_PORT = "COM7"    # 串口号
BAUD_RATE = 115200  # 波特率

__all__ = [ 'BASE_DIR', 'INPUT_FONT_BIN', 'SERIAL_PORT', 'BAUD_RATE' ]
