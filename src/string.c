
#include "string.h"

size_t string_len(const char* str)  {
	size_t len = 0;
	while (str[len])
        len++;

	return len;
}

void string_rev(char str[], size_t len) {
    size_t begin = 0, end = len - 1;

    while (begin < end) {
        const char temp = str[begin];

        str[begin++] = str[end];
        str[end--] = temp;
    }
}

char* string_itoa(int a, char str[], int base) {
    int i = 0;

    if (a == 0) {
        str[i++] = 0;
        str[i] = '\0';

        return str;
    }

    bool negative = 0;
    if (a < 0 && base == 10) {
        negative = 1;
        a = -a;
    }

    while (a != 0) {
        int r = a % base;
        str[i++] = (r < 10) ? r + '0' : (r - 10) + 'a'; 

        a /= base;
    }

    if (negative)
        str[i++] = '-';
    
    str[i] = '\0';

    string_rev(str, i);

    return str;
}

char* string_itoa_10(int a, char str[]) {
    return string_itoa(a, str, 10);
}
