from pathlib import Path

BASE_DIR = Path(__file__).resolve().parent

INPUT_FONT_C = BASE_DIR / "assets/CH_Font_Data.h"

OUTPUT_BIN = BASE_DIR / "assets/CH_Font.bin"
OUTPUT_INDEX = BASE_DIR / "assets/CH_Font_Index.h"

FONT_BYTES_PER_CHAR = 32

__all__ = ['INPUT_FONT_C', 'OUTPUT_BIN', 'OUTPUT_INDEX', 'FONT_BYTES_PER_CHAR']
