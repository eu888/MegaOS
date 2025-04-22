#include "memory.h"
#include "print.h"
#include <stddef.h>
#include <stdint.h>

#define PAGE_SIZE 4096
#define HEAP_START 0x200000
#define HEAP_SIZE  0x100000
#define GUARD_PATTERN 0xDEADBEEF
#define GUARD_SIZE sizeof(uintptr_t)

typedef struct block_header {
    size_t size;
    int free;
    struct block_header* next;
    uintptr_t guard_front;
} block_header_t;

static uintptr_t heap_base = HEAP_START;
static uintptr_t heap_end = HEAP_START + HEAP_SIZE;
static block_header_t* heap_head = NULL;

void init_heap() {
    size_t heap_pages = HEAP_SIZE / PAGE_SIZE;
    for (size_t i = 0; i < heap_pages; i++) {
        alloc_page();
    }

    heap_head = (block_header_t*)heap_base;
    heap_head->size = HEAP_SIZE - sizeof(block_header_t) - GUARD_SIZE;
    heap_head->free = 1;
    heap_head->next = NULL;
    heap_head->guard_front = GUARD_PATTERN;

    uintptr_t* guard_back = (uintptr_t*)((uintptr_t)heap_head + sizeof(block_header_t) + heap_head->size);
    *guard_back = GUARD_PATTERN;
}

void* kmalloc(size_t size) {
    if (size == 0) return NULL;

    block_header_t* current = heap_head;
    while (current) {
        if (current->free && current->size >= size) {
            current->free = 0;

            current->guard_front = GUARD_PATTERN;
            uintptr_t* guard_back = (uintptr_t*)((uintptr_t)current + sizeof(block_header_t) + size);
            *guard_back = GUARD_PATTERN;

            return (void*)((uintptr_t)current + sizeof(block_header_t));
        }

        current = current->next;
    }

    return NULL; 
}

void kfree(void* ptr) {
    if (!ptr) return;
    block_header_t* header = (block_header_t*)((uintptr_t)ptr - sizeof(block_header_t));
    header->free = 1;

    if (header->next && header->next->free) {
        header->size += sizeof(block_header_t) + header->next->size;
        header->next = header->next->next; 
    }

    block_header_t* current = heap_head;
    while (current) {
        if (current->next == header) {
            if (current->free) {
                current->size += sizeof(block_header_t) + header->size;
                current->next = header->next; // Remove header from the list
            }
            break;
        }
        current = current->next;
    }
}

void check_heap_integrity() {
    block_header_t* current = heap_head;

    while (current) {
        if (current->guard_front != GUARD_PATTERN) {
            print_str("Heap corruption detected: invalid guard at the beginning!\n");
            return;
        }

        uintptr_t* guard_back = (uintptr_t*)((uintptr_t)current + sizeof(block_header_t) + current->size);
        if (*guard_back != GUARD_PATTERN) {
            print_str("Heap corruption detected: invalid guard at the end!\n");
            return;
        }

        current = current->next;
    }

    print_str("Heap integrity check passed.\n");
}
