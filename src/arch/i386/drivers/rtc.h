
#ifndef RTC_H
#define RTC_H

#include "../irq/pic.h"

#define RTC_IRQ 8

typedef void (*rtc_callback_t) (void);

void rtc_set_callback(rtc_callback_t callback);

void rtc_handler(void);

#endif
