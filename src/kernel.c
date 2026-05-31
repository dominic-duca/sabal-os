
#include "arch/i386/cpu/gdt.h"
#include "arch/i386/cpu/idt.h"

#include "arch/i386/drivers/keyboard.h"

#include "console.h"

extern const char* SABAL_PALMETTO_ASCII;

void kernel_keyboard_callback(char key_ascii) {
    console_putchar(key_ascii);
}

void kernel_main(void) {
    __asm__ volatile ("cli");

    gdt_init();
    idt_init();

    keyboard_set_callback(kernel_keyboard_callback);

    __asm__ volatile ("sti");

    console_init();
    console_write_centered("\nWelcome to Sabal OS!\n");
}
