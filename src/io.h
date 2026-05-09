
#ifndef IO_H
#define IO_H

#include <stdint.h>

static inline uint8_t io_inb(uint16_t port) {
    uint8_t val;

    /* inb %source, %destination */
    __asm__ volatile ( "inb %w1, %b0" : "=a" (val) : "Nd" (port) : );

    return val;
}

static inline void io_outb(uint16_t port, uint8_t val) {
    /* outb %source, %destination */
    __asm__ volatile ( "outb %b0, %w1" : : "a" (val), "Nd" (port) : );
}

#endif
