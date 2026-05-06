global jump_usermode
extern test_user_function

section .text

jump_usermode:          ; rdi = user_stack_top
    mov ax, 0x23        ; ring 3 data selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push 0x23               ; SS
    push rdi                ; RSP — user stack top
    push 0x202              ; RFLAGS — IF set
    push 0x1B               ; CS — ring 3 code selector
    push test_user_function ; RIP
    iretq