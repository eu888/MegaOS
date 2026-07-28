#include "tss.h"

#include <string.h>

extern void flush_tss(void);

tss_entry_t tss_entry;

void write_tss(gdt_entry_bits_t* g){
    memset(&tss_entry, 0, sizeof(tss_entry));

    uintptr_t base = (uintptr_t)&tss_entry;
    uint32_t limit = sizeof(tss_entry) - 1;

    tss_descriptor_t desc = {
        .limit_low = limit & 0xFFFF,
        .base_low = base & 0xFFFF,
        .base_mid = (base >> 16) & 0xFF,
        .base_high = (base >> 24) & 0xFF,
        .base_upper = (uint32_t)(base >> 32),
        .access = 0x89,
        .granularity = (limit >> 16) & 0x0F,
        .reserved = 0
    };

    memcpy(g, &desc, sizeof(desc));

    tss_entry.rsp0 = 0;
}

void set_kernel_stack(uintptr_t stack){ 
	tss_entry.rsp0 = stack;
}