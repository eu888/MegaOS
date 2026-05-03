#include "gtd.h"

static gdt_entry_bits gdt[6];

gdt_entry_bits* ring3_code = &gdt[3];
gdt_entry_bits* ring3_data = &gdt[4];

ring3_code->limit_low = 0xFFFF;
ring3_code->base_low = 0;
ring3_code->accessed = 0;
ring3_code->read_write = 1; 
ring3_code->conforming = 0;
ring3_code->code = 1;
ring3_code->code_data_segment = 1;
ring3_code->DPL = 3;
ring3_code->present = 1;
ring3_code->limit_high = 0xF;
ring3_code->available = 1;
ring3_code->long_mode = 0;
ring3_code->big = 1;
ring3_code->granularity = 1;
ring3_code->base_high = 0;

*ring3_data = *ring3_code; 
ring3_data->code = 0;

