#include "gtd.h"
#include "tss.h"



gdt_entry_bits_t gdt[8];
gdt_entry_bits_t* ring3_code = &gdt[3];
gdt_entry_bits_t* ring3_data = &gdt[4];

extern void flush_tss(void);

void gdt_set_entry(gdt_entry_bits_t *entry, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags){
    entry->limit_low  = limit & 0xFFFF;

    entry->base_low = base & 0xFFFF;
    entry->base_mid = (base >> 16) & 0xFF;
    entry->base_high = (base >> 24) & 0xFF;

    entry->access = access;

    entry->granularity = ((limit >> 16) & 0x0F) | (flags & 0xF0);
}

void init_gdt(void){
    memset(gdt, 0, sizeof gdt);

    // Kernel code segment (ring 0)
    gdt_set_entry(&gdt[1], 0, 0xFFFFF, 0x9A, 0xA0);

    // Kernel data segment (ring 0)
    gdt_set_entry(&gdt[2], 0, 0xFFFFF, 0x92, 0xC0);
    

    // User code segment (ring 3)
    gdt_set_entry(ring3_code, 0, 0xFFFFF, 0xFA, 0xA0);
    
    // User data segment (ring 3)
    gdt_set_entry(ring3_data, 0, 0xFFFFF, 0xF2, 0xC0);

    write_tss(&gdt[5]);

    gdt_ptr_t gdt_ptr = {
        .limit = sizeof(gdt) - 1,
        .base = (uint64_t)&gdt
    };

    load_gdt(&gdt_ptr);

    flush_tss();
}
