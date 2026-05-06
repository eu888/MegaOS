#include "gtd.h"
#include "tss.h"



gdt_entry_bits_t gdt[8];
gdt_entry_bits_t* ring3_code = &gdt[3];
gdt_entry_bits_t* ring3_data = &gdt[4];

extern void flush_tss(void);

void init_gdt(void){
    memset(gdt, 0, sizeof gdt);

    // Kernel code segment (ring 0)
    gdt[1].limit_low = 0xFFFF;
    gdt[1].base_low = 0;
    gdt[1].accessed = 0;
    gdt[1].read_write = 1;
    gdt[1].conforming_expand_down = 0;
    gdt[1].code = 1;
    gdt[1].code_data_segment = 1;
    gdt[1].DPL = 0;
    gdt[1].present = 1;
    gdt[1].limit_high = 0xF;
    gdt[1].available = 0;
    gdt[1].long_mode = 1;  
    gdt[1].big = 0;
    gdt[1].granularity = 1;
    gdt[1].base_high = 0;

    // Kernel data segment (ring 0)
    gdt[2].limit_low = 0xFFFF;
    gdt[2].base_low = 0;
    gdt[2].accessed = 0;
    gdt[2].read_write = 1;
    gdt[2].conforming_expand_down = 0;
    gdt[2].code = 0;
    gdt[2].code_data_segment = 1;
    gdt[2].DPL = 0;
    gdt[2].present = 1;
    gdt[2].limit_high = 0xF;
    gdt[2].available = 0;
    gdt[2].long_mode = 0;
    gdt[2].big = 1;
    gdt[2].granularity = 1;
    gdt[2].base_high = 0;

    ring3_code->limit_low = 0xFFFF;
    ring3_code->base_low = 0;
    ring3_code->accessed = 0;
    ring3_code->read_write = 1;
    ring3_code->conforming_expand_down = 0;
    ring3_code->code = 1;
    ring3_code->code_data_segment = 1;
    ring3_code->DPL = 3;
    ring3_code->present = 1;
    ring3_code->limit_high = 0xF;
    ring3_code->available = 1;
    ring3_code->long_mode = 1;  
    ring3_code->big = 0;
    ring3_code->granularity = 1;
    ring3_code->base_high = 0;

    *ring3_data = *ring3_code;
    ring3_data->code = 0;
    ring3_data->long_mode = 0;
    ring3_data->big = 1;

    write_tss(&gdt[5]);

    gdt_ptr_t gdt_ptr = {
        .limit = sizeof(gdt) - 1,
        .base = (uint64_t)&gdt
    };

    load_gdt(&gdt_ptr);

    flush_tss();
}
