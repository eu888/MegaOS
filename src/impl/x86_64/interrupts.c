#include "interrupts.h"
#include "idt.h"
#include "print.h"
#include "io.h"
#include <stdint.h>

extern void interrupt_handler_0();  
extern void interrupt_handler_1(); 
extern void isr_irq2();
extern void interrupt_handler_3();
extern void (*interrupt_handlers[NUM_INTERRUPTS])();
extern void handle_timer_interrupt();
extern void handle_keyboard_interrupt();
extern void handle_irq2_interrupt();
extern void handle_com2_com4_interrupt();


void init_interrupts() {
    init_pic();  
    set_idt_gate(32, (uint64_t)interrupt_handler_0); // IRQ0 
    set_idt_gate(33, (uint64_t)interrupt_handler_1); // IRQ1 
    set_idt_gate(34, (uint64_t)isr_irq2); // IRQ2
    set_idt_gate(35, (uint64_t)interrupt_handler_3); // IRQ3
    print_str("IRQ0 seted.\n");
    print_str("IRQ1 seted.\n");

    interrupt_handlers[32] = handle_timer_interrupt;
    interrupt_handlers[33] = handle_keyboard_interrupt;
    interrupt_handlers[34] = handle_irq2_interrupt;
    interrupt_handlers[35] = handle_com2_com4_interrupt;

    __asm__ volatile ("sti");

    print_str("All interrupts seted.\n");
}

void init_pic() {
    outb(PIC1_COMMAND, 0x11);
    outb(PIC2_COMMAND, 0x11);
    outb(PIC1_DATA, 0x20);  
    outb(PIC2_DATA, 0x28);  
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);
    outb(PIC2_DATA, 0xFF); 
    
    uint8_t master_mask = inb(PIC1_DATA);
    master_mask &= ~(1 << 0);  
    master_mask &= ~(1 << 1);  
    master_mask &= ~(1 << 2);  
    master_mask &= ~(1 << 3);
    outb(PIC1_DATA, master_mask);
    outb(PIC2_DATA, 0xFF);
}
