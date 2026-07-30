global irq_stub
global keyboard_stub
global exception_stub_0
extern time_interrupt_handler
extern keyboard_interrupt_handler
extern exception_handler
global exception_stub_8
global exception_stub_13
global exception_stub_14
global exception_stub_generic

irq_stub:
    push rax
    push rcx
    push rdx
    push rbx
    push rbp
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    mov rbx, rsp
    and rsp, -16
    call time_interrupt_handler
    mov rsp, rbx

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rbp
    pop rbx
    pop rdx
    pop rcx
    pop rax
    iretq

keyboard_stub:
    push rbp
    mov rbp, rsp

    push rax
    push rcx
    push rdx
    push rbx
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    mov ax, ds
    push rax

    mov rbx, rsp
    and rsp, -16

    mov ax, 0x10
    mov ds, ax
    mov es, ax

    call keyboard_interrupt_handler

    mov rsp, rbx

    pop rax
    mov ds, ax
    mov es, ax

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rbx
    pop rdx
    pop rcx
    pop rax
    pop rbp

    iretq 
        
exception_stub_0:
    push 0 
    mov rdi, 0
    mov rsi, 0
    call exception_handler

exception_stub_8:
    mov rdi, 0x8
    pop rsi
    call exception_handler

exception_stub_13:
    mov rdi, 0xD
    pop rsi
    call exception_handler

exception_stub_14:
    mov rdi, 0xE
    pop rsi
    call exception_handler
    hlt

exception_stub_generic:
    push 0 
    mov rdi, 0xFF
    mov rsi, 0
    call exception_handler