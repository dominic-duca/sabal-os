
#include "vmm.h"

static uint32_t vmm_dir[1024] __attribute__((aligned(4096)));
static size_t   vmm_dir_size;

void vmm_init(void) {
    for (size_t i = 0; i < 1024; i++) {
        vmm_dir[i] = 0x00000000;
    }
    
    vmm_dir_size = 0;
}
