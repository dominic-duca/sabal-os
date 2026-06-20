
#include "arch/i386/cpu/gdt.h"
#include "arch/i386/cpu/idt.h"

#include "arch/i386/drivers/keyboard.h"
#include "arch/i386/drivers/rtc.h"

#include "arch/i386/pmm.h"

#include "util/datetime.h"

#include "multiboot2.h"

#include "console.h"

extern void isr_stub_keyboard(void);    /* In isr.s */
extern void isr_stub_rtc(void);         /* In isr.s */

extern const char* SABAL_PALMETTO_ASCII;

extern uint8_t _kernel_start[];
extern uint8_t _kernel_end[];

datetime_t kernel_datetime;
bool kernel_datetime_set = 0;

void kernel_keyboard_callback(char key_ascii) {
    console_putchar(key_ascii);
}

void kernel_rtc_callback(void) {
    if (kernel_datetime_set) {
        /* Increment time */
        if (++kernel_datetime.time.second == 60) {
            kernel_datetime.time.second = 0;

            if (++kernel_datetime.time.minute == 60) {
                kernel_datetime.time.minute = 0;

                if (++kernel_datetime.time.hour == 60) {
                    kernel_datetime.time.hour = 0;

                    /* TODO: date.day, date.month, date.year rollover */
                }
            }
        }

    } else {
        kernel_datetime = rtc_get_datetime();
        kernel_datetime_set = 1;

        /* TODO: Use automatic 0 padding (handled inside printf) */
        /*
        console_printf("%d %s %d %c%d:%c%d:%c%d (UTC)\n",
            kernel_datetime.date.day, datetime_months[kernel_datetime.date.month], kernel_datetime.date.year,
            (kernel_datetime.time.hour < 10)   ? '0' : '\0', kernel_datetime.time.hour,
            (kernel_datetime.time.minute < 10) ? '0' : '\0', kernel_datetime.time.minute,
            (kernel_datetime.time.second < 10) ? '0' : '\0', kernel_datetime.time.second
        );
        */
    }
}

void kernel_irq_init(void) {
    /* PS/2 keyboard */
    idt_insert_entry(IDT_EXCEPT_LIMIT + KEYBOARD_PS2_IRQ,
        idt_entry(
            (uint32_t) isr_stub_keyboard,

            GDT_KERNEL_CODE,

            IDT_ENTRY_TYPE_INT,
            IDT_ENTRY_DPL_0
        )
    );

    /* RTC */
    idt_insert_entry(IDT_EXCEPT_LIMIT + RTC_IRQ,
        idt_entry(
            (uint32_t) isr_stub_rtc,
            
            GDT_KERNEL_CODE,

            IDT_ENTRY_TYPE_INT,
            IDT_ENTRY_DPL_0
        )
    );
}

void kernel_main(uint32_t multiboot_magic, uint32_t multiboot_info) {
    __asm__ volatile ("cli");
    
    if (multiboot_magic != MULTIBOOT_2_MAGIC) {
        /* Kernel not loaded by a Multiboot 2 compliant bootloader */
        return;
    }
    
    gdt_init();
    
    multiboot_info_parsed_t multiboot_info_parsed = multiboot_parse_info((multiboot_info_t*) multiboot_info);

    pmm_init();
    for (int i = 0; i < multiboot_info_parsed.mmap.size; i++) {
        const uint64_t base = multiboot_info_parsed.mmap.map[i].base,
                       len  = multiboot_info_parsed.mmap.map[i].len;
        
        const uint64_t base_aligned = bit_ceil(base, PMM_PAGE_SIZE),
                       end_aligned  = bit_floor(base + len, PMM_PAGE_SIZE);

        if (base_aligned >= end_aligned)
            continue; 

        pmm_free_pages((uint32_t) base_aligned / PMM_PAGE_SIZE,
            (uint32_t) (end_aligned - base_aligned) / PMM_PAGE_SIZE);
    }

    idt_init();

    kernel_irq_init();

    keyboard_set_callback(kernel_keyboard_callback);
    rtc_set_callback(kernel_rtc_callback);

    rtc_set_tick(1);

    pic_remap(IDT_EXCEPT_LIMIT, IDT_EXCEPT_LIMIT + 8);

    pic_irq_unmask(KEYBOARD_PS2_IRQ);
    pic_irq_unmask(RTC_IRQ);

    __asm__ volatile ("sti");

    console_init();
    console_printf("\nWelcome to SabalOS!\n");

    console_printf("Available memory: %d MiB\n", (int) (pmm_get_free_count() * PMM_PAGE_SIZE / (1024 * 1024)));

    while (1) {}
}
