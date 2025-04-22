global outb
global inb

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
