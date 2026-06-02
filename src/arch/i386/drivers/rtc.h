
#ifndef RTC_H
#define RTC_H

#include "../irq/pic.h"

#include "../../../util/datetime.h"

#define RTC_IRQ 8

typedef void (*rtc_callback_t) (void);

datetime_t rtc_get_datetime(void);

void rtc_set_callback(rtc_callback_t callback);

uint8_t rtc_get_register(uint8_t register_cmos);

void rtc_handler(void);

#endif
