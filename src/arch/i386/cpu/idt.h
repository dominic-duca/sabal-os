
#ifndef IDT_H
#define IDT_H

#include <stddef.h>
#include <stdint.h>

#include "gdt.h"

#define IDT_ENTRY_LIMIT     256
#define IDT_EXCEPT_LIMIT    32

#define IDT_ENTRY_TYPE_INT  0x0E
#define IDT_ENTRY_TYPE_TRAP 0x0F

#define IDT_ENTRY_DPL_0     0

extern void* isr_stubs[]; /* In isr.s */

typedef struct {
    uint16_t limit; /* IDT size (in bytes) - 1 */
    uint32_t base;  /* IDT address */

} __attribute__ ((packed)) idt_reg_t;

typedef struct {
    /* Lower dword */
    uint16_t service_l; /* ISR */
    uint16_t segment; /* Segment selector */

    /* Upper dword */
    uint8_t null;
    uint8_t type_attr; /* Type, DPL, P */

    uint16_t service_u; /* ISR */

} __attribute__ ((packed)) idt_gate_t;

void idt_init(void);

void idt_update_reg(void);

idt_gate_t idt_entry(uint32_t service, uint16_t segment, uint8_t type, uint8_t dpl);
void idt_push_entry(idt_gate_t entry);

#endif
