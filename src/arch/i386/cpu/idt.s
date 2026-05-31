
.section .text

.extern idt_reg
.global idt_load

idt_load:
    lidt idt_reg

    ret
