#include "memory.h"

#define PAGE_SIZE 4096
#define MAX_MEMORY (1024 * 1024 * 512) 
#define MAX_PAGES (MAX_MEMORY / PAGE_SIZE)

static uint8_t page_bitmap[MAX_PAGES / 8];
static size_t total_pages;

static inline void set_bit(size_t bit) {
    page_bitmap[bit / 8] |= (1 << (bit % 8));
}

static inline void clear_bit(size_t bit) {
    page_bitmap[bit / 8] &= ~(1 << (bit % 8));
}

static inline int test_bit(size_t bit) {
    return (page_bitmap[bit / 8] >> (bit % 8)) & 1;
}

void init_memory_manager(size_t mem_size) {
    total_pages = mem_size / PAGE_SIZE;
    
    for (size_t i = 0; i < total_pages / 8; i++) {
        page_bitmap[i] = 0; 
    }

    for (size_t i = 0; i < 256; i++) {
        set_bit(i);
    }
}
    

void* alloc_page() {
    for (size_t i = 0; i < total_pages; i++) {
        if (!test_bit(i)) {
            set_bit(i);
            return (void*)(i * PAGE_SIZE);
        }
    }
    return 0; 
}

void free_page(void* addr) {
    size_t index = (size_t)addr / PAGE_SIZE;
    clear_bit(index);
}