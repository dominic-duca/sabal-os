
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "vga.h"
#include "string.h"

#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  0xB8000

size_t terminal_row, terminal_column;
uint8_t terminal_color;

uint16_t* terminal_buffer = (uint16_t*) VGA_MEMORY;

void terminal_initialize(void) {
    terminal_row = terminal_color = 0;

    terminal_color = vga_entry_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK);

    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t offset = y * VGA_WIDTH + x;

            terminal_buffer[offset] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_putchar(char c) {
    const size_t offset = terminal_row * VGA_WIDTH + terminal_column;
    terminal_buffer[offset] = vga_entry(c, terminal_color);

    if (++terminal_column == VGA_WIDTH) {
        terminal_column = 0;

        if (++terminal_row == VGA_HEIGHT)
            terminal_row = 0;
    }
}

void terminal_write(const char* data) {
    const size_t size = string_len(data);

    for (size_t i = 0; i < size; i++)
        terminal_putchar(data[i]);
}

void kernel_main(void) {
    terminal_initialize();

    terminal_write("Hello, World!");
}
