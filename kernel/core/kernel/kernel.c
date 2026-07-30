#include "vga.h"
#include "interrupts.h"
#include "heap.h"
#include "gtd.h"
#include "tss.h"

void user_entry(void){
    terminal_write_string("\n>");
    for(;;);
}

void kernel_main(){
    terminal_initialize();
    init_gdt();
    interrupt_setup();
    init_heap();
    void* user_stack = kmalloc(4096);
    uintptr_t user_stack_top = (uintptr_t)user_stack + 4096;
    void* kernel_stack = kmalloc(4096);
    uintptr_t kernel_stack_top = (uintptr_t)kernel_stack + 4096;
    set_kernel_stack(kernel_stack_top);
    jump_usermode((uintptr_t)user_entry,user_stack_top);
}