
#ifndef IDT_H
#define IDT_H

#include <stdint.h>

typedef struct {
    uint16_t limit; /* IDT size (in bytes) - 1 */
    uint32_t base;  /* IDT address */

} __attribute__ ((packed)) idt_reg_t;

#endif
