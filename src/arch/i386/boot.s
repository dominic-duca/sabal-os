
/* Multiboot 2 constants */
.set MULTIBOOT_MAGIC, 0xE85250D6
.set MULTIBOOT_ARCH,  0
.set MULTIBOOT_LEN,   multiboot_end - multiboot_start

.set MULTIBOOT_SUM,   -(MULTIBOOT_MAGIC + MULTIBOOT_ARCH + MULTIBOOT_LEN)

/* Multiboot 2 header */
.section .multiboot
.align 8
multiboot_start:
    .long MULTIBOOT_MAGIC
    .long MULTIBOOT_ARCH
    .long MULTIBOOT_LEN

    .long MULTIBOOT_SUM

    /* End tag */
    .short 0
    .short 0
    .long  8

multiboot_end:

/* Allocate stack */
.section .bss
.align 16

stack_bottom:
.skip 16384 # 16 KiB

stack_top:
.section .text

.global _start
.type _start, @function

_start:
    mov $stack_top, %esp

    push %ebx /* Info struct pointer (Multiboot 2) */ 
    push %eax /* Magic (Multiboot 2) */

    call kernel_main

    /* Halt indefinitely if kernel_main ever returns */
    cli
1:
    hlt
    jmp 1b

.size _start, . - _start # Wish I knew what this meant
