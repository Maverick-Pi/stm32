from config import *
from parser.font_c_parser import parse_font_c_file
from generator.bin_writer import write_bin
from generator.index_writer import write_index


def main():
    glyphs = parse_font_c_file(INPUT_FONT_C)

    write_bin(OUTPUT_BIN, glyphs)
    write_index(OUTPUT_INDEX, glyphs)

    print("字模转换完成")
    print(f"字数: {len(glyphs)}")
    print(f"BIN: {OUTPUT_BIN}")
    print(f"IDX: {OUTPUT_INDEX}")


if __name__ == "__main__":
    main()
