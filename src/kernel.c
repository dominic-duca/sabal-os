
#include "arch/i386/gdt.h"
#include "arch/i386/idt.h"

#include "console.h"

extern const char* SABAL_PALMETTO_ASCII;

void kernel_main(void) {
    gdt_init();
    idt_init();
    
    console_init();
    console_write_centered("\nWelcome to Sabal OS!\n");
}
