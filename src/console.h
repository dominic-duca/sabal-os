
#ifndef CONSOLE_H
#define CONSOLE_H

#include <stddef.h>
#include <stdint.h>

#include "vga.h"
#include "string.h"

#define CONSOLE_DEFAULT_FG VGA_COLOR_YELLOW
#define CONSOLE_DEFAULT_BG VGA_COLOR_BLACK

#define CONSOLE_TAB_WIDTH 8

void console_init(void);

void console_putchar(char c);
void console_write(const char* str);

void console_write_centered(const char* str);

#endif
