from model.font_model import FontGlyph


def write_index(filepath: str, glyphs: list[FontGlyph]):
    with open(filepath, "w", encoding="utf-8") as f:
        f.write("#ifndef __CH_FONT_INDEX_H__\n")
        f.write("#define __CH_FONT_INDEX_H__\n\n")

        f.write("#include <stdint.h>\n\n")

        f.write("typedef struct\n{\n")
        f.write("    uint16_t unicode;   // Unicode 编码\n")
        f.write("    uint16_t index;     // 字模索引\n")
        f.write("} CH_FontIndex_t;\n\n")

        f.write(f"#define CH_FONT_COUNT {len(glyphs)}\n")
        f.write("#define CH_FONT_BYTES_PER_CHAR 32\n")
        f.write("#define CH_FONT_BASE_ADDR 0x000000\n")
        f.write("#define CH_FONT_WIDTH 16\n")
        f.write("#define CH_FONT_HEIGHT 16\n\n")
        f.write("#define CH_CACHE_SIZE 32\n\n")

        f.write("// 汉字缓存(最近使用的字模)\n")
        f.write("typedef struct {\n")
        f.write("    uint16_t unicode;  // Unicode 编码\n")
        f.write("    uint8_t data[CH_FONT_BYTES_PER_CHAR];  // 字模数据\n")
        f.write("    uint8_t used;  // 是否已使用\n")
        f.write("} CH_FontCache_t;\n\n")
        

        f.write("static const CH_FontIndex_t OLED_CH_FontIndex[] =\n{\n")
        for g in glyphs:
            f.write(f"    {{0x{g.unicode:04X}, {g.index}}},\n")
        f.write("};\n\n")

        f.write("#endif /* __CH_FONT_INDEX_H__ */\n")
