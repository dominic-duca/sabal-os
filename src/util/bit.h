
#ifndef BIT_H
#define BIT_H

static inline unsigned int bit_align_up(unsigned int a, unsigned int b) {
    return (a + (b - 1)) & ~(b - 1);
}

#endif
