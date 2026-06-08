
#ifndef MULTIBOOT2_H
#define MULTIBOOT2_H

#include <stdint.h>

#include "util/bit.h"

#define MULTIBOOT_2_MAGIC        0x36D76289

#define MULTIBOOT_TAG_TYPE_END   0x00
#define MULTIBOOT_TAG_TYPE_MMAP  0x06

#define MULTIBOOT_MEMORY_FREE    0x01

typedef struct {
    uint32_t type;
    uint32_t limit;

} multiboot_tag_t;

typedef struct {
    uint32_t limit;
    uint32_t null;

    multiboot_tag_t tags[];

} multiboot_info_t;

typedef struct {
    uint64_t base;
    uint64_t limit; /* Size of the memory region in bytes */
    uint32_t type;
    uint32_t null;

} multiboot_mmap_entry_t;

typedef struct {
    uint32_t type;  /* 0x06 */
    uint32_t limit;

    uint32_t limit_entry;
    uint32_t version_entry;

    multiboot_mmap_entry_t entries[];

} multiboot_tag_mmap_t;

void multiboot_parse_info(multiboot_info_t* info);

#endif
