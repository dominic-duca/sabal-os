
#ifndef VMM_H
#define VMM_H

#include <stddef.h>
#include <stdint.h>

#include "pmm.h"

#define VMM_ENTRY_PRESENT 0x01
#define VMM_ENTRY_WRITE   0x02
#define VMM_ENTRY_USER    0x04

static inline uint32_t vmm_entry(uint32_t addr, uint16_t flags) {
    return (addr & 0xFFFFF000) | (flags & 0x0FFF);
}

void vmm_init(uint32_t kernel_start, uint32_t kernel_end);

void vmm_dir_insert(uint16_t index, uint32_t entry);

#endif
