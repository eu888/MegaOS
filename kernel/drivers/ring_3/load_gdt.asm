global load_gdt

section .text

load_gdt:
    lgdt [rdi]              

    push 0x08               
    lea rax, [rel .reload]
    push rax                
    retfq                   

.reload:
    mov ax, 0x10           
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret