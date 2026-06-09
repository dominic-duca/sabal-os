
#include "pmm.h"

static uint32_t pmm_bitmap[PMM_BITMAP_LIMIT];

void pmm_init(void) {
    for (int i = 0; i < PMM_BITMAP_LIMIT; i++) {
        pmm_bitmap[i] = 0xFFFFFFFF;
    }
}
