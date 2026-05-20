
#ifndef IDT_H
#define IDT_H

#include <stdint.h>

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
    uint8_t type_attrib; /* Type, DPL, P */

    uint16_t service_u; /* ISR */

} __attribute__ ((packed)) idt_gate_t;

#endif
