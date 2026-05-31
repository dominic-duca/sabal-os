
#include "pic.h"

void pic_remap(int offset_primary, int offset_secondary) {
    /* Start init sequence */
    io_outb(PIC_1_COMMAND, PIC_INIT);
    io_delay();
    
    io_outb(PIC_2_COMMAND, PIC_INIT);
    io_delay();

    /* Set vector offsets */
    io_outb(PIC_1_DATA, offset_primary);
    io_delay();

    io_outb(PIC_2_DATA, offset_secondary);
    io_delay();

    /* Set cascade */
    io_outb(PIC_1_DATA, 1 << PIC_IRQ_CASCADE);
    io_delay();

    io_outb(PIC_2_DATA, PIC_IRQ_CASCADE);
    io_delay();

    /* Set 8086 modes */
    io_outb(PIC_1_DATA, PIC_MODE_8086);
    io_delay();

    io_outb(PIC_2_DATA, PIC_MODE_8086);
    io_delay();

    /* Mask all IRQs */
    io_outb(PIC_1_DATA, 0xFF);
    io_outb(PIC_2_DATA, 0xFF);
}

void pic_irq_mask(uint8_t irq) {
    uint8_t port;

    if (irq < 8) {
        port = PIC_1_DATA;

    } else {
        port = PIC_2_DATA;
        irq -= 8;
    }
    
    io_outb(port, io_inb(port) | (1 << irq));
}

void pic_irq_unmask(uint8_t irq) {
    uint8_t port;

    if (irq < 8) {
        port = PIC_1_DATA;

    } else {
        port = PIC_2_DATA;
        irq -= 8;
    }
    
    io_outb(port, io_inb(port) & ~(1 << irq));
}
