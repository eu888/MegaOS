#include "print.h"
#include "memory.h"

void kernel_main() {
    init_memory_manager(64 * 1024 * 1024); 
    init_heap();

    print_str("Reserving heap...\n");

    void* ptr = kmalloc(128);
    if (ptr) {
        print_str("Allocated 128 bytes.\n");
    }

    check_heap_integrity();

    kfree(ptr);

    check_heap_integrity();
}
