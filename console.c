#include "kernel.h"
#include "fb.h"

static int cursor_col = 0;
static int cursor_row = 0;

static const int CHAR_W = 8;
static const int CHAR_H = 8;

static const int TERM_COLS = FB_WIDTH  / CHAR_W;
static const int TERM_ROWS = FB_HEIGHT / CHAR_H;

static uint32_t fg_color = 0x00FFFFFF;
static uint32_t bg_color = 0x00000020;

// simple blink state
static int blink_counter = 0;
static int blink_state   = 0;

static void terminal_scroll(void) {
    int row_bytes = FB_WIDTH * 4 * CHAR_H;
    uint8_t *base = (uint8_t *)FB_BASE;

    // move all rows up by one text row
    for (int y = 0; y < (TERM_ROWS - 1) * CHAR_H; y++) {
        uint32_t *dst = (uint32_t *)(base + y * FB_WIDTH * 4);
        uint32_t *src = (uint32_t *)(base + (y + CHAR_H) * FB_WIDTH * 4);
        for (int x = 0; x < FB_WIDTH; x++) {
            dst[x] = src[x];
        }
    }

    // clear last row
    for (int y = (TERM_ROWS - 1) * CHAR_H; y < TERM_ROWS * CHAR_H; y++) {
        for (int x = 0; x < FB_WIDTH; x++) {
            fb_putpixel(x, y, bg_color);
        }
    }

    cursor_row = TERM_ROWS - 1;
}

static void terminal_newline(void) {
    cursor_col = 0;
    cursor_row++;
    if (cursor_row >= TERM_ROWS) {
        terminal_scroll();
    }
}

static void terminal_draw_cursor(int on) {
    int x0 = cursor_col * CHAR_W;
    int y0 = cursor_row * CHAR_H;
    uint32_t c = on ? 0x00FFFFFF : bg_color;

    for (int y = 0; y < CHAR_H; y++) {
        for (int x = 0; x < CHAR_W; x++) {
            fb_putpixel(x0 + x, y0 + y, c);
        }
    }
}

void uart_init(void) {
    fb_clear(bg_color);
    cursor_col = 0;
    cursor_row = 0;
    blink_counter = 0;
    blink_state = 0;
}

void uart_putc(char c) {
    // erase cursor before drawing
    terminal_draw_cursor(0);

    if (c == '\n') {
        terminal_newline();
    } else {
        int x = cursor_col * CHAR_W;
        int y = cursor_row * CHAR_H;
        fb_draw_char(x, y, c, fg_color, bg_color);
        cursor_col++;
        if (cursor_col >= TERM_COLS) {
            terminal_newline();
        }
    }

    // redraw cursor
    terminal_draw_cursor(1);
}

void uart_puts(const char *s) {
    while (*s) {
        uart_putc(*s++);
    }
}

// crude blink: call this often from your main loop
void terminal_tick(void) {
    blink_counter++;
    if (blink_counter > 200000) { // tune this
        blink_counter = 0;
        blink_state = !blink_state;
        terminal_draw_cursor(blink_state);
    }
}

#define KEYBUF_SIZE 128
static char keybuf[KEYBUF_SIZE];
static int keybuf_head = 0;
static int keybuf_tail = 0;

static int keybuf_is_empty(void) {
    return keybuf_head == keybuf_tail;
}

static int keybuf_is_full(void) {
    return ((keybuf_head + 1) % KEYBUF_SIZE) == keybuf_tail;
}

void keybuf_push(char c) {
    if (keybuf_is_full()) return;
    keybuf[keybuf_head] = c;
    keybuf_head = (keybuf_head + 1) % KEYBUF_SIZE;
}

char uart_getc(void) {
    // busy-wait for now; USB will feed keybuf_push()
    while (keybuf_is_empty()) {
        terminal_tick(); // keep cursor blinking while waiting
    }
    char c = keybuf[keybuf_tail];
    keybuf_tail = (keybuf_tail + 1) % KEYBUF_SIZE;
    return c;
}

void uart_gets(char *buf) {
    int i = 0;
    while (1) {
        char c = uart_getc();
        if (c == '\r') c = '\n';

        if (c == '\n') {
            uart_putc('\n');
            buf[i] = '\0';
            return;
        } else if (c == '\b') {
            if (i > 0) {
                i--;
                uart_puts("\b \b");
            }
        } else {
            buf[i++] = c;
            uart_putc(c);
            if (i >= MAX_INPUT - 1) {
                buf[i] = '\0';
                return;
            }
        }
    }
}


