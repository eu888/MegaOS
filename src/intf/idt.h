#ifndef IDT_H
#define IDT_H

#include <stdint.h>

#define NUM_INTERRUPTS 256

struct idt_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t  ist;
    uint8_t  type_attr;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t zero;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

void set_idt_gate(int vector, uint64_t handler);
void load_idt();
void init_idt();
void init_pic();

#endif
