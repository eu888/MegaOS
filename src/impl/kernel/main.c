#include "print.h"
#include "memory.h"
#include "idt.h"
#include "pit.h"
#include "interrupts.h"
#include "terminal.h"
#include "ata_pio_read.h"
#include "file.h"
#include "io.h"

void kernel_main() {
    print_clear();
    print_set_color(PRINT_COLOR_LIGHT_GREEN, PRINT_COLOR_BLACK);
    print_str("Initializing memory manager...\n");
    init_memory_manager(1024 * 1024 * 512); 
    print_str("Memory manager initialized.\n");

    print_str("Initializing heap...\n");
    init_heap();
    print_str("Heap initialized.\n");

    void* page = alloc_page();
    if (!page) {
        print_set_color(PRINT_COLOR_RED, PRINT_COLOR_BLACK);
        print_str("Failed to allocate page\n");
        return;
    }
    print_str("Page allocated successfully\n");

    void* ptr = kmalloc(512 * 1024);
    if (!ptr) {
        print_set_color(PRINT_COLOR_RED, PRINT_COLOR_BLACK);
        print_str("Failed to allocate 512 kilobytes from heap.\n");
        return;
    }
    print_str("Allocated 512 kilobytes successfully.\n");

    print_str("Checking heap integrity...\n");
    check_heap_integrity();
    clear_screen();

    print_str("Starting memory driver\n");
    uint8_t exFATBuffer[512];
    print_str("Reading LBA0...\n");
    ata_pio_read28(0,1, exFATBuffer);
    for (int i = 0; i < 16; i++) {
        print_hex(exFATBuffer[i]);
        print_str(" ");
    }
    print_str("\n");
    print_str("LBA0 read successfully.\n");
    print_str("Initializing filesystem...\n");
    fs_init(exFATBuffer, 512);
    print_str("Filesystem initialized.\n");

    init_idt();         
    init_interrupts();  
    init_pit(100);      
    print_str("Kernel setup complete.\n");
    start_simbol();  

    while (1) {
        __asm__ volatile ("hlt"); 
    }
}
