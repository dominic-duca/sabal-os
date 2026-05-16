
#include "gdt.h"

gdt_seg_t gdt_entry(uint32_t limit, uint32_t base, uint8_t access, uint8_t flags) {
    gdt_seg_t entry;

    /* Lower dword */
    entry.limit_l = (uint16_t) limit;
    entry.base_l = (uint16_t) base;

    /* Upper dword */
    entry.base_m = (uint8_t) (base >> 16);
    
    entry.access = access;

    entry.limit_u_flags = (flags << 4) | ((limit >> 16) & 0xF);
    entry.base_u = (uint8_t) (base >> 24);

    return entry;
}
