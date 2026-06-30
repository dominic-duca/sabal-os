
#include "vmm.h"

static uint32_t vmm_dir[1024]   __attribute__((aligned(4096)));
static uint32_t vmm_table[1024] __attribute__((aligned(4096)));

void vmm_dir_insert(uint16_t index, uint32_t entry) {
    vmm_dir[index] = entry;
}

void vmm_init(uint32_t kernel_start, uint32_t kernel_end, uint32_t v_addr) {
    for (size_t i = 0; i < 1024; i++) {
        vmm_dir[i] = 0x00000000;
        
        vmm_table[i] = 0x00000000;
    }

    kernel_start = bit_floor(kernel_start, PMM_PAGE_SIZE);
    kernel_end   = bit_ceil(kernel_end, PMM_PAGE_SIZE);

    pmm_alloc_pages(kernel_start / PMM_PAGE_SIZE, (kernel_end - kernel_start) / PMM_PAGE_SIZE);

    /* TODO: Handle multiple page tables */
    for (uint32_t i = 0, p_addr = kernel_start; p_addr < kernel_end; i++, p_addr += PMM_PAGE_SIZE) {
        vmm_table[((v_addr >> 12) & 0x3FF) + i] = vmm_entry(p_addr, VMM_ENTRY_WRITE | VMM_ENTRY_PRESENT);
    }

    vmm_dir[v_addr >> 22] = vmm_entry((uint32_t) vmm_table, VMM_ENTRY_WRITE | VMM_ENTRY_PRESENT);
}
