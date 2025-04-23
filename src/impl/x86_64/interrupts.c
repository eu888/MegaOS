#include "interrupts.h"
#include "idt.h"
#include "print.h"
#include "io.h"
#include <stdint.h>

extern void interrupt_handler_0();
extern void interrupt_handler_1(); 
extern void (*interrupt_handlers[NUM_INTERRUPTS])();
extern void handle_timer_interrupt();
extern void handle_keyboard_interrupt();


void init_interrupts() {
    init_pic();  
    set_idt_gate(32, (uint64_t)interrupt_handler_0); // IRQ0 - Timer
    set_idt_gate(33, (uint64_t)interrupt_handler_1); // IRQ1 - Keyboard
    print_str("IRQ0 seted.\n");
    print_str("IRQ1 seted.\n");

    interrupt_handlers[32] = handle_timer_interrupt;
    interrupt_handlers[33] = handle_keyboard_interrupt;

    __asm__ volatile ("sti");

    print_str("All interrupts seted.\n");
}

void init_pic() {
    
    outb(PIC1_COMMAND, 0x11);  // Send the initialization command (ICW1)
    outb(PIC1_DATA, PIC1_COMMAND);  // Master PIC vector offset (0x20 for IRQ0)
    outb(PIC1_DATA, 0x04);  // Master PIC has a slave on IRQ2
    outb(PIC1_DATA, 0x01);  // 8086/88 mode

    outb(PIC2_COMMAND, 0x11);  // Send the initialization command (ICW1)
    outb(PIC2_DATA, 0x28);  // Slave PIC vector offset (0x28 for IRQ8)
    outb(PIC2_DATA, 0x02);  // Slave PIC is connected to IRQ2 on the master PIC
    outb(PIC2_DATA, 0x01);  // 8086/88 mode

    outb(PIC1_DATA, 0xFF);  // Mask all IRQs on the master PIC
    outb(PIC2_DATA, 0xFF);  // Mask all IRQs on the slave PIC

    outb(PIC1_DATA, 0xFC);  // Unmask IRQ0 and IRQ1 (0xFD would mask IRQ1, unmask IRQ0)
    outb(PIC2_DATA, 0xFF);  // Keep the slave PIC masked

    outb(PIC1_COMMAND, PIC1_COMMAND);  // Send EOI to the master PIC
    outb(PIC2_COMMAND, PIC1_COMMAND);  // Send EOI to the slave PIC (for IRQ2)
}
