BITS 32
section .multiboot
align 8
multiboot_header:
    dd 0xE85250D6
    dd 0
    dd header_end - multiboot_header
    dd -(0xE85250D6 + 0 + (header_end - multiboot_header))
    dw 0
    dw 0
    dd 8
header_end:

section .text
global _start
extern kernel_main
_start:
    mov ax, 0x0F41      ; 'A' white on black
    mov [ebx], ax ;supposedly framebuffer addr
    call kernel_main
jmp .start

.start:
    mov ax, 0x41
    mov [ebx], ax
    mov edi, 0xB8000
    mov [edi], ax

    jmp .hang

.hang:
    cli
    hlt
    jmp .hang