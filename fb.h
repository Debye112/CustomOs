// fb.h
#pragma once

#include <stdint.h>

#define FB_BASE   0x3f800000UL
#define FB_WIDTH  1920
#define FB_HEIGHT 1080
#define FB_STRIDE_BYTES 3840
#define FB_STRIDE_PIX   (FB_STRIDE_BYTES / 4)

void fb_clear(uint32_t color);
void fb_fill_rect(int x, int y, int w, int h, uint32_t color);
void fb_put_pixel(int x, int y, uint32_t color);
void fb_draw_char(int x, int y, char c, uint32_t fg, uint32_t bg);
void fb_draw_string(int x, int y, const char *s, uint32_t fg, uint32_t bg);
