from dataclasses import dataclass

@dataclass
class FontGlyph:
    unicode: int    # Unicode 编码 (uint16_t)
    index: int      # 字模索引
    data: bytes     # 32 字节字模
