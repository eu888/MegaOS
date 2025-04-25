#pragma once

#include <stdint.h>
#include <stddef.h>


int strcmp(const char* s1, const char* s2);
int strncmp(const char* s1, const char* s2, size_t n);
int strlen(const char* str);
void* memcpy(void* dest, const void* src, int count);
