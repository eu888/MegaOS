#ifndef GTD_H
#define GTD_H

#include <stdint.h>
#include <stddef.h>
#include "vga.h"
#include "string.h"

typedef struct gdt_entry_bits{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed)) gdt_entry_bits_t;

typedef struct gdt_ptr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) gdt_ptr_t;

extern gdt_entry_bits_t gdt[8];
extern gdt_entry_bits_t* ring3_code;
extern gdt_entry_bits_t* ring3_data;

extern void load_gdt(gdt_ptr_t* ptr);
extern void jump_usermode(uintptr_t stack_top);

void gdt_set_entry(gdt_entry_bits_t *entry, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags);
void init_gdt(void);

#endif