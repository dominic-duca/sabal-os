
#include "gdt.h"

static gdt_reg_t gdt_reg;

static gdt_seg_t gdt[GDT_ENTRY_LIMIT];
static size_t gdt_size;

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

void gdt_push_entry(gdt_seg_t entry) {
    gdt[gdt_size++] = entry;
}

void gdt_update_reg(void) {
    gdt_reg.base = (uint32_t) gdt;
    gdt_reg.limit = (uint16_t) (sizeof(gdt_seg_t) * gdt_size - 1);
}

