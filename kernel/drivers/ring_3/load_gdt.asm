global load_gdt

section .text

load_gdt:
    lgdt [rdi]              ; rdi = pointer to gdt_ptr struct (first arg in SysV ABI)

    ; far return to reload CS with kernel code selector 0x08
    push 0x08               ; new CS
    lea rax, [rel .reload]
    push rax                ; new RIP
    retfq                   ; pops RIP then CS — atomically reloads CS

.reload:
    mov ax, 0x10            ; kernel data selector (gdt[2] = index 2, 2*8 = 0x10)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret