
#include "keyboard.h"

static keyboard_callback_t keyboard_callback;

void keyboard_set_callback(keyboard_callback_t callback) {
    keyboard_callback = callback;
}

void keyboard_handler(void) {
    uint8_t scancode = io_inb(KEYBOARD_PS2_DATA);
    
    if (scancode < sizeof(keyboard_scancode_ascii)) {
        /* Do something with keyboard_scancode_ascii[scancode] */
    }

    pic_irq_eoi(KEYBOARD_PS2_IRQ);
}
