
#include "string.h"

size_t string_len(const char* str)  {
	size_t len = 0;
	while (str[len])
        len++;

	return len;
}
