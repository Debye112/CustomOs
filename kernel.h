#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>

// Basic typedefs
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;

// -----------------------------
// UART API (redirected to HDMI)
// -----------------------------
// These functions will be implemented in console.c
void uart_init(void);
void uart_putc(char c);
void uart_puts(const char *s);
char uart_getc(void);
void uart_gets(char *buf);

// -----------------------------
// String functions
// -----------------------------
int strlen(char *resp);
int nameBoundCheck(char *name);
int stringcom(char *comref, int s_comref, char *com, int s_com);

// -----------------------------
// Filesystem structures
// -----------------------------
#define MAX_INPUT 128

struct nodes {
    char nodeName[32];
    int  size;
    int  layer;
    int  parentIndex;
};

struct commandInputs {
    char command[32];
    int  commandSize;
    char name[32];
    int  nameSize;
};

// Globals
extern int currentDirectory;
extern int poolIndex;
extern int poolNumber;
extern struct nodes nodePool[128];

// FS functions
void newDirectory(char *name);
void list(void);
void commandDisecter(struct commandInputs *c, char *input);
void changeDirect(char *name);
void removeDirectory(char *name);
int  copyDirectory(char *name);
void pasteDirectory(int poolNumber);
int  commands(char *input);

// -----------------------------
// Framebuffer API
// -----------------------------
void fb_putpixel(int x, int y, unsigned int color);
void fb_draw_char(int x, int y, char c, uint32_t fg, uint32_t bg);
void fb_draw_string(int x, int y, const char *s, uint32_t fg, uint32_t bg);
void fb_clear(uint32_t color);
void fb_fill_rect(int x, int y, int w, int h, uint32_t color);

// -----------------------------
// Kernel entry
// -----------------------------
void kmain(void);

#endif
