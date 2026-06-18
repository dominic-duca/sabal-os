
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
    uint32_t size;

} __attribute__ ((packed)) multiboot_tag_t;

typedef struct {
    uint32_t size;
    uint32_t null;

    multiboot_tag_t tags[];

} __attribute__ ((packed)) multiboot_info_t;

typedef struct {
    uint64_t base;
    uint64_t len;   /* Size of the memory region in bytes */

    uint32_t type;
    uint32_t null;

} __attribute__ ((packed)) multiboot_mmap_entry_t;

typedef struct {
    uint32_t type;  /* 0x06 */
    uint32_t size;

    uint32_t entry_size;
    uint32_t entry_ver;

    multiboot_mmap_entry_t entries[];

} __attribute__ ((packed)) multiboot_tag_mmap_t;

/* PARSED STRUCTS */

typedef struct {
    uint64_t base;
    uint64_t len;

} multiboot_memory_parsed_t;

typedef struct {
    uint16_t size;
    multiboot_memory_parsed_t map[MULTIBOOT_MMAP_PARSED_LIMIT];

} multiboot_mmap_parsed_t;

typedef struct {
    multiboot_mmap_parsed_t mmap;

} multiboot_info_parsed_t;

multiboot_info_parsed_t multiboot_parse_info(multiboot_info_t* info);

#endif
