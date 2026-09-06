bits 16
org 0x7c00

boot_drive db 0


start:
    cli

    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00

    mov [boot_drive], dl

    mov ax, 0x1000
    mov es, ax
    xor bx, bx

    mov ah, 0x02
    mov al, 17
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov dl, [boot_drive]

    int 0x13
    jc disk_error
    jmp 0x1000:0x0000


disk_error:
    cli
    hlt



times 510 - ($ - $$) db 0
dw 0xAA55