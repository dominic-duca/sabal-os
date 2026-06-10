
#ifndef MULTIBOOT2_H
#define MULTIBOOT2_H

#include <stdint.h>

#include "util/bit.h"

#define MULTIBOOT_2_MAGIC       0x36D76289

#define MULTIBOOT_TAG_TYPE_END  0x00
#define MULTIBOOT_TAG_TYPE_MMAP 0x06

#define MULTIBOOT_MEMORY_FREE   0x01

#define MULTIBOOT_MMAP_PARSED_LIMIT 256

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

/* PARSED STRUCTS */

typedef struct {
    uint64_t base;
    uint64_t limit;

} multiboot_memory_parsed_t;

typedef struct {
    uint16_t limit;
    multiboot_memory_parsed_t map[MULTIBOOT_MMAP_PARSED_LIMIT];

} multiboot_mmap_parsed_t;

typedef struct {
    multiboot_mmap_parsed_t mmap;

} multiboot_info_parsed_t;

multiboot_info_parsed_t multiboot_parse_info(multiboot_info_t* info);

#endif
