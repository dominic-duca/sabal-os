
#include "vmm.h"

static uint32_t vmm_dir[1024]   __attribute__((aligned(4096)));
static uint32_t vmm_table[1024] __attribute__((aligned(4096)));

void vmm_dir_insert(uint16_t index, uint32_t entry) {
    vmm_dir[index] = entry;
}

void vmm_init(uint32_t kernel_start, uint32_t kernel_end) {
    for (size_t i = 0; i < 1024; i++) {
        vmm_dir[i] = 0x00000000;
    }

    kernel_start = bit_floor(kernel_start, PMM_PAGE_SIZE);
    kernel_end   = bit_ceil(kernel_end, PMM_PAGE_SIZE);

    pmm_alloc_pages(kernel_start / PMM_PAGE_SIZE, (kernel_end - kernel_start) / PMM_PAGE_SIZE);
}
