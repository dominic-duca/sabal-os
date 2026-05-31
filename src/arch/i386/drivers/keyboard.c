
#include "keyboard.h"

void keyboard_handler(void) {
    uint8_t scancode = io_inb(KEYBOARD_PS2_DATA);
    
    if (scancode < sizeof(keyboard_scancode_ascii)) {
        /* Do something with keyboard_scancode_ascii[scancode] */
    }

    pic_irq_eoi(KEYBOARD_PS2_IRQ);
}
