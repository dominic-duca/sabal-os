
#include "arch/i386/cpu/gdt.h"
#include "arch/i386/cpu/idt.h"

#include "arch/i386/drivers/keyboard.h"
#include "arch/i386/drivers/rtc.h"

#include "util/datetime.h"

#include "console.h"

extern void isr_stub_keyboard(void);    /* In isr.s */
extern void isr_stub_rtc(void);         /* In isr.s */

extern const char* SABAL_PALMETTO_ASCII;

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

void kernel_main(void) {
    __asm__ volatile ("cli");

    gdt_init();
    idt_init();

    kernel_idt_init(); /* Find a better name */

    keyboard_set_callback(kernel_keyboard_callback);
    rtc_set_callback(kernel_rtc_callback);

    rtc_set_tick(1);

    pic_remap(IDT_EXCEPT_LIMIT, IDT_EXCEPT_LIMIT + 8);

    pic_irq_unmask(KEYBOARD_PS2_IRQ);
    pic_irq_unmask(RTC_IRQ);

    __asm__ volatile ("sti");

    console_init();
    console_write_centered("\nWelcome to Sabal OS!\n");

    while (1) {}
}
