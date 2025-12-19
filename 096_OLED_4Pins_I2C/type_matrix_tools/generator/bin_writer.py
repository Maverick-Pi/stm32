from model.font_model import FontGlyph

def write_bin(filepath: str, glyphs: list[FontGlyph]):
    with open(filepath, "wb") as f:
        for g in glyphs:
            f.write(g.data)
