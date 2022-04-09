from io import BufferedReader
import os

# 8bpp
def convert_image(image: BufferedReader, filename: str):
    with open("../include/gfx/" + filename + ".h", "wb") as outfile:
        image.seek(0, os.SEEK_END)
        array_length = image.tell()//2
        image.seek(0)

        outfile.write(b"#include <nds/ndstypes.h>\n\n")
        outfile.write(b"extern u16 " + bytes(filename, "utf-8") + b"[" + bytes(str(array_length), "utf-8") + b"];")
    with open("../source/gfx/" + filename + ".c", "wb") as outfile:
        outfile.write(b"#include \"gfx/" + bytes(filename, "utf-8") + b".h\"\n\n")
        outfile.write(b"u16 " + bytes(filename, "utf-8") + b"[" + bytes(str(array_length), "utf-8") + b"]" + b"= {\n    ")
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
if not os.path.exists("../source/gfx"):
    os.mkdir("../source/gfx")
with open("../include/gfx/gfx.h", "w") as header:
    header.write("#ifndef GFXH\n#define GFXH\n\n")
    for _filename in os.listdir("../ext"):
        with open(f"../ext/{_filename}", "rb") as im:
            filename = os.path.splitext(_filename)[0]
            convert_image(im, filename)
        header.write(f"#include \"gfx/{filename}.h\"\n")
    header.write("#endif")