
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../irq/pic.h"

#define KEYBOARD_PS2_DATA   0x60

#define KEYBOARD_PS2_IRQ    0x01

static const char keyboard_scancode_ascii[] = {
    0, '\e', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' ', 0,
    /* ... function keys and more here */
};

void keyboard_handler(void);

#endif
