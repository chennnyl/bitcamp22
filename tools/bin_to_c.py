from io import BufferedReader
import os

# 8bpp
def convert_image(image: BufferedReader, path: str, filename: str):
    with open(path + filename + ".h", "wb") as outfile:
        outfile.write(b"#include <nds/ndstypes.h>\n\n")
        outfile.write(b"u16 " + bytes(filename, "utf-8") + b"[] = {\n    ")
        b = None
        total = 0
        while b != b'':
            b = image.read(2)
            if b == b'':
                break
            outfile.write(bytes(str(int.from_bytes(b, "little")), "utf-8") + b",")
            total += 2
            if total % 16 == 0:
                outfile.write(b"\n    ")
        outfile.write(b"\n};")
        pass
    pass

# 2 bytes per entry
def convert_palette(palette: BufferedReader, path: str, filename: str):
    with open(path + filename + ".c", "wb") as outfile:
        pass
    pass

if not os.path.exists("../include/gfx"):
    os.mkdir("../include/gfx")
for filename in os.listdir("../ext"):
    with open(f"../ext/{filename}", "rb") as im:
        convert_image(im, "../include/gfx/", os.path.splitext(filename)[0])