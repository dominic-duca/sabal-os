
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

/* TEST */
static const char* date_months[] = {
    "January", "February", "March", "April", "May", "June",
    "July", "August", "September","October", "November", "December",
};

/* TEST */
void time_add_leading_zero(char time[]) {
    time[2] = '\0';
    time[1] = time[0];
    time[0] = '0';
}

/* TEST */
void kernel_output_datetime() {
    char year[3], day[3], hour[3], minute[3], second[3];

    console_write(string_itoa_10(kernel_datetime.date.day, day));
    console_putchar(' ');
    console_write(date_months[kernel_datetime.date.month - 1]);
    console_putchar(' ');
    console_write(string_itoa_10(kernel_datetime.date.year, year));
    console_putchar('\t');

    string_itoa_10(kernel_datetime.time.hour, hour);
    if (kernel_datetime.time.hour < 10)
        time_add_leading_zero(hour);
    
    console_write(hour);
    console_putchar(':');

    string_itoa_10(kernel_datetime.time.minute, minute);
    if (kernel_datetime.time.minute < 10)
        time_add_leading_zero(minute);
    
    console_write(minute);
    console_putchar(':');
    
    string_itoa_10(kernel_datetime.time.second, second);
    if (kernel_datetime.time.second < 10)
        time_add_leading_zero(second);

    console_write(second);
    console_write(" (UTC)\n");
}

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

    kernel_output_datetime(); /* TEST: See if kernel_datetime is correct (UTC) */
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
