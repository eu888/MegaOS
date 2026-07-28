#pragma once

#include <stddef.h>
#include <stdint.h>

uint8_t strcmp(const char* s1, const char* s2);
size_t strlen(const char* str);
uint8_t strncmp(const char* s1, const char* s2, size_t index);
void *memset(void *dest, int val, size_t len);
void *memcpy(void *dest, const void *src, size_t n);