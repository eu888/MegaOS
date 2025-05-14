global outb
global inb
global outw
global inw

section .text
outb:
    mov dx, di
    mov al, sil
    out dx, al
    ret
inb:
    mov dx, di
    in al, dx
    movzx eax, al
    ret
outw:
    mov dx, di
    mov ax, si
    out dx, ax
    ret
inw:
    mov dx, di
    in ax, dx
    ret
