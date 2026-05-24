
.global isr_stubs

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
