
#ifndef GDT_H
#define GDT_H

#include <stddef.h>
#include <stdint.h>

#define GDT_ENTRY_LIMIT 8192

#define GDT_KERNEL_CODE 0x08
#define GDT_KERNEL_DATA 0x10

#define GDT_USER_CODE   0x18
#define GDT_USER_DATA   0x20

extern void gdt_load(void); /* In gdt.s */

typedef struct {
    uint16_t limit; /* GDT size (in bytes) - 1 */
    uint32_t base;  /* GDT address */

} __attribute__ ((packed)) gdt_reg_t;

typedef struct {
    /* Lower dword */
    uint16_t limit_l;
    uint16_t base_l;

    /* Upper dword */
    uint8_t base_m;
    uint8_t access;
    uint8_t limit_u_flags;
    uint8_t base_u;

} __attribute__ ((packed)) gdt_seg_t;

void gdt_init(void);

void gdt_update_reg(void);

gdt_seg_t gdt_entry(uint32_t limit, uint32_t base, uint8_t access, uint8_t flags);
void gdt_push_entry(gdt_seg_t entry);

#endif
