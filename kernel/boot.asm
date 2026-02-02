BITS 32

section .multiboot
align 8
multiboot_header:
    dd 0xE85250D6                  ; magic
    dd 0                            ; flags
    dd header_end - multiboot_header ; checksum placeholder
    dd -(0xE85250D6 + 0 + (header_end - multiboot_header)) ; real checksum
    dw 0
    dw 0
    dd 8                            ; header length
header_end:

global _start
extern kernel_main

_start:
    ; ======== MULTIBOOT2 REQUESTS FRAMEBUFFER ========
    ; Flags to request framebuffer:
    ; Bit 12 = Use framebuffer
    ; Bits 0 = 0 (ignore memory info)
    ; So flags = 0x00010000 (bit 16 in 32-bit) ? Actually, see below

    ; For Multiboot2, to request a framebuffer, you don’t need
    ; any BIOS call. Just leave flags = 0, GRUB will provide a framebuffer tag
    ; if the video mode is set in GRUB menu (or if you add a framebuffer tag manually)
    
    ; We remove any text-mode BIOS calls
    ; mov ah, 0x00
    ; mov al, 0x03
    ; int 0x10

    call kernel_main       ; jump to your kernel

.hang:
    cli
    hlt
    jmp .hang
