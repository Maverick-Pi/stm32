import re
from model.font_model import FontGlyph
from config import FONT_BYTES_PER_CHAR

HEX_PATTERN = re.compile(r'0x([0-9A-Fa-f]{2})')
COMMENT_PATTERN = re.compile(r"/\*\s*'(.{1})'\s*,\s*(\d+)\s*\*/")

def parse_font_c_file(filepath: str) -> list[FontGlyph]:
    glyphs = []
    current_bytes = []

    with open(filepath, "r", encoding="utf-8") as f:
        for line in f:
            # 提取字模数据
            for hv in HEX_PATTERN.findall(line):
                current_bytes.append(int(hv, 16))
            
            # 提取注释中的汉字和索引
            m = COMMENT_PATTERN.search(line)
            if m:
                char = m.group(1)
                index = int(m.group(2))
                unicode_val = ord(char)

                if len(current_bytes) != FONT_BYTES_PER_CHAR:
                    raise ValueError(
                        r"字 '{char}' 字节数错误：{len(current_bytes)}"
                    )

                glyphs.append(
                    FontGlyph(unicode=unicode_val, index=index, data=bytes(current_bytes))
                )
                current_bytes.clear()

    return glyphs
