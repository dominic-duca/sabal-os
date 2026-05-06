
#ifndef CONSOLE_H
#define CONSOLE_H

#include <stddef.h>
#include <stdint.h>

#include "vga.h"
#include "string.h"

#define CONSOLE_DEFAULT_FG VGA_COLOR_YELLOW
#define CONSOLE_DEFAULT_BG VGA_COLOR_BLACK

void console_init(void);

void console_putchar(char c);
void console_write(const char* str);

#endif
