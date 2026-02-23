#include <stdint.h>
#include "fb.h"

void kmain(void) {
    fb_clear(0x00000020); // dark background
    fb_fill_rect(50, 50, 300, 150, 0x000000FF); // blue box

    fb_draw_string(60, 60, "AAAAAA", 0x00FFFFFF, 0x000000FF);

    while (1) { }
}

