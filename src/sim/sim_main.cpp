#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

#ifndef MATRIX_WIDTH
#define MATRIX_WIDTH 64
#endif
#ifndef MATRIX_HEIGHT
#define MATRIX_HEIGHT 32
#endif
#ifndef CHAIN_LENGTH
#define CHAIN_LENGTH 1
#endif

// Tiny 3x5 font for digits and a few letters (monospace)
static const uint8_t font3x5[][5] = {
    // '0'..'9'
    {0b111,0b101,0b101,0b101,0b111}, //0
    {0b010,0b110,0b010,0b010,0b111}, //1
    {0b111,0b001,0b111,0b100,0b111}, //2
    {0b111,0b001,0b111,0b001,0b111}, //3
    {0b101,0b101,0b111,0b001,0b001}, //4
    {0b111,0b100,0b111,0b001,0b111}, //5
    {0b111,0b100,0b111,0b101,0b111}, //6
    {0b111,0b001,0b001,0b001,0b001}, //7
    {0b111,0b101,0b111,0b101,0b111}, //8
    {0b111,0b101,0b111,0b001,0b111}, //9
};

static inline uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

struct SimPanel {
    int w, h;
    std::vector<uint16_t> buf; // RGB565
    SimPanel(int width, int height) : w(width), h(height), buf(width*height, 0) {}
    void clear() { std::fill(buf.begin(), buf.end(), 0); }
    void fillRect(int x, int y, int ww, int hh, uint16_t c) {
        for (int j = 0; j < hh; ++j) {
            int yy = y + j; if (yy < 0 || yy >= h) continue;
            for (int i = 0; i < ww; ++i) {
                int xx = x + i; if (xx < 0 || xx >= w) continue;
                buf[yy*w + xx] = c;
            }
        }
    }
    void blitDigit(char ch, int x, int y, int scale, uint16_t c) {
        if (ch < '0' || ch > '9') return;
        const uint8_t* glyph = font3x5[ch - '0'];
        for (int row = 0; row < 5; ++row) {
            for (int col = 0; col < 3; ++col) {
                if (glyph[row] & (1 << (2 - col))) {
                    fillRect(x + col*scale, y + row*scale, scale, scale, c);
                }
            }
        }
    }
    void blitText(const std::string& s, int x, int y, int scale, uint16_t c) {
        int cursor = x;
        for (char ch : s) {
            if (ch >= '0' && ch <= '9') {
                blitDigit(ch, cursor, y, scale, c);
                cursor += (3*scale) + scale; // 1 col spacing
            } else {
                cursor += (3*scale) + scale;
            }
        }
    }
    bool writePPM(const std::string& path) {
        FILE* f = std::fopen(path.c_str(), "wb");
        if (!f) return false;
        std::fprintf(f, "P6\n%d %d\n255\n", w, h);
        for (int j = 0; j < h; ++j) {
            for (int i = 0; i < w; ++i) {
                uint16_t p = buf[j*w + i];
                uint8_t r = (p >> 11) & 0x1F; r = (r << 3) | (r >> 2);
                uint8_t g = (p >> 5) & 0x3F;  g = (g << 2) | (g >> 4);
                uint8_t b = p & 0x1F;        b = (b << 3) | (b >> 2);
                std::fputc(r, f); std::fputc(g, f); std::fputc(b, f);
            }
        }
        std::fclose(f);
        return true;
    }
};

int main() {
    const int totalW = MATRIX_WIDTH * CHAIN_LENGTH;
    SimPanel panel(totalW, MATRIX_HEIGHT);

    // Color bars test
    int bar = (totalW / 6) > 0 ? totalW / 6 : 1;
    panel.fillRect(0*bar, 0, bar, MATRIX_HEIGHT, color565(255,0,0));
    panel.fillRect(1*bar, 0, bar, MATRIX_HEIGHT, color565(0,255,0));
    panel.fillRect(2*bar, 0, bar, MATRIX_HEIGHT, color565(0,0,255));
    panel.fillRect(3*bar, 0, bar, MATRIX_HEIGHT, color565(255,255,0));
    panel.fillRect(4*bar, 0, bar, MATRIX_HEIGHT, color565(0,255,255));
    panel.fillRect(5*bar, 0, bar, MATRIX_HEIGHT, color565(255,0,255));
    panel.writePPM("build_colorbars.ppm");

    // Seconds frames 0..5 as demo
    for (int s = 0; s <= 5; ++s) {
        panel.clear();
        // header band
        panel.fillRect(0, 0, totalW, 8, color565(0,0,32));
        // centered counter text
        std::string txt = std::to_string(s);
        int scale = 4; // makes about 12x20 per digit
        int textW = (int)txt.size() * ((3*scale) + scale) - scale; // last has no gap
        int x = (totalW - textW) / 2; if (x < 0) x = 0;
        int y = 8 + (MATRIX_HEIGHT - 8 - 5*scale)/2; if (y < 8) y = 8;
        panel.blitText(txt, x, y, scale, color565(255,200,0));
        char name[64]; std::snprintf(name, sizeof(name), "build_frame_%02d.ppm", s);
        panel.writePPM(name);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    return 0;
}
