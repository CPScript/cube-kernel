BITS 16
ORG 0x7C00

start:
    mov ax, 0x0013  ; Set video mode to 320x200 256-color (Mode 13h)
    int 0x10


    mov ax, 0x7E00    ; Load kernel from 0x7E00
    mov ds, ax
    mov si, 0         ; Offset in the kernel
    jmp 0x0000:0x7E00 ; Jump to the kernel

times 510-($-$$) db 0
dw 0xAA55
