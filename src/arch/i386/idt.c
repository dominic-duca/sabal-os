
#include "idt.h"

static idt_reg_t idt_reg;

static idt_gate_t idt[IDT_ENTRY_LIMIT];
static size_t idt_size;

idt_gate_t idt_entry(uint32_t service, uint16_t segment, uint8_t type, uint8_t dpl) {
    idt_gate_t entry;

    /* Lower dword */
    entry.service_l = (uint16_t) service;
    entry.segment = segment;

    /* Upper dword */
    entry.null = 0x00;

    entry.type_attr = type | (dpl << 5) | (1 << 7);
    entry.service_u = (uint16_t) (service >> 16);

    return entry;
}

void idt_push_entry(idt_gate_t entry) {
    idt[idt_size++] = entry;
}

void idt_update_reg(void) {
    idt_reg.base = (uint32_t) idt;
    idt_reg.limit = (uint16_t) (sizeof(idt_gate_t) * idt_size - 1);
}

void idt_init(void) {
    idt_size = 0;

    /* Exception entries */
    for (uint8_t vector = 0; vector < IDT_EXCEPT_LIMIT; vector++) {
        idt_push_entry(idt_entry(
            (uint32_t) isr_stubs[vector],
            
            GDT_KERNEL_CODE,

            IDT_ENTRY_TYPE_INT,
            IDT_ENTRY_DPL_0
        ));
    }

    idt_update_reg();

    __asm__ volatile ("lidt %0" : : "m" (idt_reg) : ); /* Load IDT */
    __asm__ volatile ("sti");
}
