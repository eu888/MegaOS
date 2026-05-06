#ifndef TSS_H
#define TSS_H

#include <stdint.h>
#include <stddef.h>
#include "gtd.h"
#include "string.h"

typedef struct tss_entry {
	uint32_t reserved1;
	uint64_t rsp0;
	uint64_t rsp1;
	uint64_t rsp2;
	uint64_t reserved2;
	uint64_t ist1;
	uint64_t ist2;
	uint64_t ist3;
	uint64_t ist4;
	uint64_t ist5;
	uint64_t ist6;
	uint64_t ist7;
	uint64_t reserved3;
	uint16_t reserved4;
	uint16_t iomap_base;
} __attribute__((packed)) tss_entry_t;

void write_tss(gdt_entry_bits_t* g);
void set_kernel_stack(uintptr_t stack);

#endif // TSS_H