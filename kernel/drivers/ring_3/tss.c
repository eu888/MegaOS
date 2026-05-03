#include "tss.h"

tss_entry_t tss_entry;

extern void flush_tss(void);

void write_tss(gdt_entry_bits_t* g){
    uint32_t base = (uint32_t) &tss_entry;
    uint32_t limit = sizeof(tss_entry);

    g->limit_low = limit;
	g->base_low = base;
	g->accessed = 1; 
	g->read_write = 0;
	g->conforming_expand_down = 0; 
	g->code = 1; 
	g->code_data_segment=0;
	g->DPL = 0; 
	g->present = 1;
	g->limit_high = (limit & (0xf << 16)) >> 16;
	g->available = 0; 
	g->long_mode = 0;
	g->big = 0;
	g->granularity = 0; 
	g->base_high = (base & (0xff << 24)) >> 24;

	memset(&tss_entry, 0, sizeof tss_entry);

	tss_entry.ss0 = 0x10; 
	tss_entry.esp0 = 0; 
}

void set_kernel_stack(uint32_t stack){ 
	tss_entry.esp0 = stack;
}