    #include "vga.h"
    #include "interrupts.h"
    #include "heap.h"
    #include "gtd.h"
    #include "tss.h"

    void kernel_main(){
        terminal_initialize();
        terminal_write_string("Workss\b\n");
        terminal_write_hex(67);
        init_gdt();
        interrupt_setup();
        init_heap();
        void* user_stack = kmalloc(4096);
        uintptr_t user_stack_top = (uintptr_t)user_stack + 4096;
        void* kernel_stack = kmalloc(4096);
        uintptr_t kernel_stack_top = (uintptr_t)kernel_stack + 4096;
        set_kernel_stack(kernel_stack_top);
        jump_usermode(user_stack_top);
        for(;;);
    }