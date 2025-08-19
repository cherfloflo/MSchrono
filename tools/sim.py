import struct

def color565(r,g,b):
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)

def write_ppm(path, w, h, buf):
    with open(path, 'wb') as f:
        f.write(f"P6\n{w} {h}\n255\n".encode())
        for p in buf:
            r = (p >> 11) & 0x1F; r = (r << 3) | (r >> 2)
            g = (p >> 5) & 0x3F;  g = (g << 2) | (g >> 4)
            b = p & 0x1F;        b = (b << 3) | (b >> 2)
            f.write(bytes((r,g,b)))

def fill_rect(buf, w, h, x, y, ww, hh, c):
    for j in range(hh):
        yy = y + j
        if yy < 0 or yy >= h: continue
        base = yy * w
        for i in range(ww):
            xx = x + i
            if xx < 0 or xx >= w: continue
            buf[base + xx] = c

font3x5 = [
    [0b111,0b101,0b101,0b101,0b111],
    [0b010,0b110,0b010,0b010,0b111],
    [0b111,0b001,0b111,0b100,0b111],
    [0b111,0b001,0b111,0b001,0b111],
    [0b101,0b101,0b111,0b001,0b001],
    [0b111,0b100,0b111,0b001,0b111],
    [0b111,0b100,0b111,0b101,0b111],
    [0b111,0b001,0b001,0b001,0b001],
    [0b111,0b101,0b111,0b101,0b111],
    [0b111,0b101,0b111,0b001,0b111],
]

def blit_digit(buf, w, h, ch, x, y, scale, c):
    idx = ord(ch) - ord('0')
    if idx < 0 or idx > 9: return
    glyph = font3x5[idx]
    for row in range(5):
        for col in range(3):
            if glyph[row] & (1 << (2-col)):
                fill_rect(buf, w, h, x + col*scale, y + row*scale, scale, scale, c)

def blit_text(buf, w, h, text, x, y, scale, c):
    cursor = x
    for ch in text:
        if '0' <= ch <= '9':
            blit_digit(buf, w, h, ch, cursor, y, scale, c)
        cursor += (3*scale) + scale

def simulate(totalW=64, H=32, seconds=5):
    buf = [0] * (totalW * H)
    # color bars
    bar = max(1, totalW // 6)
    fill_rect(buf, totalW, H, 0*bar, 0, bar, H, color565(255,0,0))
    fill_rect(buf, totalW, H, 1*bar, 0, bar, H, color565(0,255,0))
    fill_rect(buf, totalW, H, 2*bar, 0, bar, H, color565(0,0,255))
    fill_rect(buf, totalW, H, 3*bar, 0, bar, H, color565(255,255,0))
    fill_rect(buf, totalW, H, 4*bar, 0, bar, H, color565(0,255,255))
    fill_rect(buf, totalW, H, 5*bar, 0, bar, H, color565(255,0,255))
    write_ppm('build_colorbars.ppm', totalW, H, buf)
    # frames
    for s in range(seconds+1):
        buf = [0] * (totalW * H)
        fill_rect(buf, totalW, H, 0, 0, totalW, 8, color565(0,0,32))
        txt = str(s)
        scale = 4
        textW = len(txt) * ((3*scale)+scale) - scale
        x = max(0, (totalW - textW)//2)
        y = 8 + max(0, (H - 8 - 5*scale)//2)
        blit_text(buf, totalW, H, txt, x, y, scale, color565(255,200,0))
        write_ppm(f'build_frame_{s:02d}.ppm', totalW, H, buf)

if __name__ == '__main__':
    simulate()
