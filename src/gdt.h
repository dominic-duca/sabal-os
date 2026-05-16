
#ifndef GDT_H
#define GDT_H

#include <stdint.h>

typedef struct {
    /* Lower dword */
    uint16_t limit_l;
    uint16_t base_l;

    /* Upper dword */
    uint8_t base_m;
    uint8_t access;
    uint8_t limit_u_flags;
    uint8_t base_u;

} gdt_seg_t __attribute__ ((packed));

#endif
