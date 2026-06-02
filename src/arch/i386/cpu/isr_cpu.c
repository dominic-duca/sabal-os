
__attribute__((noreturn))
void isr_except_handler(void) {
    while (1) {
        __asm__ volatile ("cli; hlt");
    }
}
