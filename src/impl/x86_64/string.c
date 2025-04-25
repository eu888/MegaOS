#include "strings.h"
#include <stddef.h>

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

int strncmp(const char* s1, const char* s2, size_t n) {
    while (n > 0) {
        unsigned char c1 = (unsigned char)*s1;
        unsigned char c2 = (unsigned char)*s2;

        if (c1 != c2 || c1 == '\0') {
            return c1 - c2;
        }

        s1++;
        s2++;
        n--;
    }

    return 0;
}

int strlen(const char* str) {
    int len = 0;
    while (str[len]) len++;
    return len;
}

void* memcpy(void* dest, const void* src, int count) {
    char* dst = (char*)dest;
    const char* source = (const char*)src;
    for (int i = 0; i < count; i++) {
        dst[i] = source[i];
    }
    return dest;
}
