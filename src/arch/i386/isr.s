
.extern keyboard_handler /* In drivers/keyboard.h */
.extern rtc_handler      /* In drivers/rtc.h */

.global isr_stubs

.global isr_stub_keyboard
.global isr_stub_rtc

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

isr_stub_rtc:
    pushal
    cld

    call rtc_handler

    popal

    iret
