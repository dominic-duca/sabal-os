
#include "pmm.h"

static uint32_t pmm_bitmap[PMM_BITMAP_LIMIT];

void pmm_init(void) {
    for (int i = 0; i < PMM_BITMAP_LIMIT; i++) {
        pmm_bitmap[i] = 0xFFFFFFFF;
    }
}

void pmm_free_page(uint32_t page) {
    pmm_bitmap[page / 32] &= ~((uint32_t) 0x01 << (page % 32));
}

void pmm_free_pages(uint32_t begin, uint32_t end) {
    /* Free individual pages */
    for (uint32_t page = begin; page < bit_ceil(begin, 32); page++) {
        pmm_free_page(page);
    }

    /* Free in chunks of 32 */
    for (uint32_t i_bitmap = bit_ceil(begin, 32) / 32; i_bitmap < bit_floor(end, 32) / 32; i_bitmap++) {
        pmm_bitmap[i_bitmap] = 0x00;
    }

    /* Free individual pages */
    for (uint32_t page = bit_floor(end, 32); page <= end; page++) {
        pmm_free_page(page);
    }
}
