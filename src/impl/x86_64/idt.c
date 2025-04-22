#include <stdint.h>
#include "idt.h"
#include "interrupts.h"
#include "print.h"

#define IDT_ENTRIES 256
extern void (*interrupt_handlers[256])();

extern void load_idt(struct idt_ptr*);

static struct idt_entry idt[IDT_ENTRIES];

void set_idt_gate(int vector, uint64_t handler) {
    idt[vector].offset_low  = handler & 0xFFFF;
    idt[vector].selector    = 0x08; 
    idt[vector].ist         = 0;
    idt[vector].type_attr   = 0x8E; 
    idt[vector].offset_mid  = (handler >> 16) & 0xFFFF;
    idt[vector].offset_high = (handler >> 32) & 0xFFFFFFFF;
    idt[vector].zero        = 0;
}


void init_idt() {
    print_str("Initializeing idt...\n");
    for (int i = 0; i < NUM_INTERRUPTS; i++) {
        set_idt_gate(i, (uint64_t)interrupt_handlers[i]);
    }
    print_str("Initialized idt.\n");
}

