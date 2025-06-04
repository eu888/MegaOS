#pragma once
#include <stdint.h>
#include <stddef.h>

void init_memory_manager(size_t mem_size);
void* alloc_page();
void free_page(void* addr);
void init_heap();
void* kmalloc(size_t size);
void kfree(void* ptr);
void check_heap_integrity();
void* alloc_pages(size_t count);