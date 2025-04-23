#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

void init_interrupts();

#endif
