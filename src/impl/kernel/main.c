#include "print.h"
#include "memory.h"

void kernel_main(){
    print_clear();
    print_set_color(PRINT_COLOR_LIGHT_GREEN, PRINT_COLOR_BLACK);
    print_str("Initializing memory manager...\n");
    
    init_memory_manager(1024 * 1024 * 128); // 128MB

    void* page = alloc_page();
    print_str("Allocated page at: ");
    print_hex((uint64_t) page); // you'll need a print_hex function
    print_str("\n");
    print_str("Nice OS!\n");
    print_str("root@001: ");
}