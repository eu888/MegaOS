#include "tss.h"

#include <string.h>

extern void flush_tss(void);

tss_entry_t tss_entry;

void write_tss(gdt_entry_bits_t* g){
    uintptr_t base = (uintptr_t) &tss_entry;
    uint32_t limit = sizeof(tss_entry);

    g->limit_low = limit & 0xFFFF;
	g->base_low = base & 0xFFFFFF;
	g->accessed = 1; 
	g->read_write = 0;
	g->conforming_expand_down = 0; 
	g->code = 1; 
	g->code_data_segment=0;
	g->DPL = 0; 
	g->present = 1;
	g->limit_high = (limit >> 16) & 0xF;
	g->available = 0; 
	g->long_mode = 0;
	g->big = 0;
	g->granularity = 0; 
	g->base_high = (base >> 24) & 0xFF;

    gdt_entry_bits_t* g_high = g + 1;
    g_high->limit_low = (base >> 32) & 0xFFFF;
    g_high->base_low = (base >> 48) & 0xFFFFFF;
    g_high->accessed = 0;
    g_high->read_write = 0;
    g_high->conforming_expand_down = 0;
    g_high->code = 0;
    g_high->code_data_segment = 0;
    g_high->DPL = 0;
    g_high->present = 0;
    g_high->limit_high = 0;
    g_high->available = 0;
    g_high->long_mode = 0;
    g_high->big = 0;
    g_high->granularity = 0;
    g_high->base_high = 0;

    memset(&tss_entry, 0, sizeof tss_entry);
    tss_entry.rsp0 = 0;
}

void set_kernel_stack(uintptr_t stack){ 
	tss_entry.rsp0 = stack;
}