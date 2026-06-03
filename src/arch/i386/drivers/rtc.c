
#include "rtc.h"

static rtc_callback_t rtc_callback;
static bool rtc_nmi;

uint8_t rtc_get_register(uint8_t register_cmos) {
    io_outb(RTC_CMOS, (!rtc_nmi << 7) | register_cmos);
    io_delay();

    return io_inb(RTC_CMOS_DATA);
}

void rtc_set_register(uint8_t register_cmos, uint8_t value) {
    io_outb(RTC_CMOS, (!rtc_nmi << 7) | register_cmos);
    io_delay();

    io_outb(RTC_CMOS_DATA, value);
}

void rtc_set_nmi(bool nmi) {
    rtc_nmi = nmi;

    rtc_get_register(RTC_CMOS_STATUS_C);
}

datetime_t rtc_get_datetime(void) {
    rtc_set_nmi(0);

    datetime_t datetime = {
        .date = {
            .year   = rtc_get_register(RTC_CMOS_YEAR),
            .month  = rtc_get_register(RTC_CMOS_MONTH),
            .day    = rtc_get_register(RTC_CMOS_DAY),
        },

        .time = {
            .hour   = rtc_get_register(RTC_CMOS_HOUR),
            .minute = rtc_get_register(RTC_CMOS_MINUTE),
            .second = rtc_get_register(RTC_CMOS_SECOND),
        },
    };

    uint8_t status_b = rtc_get_register(RTC_CMOS_STATUS_B);

    rtc_set_nmi(1);

    if (!(status_b & 0x04)) {
        /* BCD form, convert to binary */
        
        datetime.date.year   =  (datetime.date.year   >> 4) * 10 + (datetime.date.year   & 0x0F);
        datetime.date.month  =  (datetime.date.month  >> 4) * 10 + (datetime.date.month  & 0x0F);
        datetime.date.day    =  (datetime.date.day    >> 4) * 10 + (datetime.date.day    & 0x0F);

        datetime.time.hour   = ((datetime.time.hour   >> 4) * 10 + (datetime.time.hour   & 0x0F))
            | (datetime.time.hour & 0x80); /*  */
        datetime.time.minute =  (datetime.time.minute >> 4) * 10 + (datetime.time.minute & 0x0F);
        datetime.time.second =  (datetime.time.second >> 4) * 10 + (datetime.time.second & 0x0F);
    }

    if (!(status_b & 0x02)) {
        /* 12-hour format, convert to 24 */

        uint8_t post_meridiem = datetime.time.hour & 0x80;

        datetime.time.hour = (datetime.time.hour & 0x7F) % 12;

        if (post_meridiem)
            datetime.time.hour += 12;
    }

    /* Assume 21st century */
    datetime.date.year += 2000;

    return datetime;
}

void rtc_set_callback(rtc_callback_t callback) {
    rtc_callback = callback;
}

void rtc_handler(void) {
    /* Get and discard data in register C */
    rtc_get_register(RTC_CMOS_STATUS_C);

    rtc_callback();

    pic_irq_eoi(RTC_IRQ);
}
