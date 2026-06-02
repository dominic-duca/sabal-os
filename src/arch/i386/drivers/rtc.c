
#include "rtc.h"

static rtc_callback_t rtc_callback;

void rtc_set_callback(rtc_callback_t callback) {
    rtc_callback = callback;
}

void rtc_handler(void) {
    rtc_callback();

    pic_irq_eoi(RTC_IRQ);
}
