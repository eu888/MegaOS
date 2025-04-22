global load_idt

section .text
load_idt:
    mov rax, rdi
    lidt [rax] 
    ret
