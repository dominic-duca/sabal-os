
#include "console.h"

static size_t cursor_x;
static size_t cursor_y;

static uint8_t console_color;

static uint16_t* console_vram = (uint16_t*) VGA_MEMORY;

static bool console_escape = 0;

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

void console_update_cursor(void) {
    const uint16_t offset = cursor_y * VGA_WIDTH + cursor_x;

    // Set low byte
    io_outb(0x3D4, 0x0F);
	io_outb(0x3D5, (uint8_t) (offset));

    // Set high byte
    io_outb(0x3D4, 0x0E);
	io_outb(0x3D5, (uint8_t) (offset >> 8));
}

void console_putchar(char c) {
    /* Check if c is part of an escape sequence */
    if (console_escape) {
        /* Treat c as a VGA text mode color byte */
        console_color = (uint8_t) c;

        console_escape = 0;
        return;
    }

    switch (c) {
        /* Check if c is a control character */
        case '\x1B':
        console_escape = 1;

        break;

        case '\n':
        cursor_x = 0;
        cursor_y++;
        
        break;

        case '\t':
        cursor_x = (cursor_x / CONSOLE_TAB_WIDTH) * CONSOLE_TAB_WIDTH + CONSOLE_TAB_WIDTH;

        if (cursor_x >= VGA_WIDTH) {
            cursor_x = 0;
            cursor_y++;
        }

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
    
    console_update_cursor();
}

void console_write_centered(const char* str) {
    cursor_x = (VGA_WIDTH - string_len(str)) / 2;

    console_write(str);
}
