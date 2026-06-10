
#ifndef PMM_H
#define PMM_H

#include <stdint.h>

#include "../../util/bit.h"

/* (32768 * 32) pages * 4 KiB/page = 4 GiB */
#define PMM_BITMAP_LIMIT 32768

void pmm_init(void);

void pmm_free_page(uint32_t page);
void pmm_free_pages(uint32_t begin, uint32_t end);

#endif
