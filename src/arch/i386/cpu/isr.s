
.extern keyboard_handler /* In drivers/keyboard.h */

.global isr_stubs
.global isr_stub_keyboard

.altmacro
.macro stub i
    .long isr_stub_\i
.endm

isr_stubs:
.set i, 0
.rept 32
    stub %i
    .set i, i + 1
.endr

isr_stub_keyboard:
    pushal
    cld

    call keyboard_handler

    popal

    iret
