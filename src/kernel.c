
#include "arch/i386/cpu/gdt.h"
#include "arch/i386/cpu/idt.h"

#include "console.h"

extern const char* SABAL_PALMETTO_ASCII;

void kernel_main(void) {
    __asm__ volatile ("cli");

    gdt_init();
    idt_init();

    console_init();
    console_write_centered("\nWelcome to Sabal OS!\n");
}
