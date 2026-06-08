
#include "arch/i386/cpu/gdt.h"
#include "arch/i386/cpu/idt.h"

#include "arch/i386/drivers/keyboard.h"
#include "arch/i386/drivers/rtc.h"

#include "util/datetime.h"

#include "console.h"

#define MULTIBOOT_2_MAGIC   0x36D76289

#define MULTIBOOT_TAG_END   0x00
#define MULTIBOOT_TAG_MMAP  0x06

#define MULTIBOOT_MMAP_FREE 0x01

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

extern void isr_stub_keyboard(void);    /* In isr.s */
extern void isr_stub_rtc(void);         /* In isr.s */

extern const char* SABAL_PALMETTO_ASCII;

datetime_t kernel_datetime;
bool kernel_datetime_set = 0;

/* TEST */
static const char* date_months[] = {
    "January", "February", "March", "April", "May", "June",
    "July", "August", "September","October", "November", "December",
};

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
        console_printf("%d %s %d %c%d:%c%d:%c%d (UTC)\n",
            kernel_datetime.date.day, date_months[kernel_datetime.date.month - 1], kernel_datetime.date.year,
            (kernel_datetime.time.hour < 10)   ? '0' : '\0', kernel_datetime.time.hour,
            (kernel_datetime.time.minute < 10) ? '0' : '\0', kernel_datetime.time.minute,
            (kernel_datetime.time.second < 10) ? '0' : '\0', kernel_datetime.time.second
        );
    }
}

void kernel_idt_init(void) {
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

inline int math_align_up(int a, int b) {
    return (a + (b - 1)) & ~(b - 1);
}

void kernel_main(uint32_t multiboot_magic, uint32_t multiboot_info) {
    __asm__ volatile ("cli");
    
    if (multiboot_magic != MULTIBOOT_2_MAGIC) {
        /* Kernel not loaded by a Multiboot 2 compliant bootloader */
        return;
    }
    
    gdt_init();
    idt_init();

    kernel_idt_init(); /* TODO: find a better name */

    keyboard_set_callback(kernel_keyboard_callback);
    rtc_set_callback(kernel_rtc_callback);

    rtc_set_tick(1);

    pic_remap(IDT_EXCEPT_LIMIT, IDT_EXCEPT_LIMIT + 8);

    pic_irq_unmask(KEYBOARD_PS2_IRQ);
    pic_irq_unmask(RTC_IRQ);

    __asm__ volatile ("sti");

    console_init();
    console_printf("\nWelcome to SabalOS!\n");

    while (1) {}
}
