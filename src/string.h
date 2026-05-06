
#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

size_t string_len(const char* str);

void string_rev(char str[], size_t len);

void string_itoa(int a, char str[], int base);

void string_itoa_10(int a, char str[]);

#endif
