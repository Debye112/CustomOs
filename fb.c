// fb.c
#include "fb.h"

static volatile uint32_t *const fb = (uint32_t *)FB_BASE;

// 8x8 font for ASCII 32–127 (you can replace with a full font later)
extern const uint8_t font8x8_basic[128][8];

void fb_put_pixel(int x, int y, uint32_t color) {
    if (x < 0 || y < 0 || x >= FB_WIDTH || y >= FB_HEIGHT) return;
    fb[y * FB_STRIDE_PIX + x] = color;
}

void fb_clear(uint32_t color) {
    for (int y = 0; y < FB_HEIGHT; y++) {
        for (int x = 0; x < FB_STRIDE_PIX; x++) {
            fb[y * FB_STRIDE_PIX + x] = color;
        }
    }
}

void fb_fill_rect(int x, int y, int w, int h, uint32_t color) {
    for (int j = 0; j < h; j++) {
        int yy = y + j;
        if (yy < 0 || yy >= FB_HEIGHT) continue;
        for (int i = 0; i < w; i++) {
            int xx = x + i;
            if (xx < 0 || xx >= FB_WIDTH) continue;
            fb[yy * FB_STRIDE_PIX + xx] = color;
        }
    }
}

void fb_draw_char(int x, int y, char c, uint32_t fg, uint32_t bg) {
    const uint8_t *glyph = font8x8_basic[(unsigned char)c];

    for (int row = 0; row < 8; row++) {
        uint8_t bits = glyph[row];
        for (int col = 0; col < 8; col++) {
            uint32_t color = (bits & (1 << (7 - col))) ? fg : bg;
            fb_put_pixel(x + col, y + row, color);
        }
    }
}

void fb_draw_string(int x, int y, const char *s, uint32_t fg, uint32_t bg) {
    int cx = x;
    while (*s) {
        if (*s == '\n') {
            y += 8;
            cx = x;
        } else {
            fb_draw_char(cx, y, *s, fg, bg);
            cx += 8;
        }
        s++;
    }
}
