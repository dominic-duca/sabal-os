
#ifndef PIC_H
#define PIC_H

#include "../../../io.h"

#define PIC_1_COMMAND   0x20
#define PIC_1_DATA      0x21       

#define PIC_2_COMMAND   0xA0
#define PIC_2_DATA      0xA1

#define PIC_MODE_8086   0x01

#define PIC_IRQ_CASCADE 0x02

#define PIC_INIT        0x11

void pic_remap(int offset_primary, int offset_secondary);

void pic_irq_mask(uint8_t irq);
void pic_irq_unmask(uint8_t irq);

#endif
