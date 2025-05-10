#include "idt.h"
#include "io.h"
#include "print.h"
#include "terminal.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

void (*interrupt_handlers[NUM_INTERRUPTS])();

void remap_pic() {
    outb(PIC1_COMMAND, 0x11);
    outb(PIC2_COMMAND, 0x11);
    outb(PIC1_DATA, 0x20);
    outb(PIC2_DATA, 0x28);
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);
    outb(PIC1_DATA, 0x0);
    outb(PIC2_DATA, 0x0);
}

void send_eoi(int irq) {
    if (irq >= 8)
        outb(PIC2_COMMAND, 0x20);
    outb(PIC1_COMMAND, 0x20);
}

void handle_timer_interrupt() {
    if (interrupt_handlers[32])
        interrupt_handlers[32]();
    send_eoi(0);
}

void handle_keyboard_interrupt() {
    uint8_t scancode = inb(0x60);  

    if (scancode < 0x80) {
        char key = scancode_to_ascii(scancode);
        if (key) {
            process_key(key);
        }
    }

    send_eoi(1);  
}

void handle_irq2_interrupt(){
    print_str("IRQ2 seted up.\n");
    send_eoi(2);
}

void handle_com2_com4_interrupt(){
    print_str("IRQ3 seted up.\n");
    send_eoi(3);
}

char scancode_to_ascii(uint8_t scancode) {
    static const char ascii_table[128] = {
        0,  27, '1','2','3','4','5','6','7','8','9','0','-','=','\b', // 0x00 - 0x0E
        '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',    // 0x0F - 0x1C
        0,  'a','s','d','f','g','h','j','k','l',';','\'','`',         // 0x1D - 0x29
        0,  '\\','z','x','c','v','b','n','m',',','.','/',0,           // 0x2A - 0x35
        '*',0,  ' ',0, 0, 0, 0, 0, 0, 0,                              // 0x36 - 0x3F
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,                        // 0x40 - 0x49
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,                        // 0x4A - 0x53
        0,  0,  0,  0,  0,  0,  0,  0                                 // 0x54 - 0x5B
    };

    if (scancode > 127) return 0;
    return ascii_table[scancode];
}
