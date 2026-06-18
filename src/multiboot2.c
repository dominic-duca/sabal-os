
#include "multiboot2.h"

multiboot_info_parsed_t multiboot_parse_info(multiboot_info_t* info) {
    multiboot_info_parsed_t parsed = {
        .mmap.size = 0,
    };
    
    /* Step through each tag in multiboot_info (the first tag is already 8-byte aligned) */
    for (multiboot_tag_t* tag = ((multiboot_info_t*) info)->tags; tag->type != MULTIBOOT_TAG_TYPE_END;
        tag = (multiboot_tag_t*) ((uint8_t*) tag + bit_ceil(tag->size, 8))) {
        
        switch(tag->type) {
            case MULTIBOOT_TAG_TYPE_MMAP: {
                multiboot_tag_mmap_t* tag_mmap = (multiboot_tag_mmap_t*) tag;

                /* Step through each entry in tag_mmap */
                for (multiboot_mmap_entry_t* mmap_entry = tag_mmap->entries; mmap_entry < (multiboot_mmap_entry_t*) ((uint8_t*) tag_mmap + tag_mmap->size);
                    mmap_entry = (multiboot_mmap_entry_t*) (((uint8_t*) mmap_entry) + tag_mmap->entry_size)) {
                    
                    switch (mmap_entry->type) {
                        case MULTIBOOT_MEMORY_FREE: {
                            parsed.mmap.map[parsed.mmap.size].base  = mmap_entry->base;
                            parsed.mmap.map[parsed.mmap.size].len   = mmap_entry->len;

                            parsed.mmap.size++;

                            break;
                        }
                    }
                }
                break;
            }
        }
    }

    return parsed;
}
