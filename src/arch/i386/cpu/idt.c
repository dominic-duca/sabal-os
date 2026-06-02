
#include "idt.h"

idt_reg_t idt_reg;

static idt_gate_t idt[IDT_ENTRY_LIMIT];

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

void idt_insert_entry(uint8_t vector, idt_gate_t entry) {
    idt[vector] = entry;
}

void idt_init(void) {
    /* IDTR */
    idt_reg.base = (uint32_t) idt;
    idt_reg.limit = (uint16_t) (sizeof(idt_gate_t) * IDT_ENTRY_LIMIT - 1);

    /* Exception entries */
    for (uint8_t vector = 0; vector < IDT_EXCEPT_LIMIT; vector++) {
        idt_insert_entry(vector, idt_entry(
            (uint32_t) isr_stubs[vector],
            
            GDT_KERNEL_CODE,

            IDT_ENTRY_TYPE_INT,
            IDT_ENTRY_DPL_0
        ));
    }

    idt_load();
}
