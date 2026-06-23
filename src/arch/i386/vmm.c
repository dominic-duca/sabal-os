
#include "vmm.h"

static uint32_t vmm_dir[1024] __attribute__((aligned(4096)));

void vmm_dir_insert(uint16_t index, uint32_t entry) {
    vmm_dir[index] = entry;
}

void vmm_init(void) {
    for (size_t i = 0; i < 1024; i++) {
        vmm_dir[i] = 0x00000000;
    }
}
