

#include "console.h"

extern const char* SABAL_PALMETTO_ASCII;

void kernel_main(void) {
    console_init();

    console_write("Welcome to Sabal OS!\n\n");
    console_write(SABAL_PALMETTO_ASCII);
}
