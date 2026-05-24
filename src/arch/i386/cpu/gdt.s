
.extern gdt_reg
.global gdt_load

.text
gdt_load:
    lgdt gdt_reg
    
    ljmp $0x08, $reload_segments

reload_segments:
    movw $0x10, %ax

    movw %ax,   %ds
    movw %ax,   %es
    movw %ax,   %fs
    movw %ax,   %gs
    movw %ax,   %ss

    ret
