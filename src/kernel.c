

#include "console.h"

void kernel_main(void) {
    console_init();

    console_write("Hello, world!");
}
