

#include "console.h"

extern const char* SABAL_PALMETTO_ASCII;

void kernel_main(void) {
    console_init();

    console_write("\n");
    console_write_centered("Welcome to Sabal OS!");
    
    console_write("\n\n");
    console_write(SABAL_PALMETTO_ASCII);
}
