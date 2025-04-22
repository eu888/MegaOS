#include "interrupts.h"
#include "idt.h"
#include <stdint.h>

extern void interrupt_handler_0();
extern void (*interrupt_handlers[NUM_INTERRUPTS])();

void init_interrupts() {
    interrupt_handlers[0] = interrupt_handler_0;

    set_idt_gate(0, (uint64_t)interrupt_handlers[0]); 
}
