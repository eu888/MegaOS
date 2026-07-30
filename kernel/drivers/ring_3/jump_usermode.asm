global jump_usermode

jump_usermode:

    cli

    mov ax, 0x23
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push 0x23                ; SS
    push rsi                 ; RSP
    push 0x202               ; RFLAGS
    push 0x1B                ; CS
    push rdi

    ; jmp $

    iretq