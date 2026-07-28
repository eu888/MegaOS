bits 32

%define ALIGN 1<<0
%define MEMINFO 1<<1
%define FLAGS (ALIGN | MEMINFO)
%define MAGIC 0x1BADB002
%define CHECKSUM -(MAGIC + FLAGS)

extern long_mode_start

section .data
align 8
gdt_start:
    ; ring 0
    dq 0x0000000000000000
    dq 0x00AF9A000000FFFF    
    dq 0x00CF92000000FFFF    

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

section .multiboot
align 4
dd MAGIC
dd FLAGS
dd CHECKSUM

section .bss
align 4096
l4: resq 512
l3: resq 512
l2: resq 512
l1: resq 512
align 16
stack_bottom:
    resb 16384
stack_top:

section .text
global _start
_start:
    cli
        
    mov esp, stack_top

    xor edx, edx

    mov eax, l3
    or eax, 0x07       ; present, rw, user
    mov [l4], eax
    mov dword [l4 + 4], edx

    mov eax, l2
    or eax, 0x07       ; present, rw, user
    mov [l3], eax
    mov dword [l3 + 4], edx

    xor ecx, ecx
    .pd_loop:
        mov eax, ecx
        shl eax, 21         
        or eax, 0x87        
        mov [l2 + ecx*8], eax
        mov dword [l2 + ecx*8 + 4], edx
        inc ecx
        cmp ecx, 512
        jne .pd_loop

        lgdt [gdt_descriptor]

        mov eax, cr4
        or eax, 1 << 5
        mov cr4, eax

        mov eax, l4
        mov cr3, eax

        mov ecx, 0xC0000080
        rdmsr
        or eax, 1 << 8
        wrmsr

        mov eax, cr0
        or eax, 1 << 31
        mov cr0, eax

        
        jmp 0x08:long_mode_start
