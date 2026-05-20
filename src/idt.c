
#include "idt.h"

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
