#pragma once
#include <stdint.h>
#include <stddef.h>

void init_memory_manager(size_t mem_size);
void* alloc_page();
void free_page(void* addr);