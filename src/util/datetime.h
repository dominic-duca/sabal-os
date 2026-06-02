
#ifndef DATETIME_H
#define DATETIME_H

#include <stdint.h>

typedef struct {
    uint16_t year;
    uint8_t month;
    uint8_t day;

} date_t;

typedef struct {
    uint8_t hour;
    uint8_t minute;
    uint8_t second;

} time_t;

typedef struct {
    date_t date;
    time_t time;
    
} datetime_t;

#endif
