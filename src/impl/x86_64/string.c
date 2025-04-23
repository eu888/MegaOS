#include "string.h"

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
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
