#include "interrupts.h"

/** @brief An array of pointers to the interrupt handler functions. */
void (*interrupt_handlers[IDT_ENTRIES])();

volatile uint64_t ticks = 0;

enum shift_status shift = SHIFT_RELEASED;
enum caps_status caps = CAPS_OFF;

/**
 * @brief Handles the timer interrupt.
 */
void time_interrupt_handler(){
    ticks++;
    PIC_send_EOI(0);
}

/**
 * @brief Handles the keyboard interrupt.
 */
void keyboard_interrupt_handler(){
    uint8_t status = inb(0x64);
    if(!(status & 0x01)){
        PIC_send_EOI(1);
        return;
    }

    uint8_t scancode = inb(0x60);
    if((scancode == 0x2A) || (scancode == 0x36)) shift = SHIFT_PRESSED;
    if((scancode == 0xAA) || (scancode == 0xB6)) shift = SHIFT_RELEASED;
    if(scancode == 0x3A){
        if(caps == CAPS_OFF) caps = CAPS_ON;
        else caps = CAPS_OFF;
    }
    if (scancode >= 0x80){
        PIC_send_EOI(1);
        return;
    }

    

    char key = scancode_to_ascii(scancode);
    if (!key){
        PIC_send_EOI(1);
        return;
    }

    terminal_put_char(key);
    console_process_key(key);


    PIC_send_EOI(1);
}

/**
 * @brief Converts a scancode to an ASCII character, taking into account the shift and caps lock status.
 * @param scancode The scancode to convert.
 * @return The corresponding ASCII character, or 0 if the scancode does not correspond to a valid key.
 */
char scancode_to_ascii(uint8_t scancode) {
    if (scancode > 0x7F) return 0;
    if((shift ^ caps) == 0) return keyboard_keymap[scancode];
    return keyboard_keymap_shift[scancode];
} 

/**
 * @brief Handles CPU exceptions.
 * @param vector The exception vector.
 * @param error_code The error code associated with the exception, usualy provided by the CPU for certain exceptions(moste of times the rsi register value is provided as error code).  
 * This function prints a panic message with the exception name, vector, and error code, and then halts the system.
 */
void exception_handler(uint8_t vector, uint64_t error_code){
    terminal_write_string("\n[KERNEL PANIC] ");
    if(vector < 20){
        terminal_write_string(exception_names[vector]);
    } else {
        terminal_write_string("Unknown exception");
    }
    terminal_write_string("\nVector: ");
    terminal_write_hex(vector);
    terminal_write_string("\nError code: ");
    terminal_write_hex(error_code);
    uint64_t cr2;
    __asm__ volatile("mov %%cr2, %0" : "=r"(cr2));

    terminal_write_string("\nCR2: ");
    terminal_write_hex(cr2);
    uint64_t rip;
    __asm__ volatile("lea (%%rip), %0" : "=r"(rip));
    terminal_write_string("\nRIP: ");
    terminal_write_hex(rip);
    uint64_t cs;
    __asm__ volatile("mov %%cs, %0" : "=r"(cs));
    terminal_write_string("\nCS: ");
    terminal_write_hex(cs);
    for(;;);
    // terminal_write_string("\nSystem halted.");
    // __asm__ volatile("cli");
    // __asm__ volatile("hlt");
}