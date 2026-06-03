
#ifndef RTC_H
#define RTC_H

#include "../irq/pic.h"

#include "../../../util/datetime.h"

#define RTC_CMOS        0x70
#define RTC_CMOS_DATA   0x71

#define RTC_CMOS_STATUS_A 0x0A
#define RTC_CMOS_STATUS_B 0x0B
#define RTC_CMOS_STATUS_C 0x0C

#define RTC_CMOS_YEAR   0x09
#define RTC_CMOS_MONTH  0x08
#define RTC_CMOS_DAY    0x07

#define RTC_CMOS_HOUR   0x04
#define RTC_CMOS_MINUTE 0x02
#define RTC_CMOS_SECOND 0x00

#define RTC_IRQ 8

typedef void (*rtc_callback_t) (void);

datetime_t rtc_get_datetime(void);

void rtc_set_callback(rtc_callback_t callback);

uint8_t rtc_get_register(uint8_t register_cmos);

void rtc_handler(void);

#endif
