
#include "console.h"

static size_t cursor_x;
static size_t cursor_y;

static uint8_t console_color;

static uint16_t* console_vram = (uint16_t*) VGA_MEMORY;

void console_init(void) {
    cursor_x = cursor_y = 0;
    console_color = vga_entry_color(CONSOLE_DEFAULT_FG, CONSOLE_DEFAULT_BG);

    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t offset = y * VGA_WIDTH + x;

            console_vram[offset] = vga_entry(' ', console_color);
        }
    }
}

void console_putchar(char c) {
    /* Check if c is a control character */
    switch (c) {
        case '\n':
        cursor_x = 0;
        cursor_y++;
        
        break;

        /*
        The other ones here

        ...
        */

        default:
        /* Check if c is a printable character */
        if (c > 31 && c < 127) {
            const size_t offset = cursor_y * VGA_WIDTH + cursor_x;

            console_vram[offset] = vga_entry(c, console_color);

            if (++cursor_x == VGA_WIDTH) {
                cursor_x = 0;

                if (++cursor_y == VGA_HEIGHT)
                    cursor_y = 0;
            }
        }

        break;
    }
}

void console_write(const char* str) {
    for (size_t i = 0; str[i]; i++)
        console_putchar(str[i]);
}
