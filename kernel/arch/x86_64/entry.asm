BITS 64
GLOBAL _start
EXTERN kernel_main

SECTION .text
_start:
    ; Limine already put us in long mode with paging enabled

    ; Set up stack
    mov rsp, stack_top

    ; Clear direction flag (required by SysV ABI)
    cld

    ; Call C kernel entry
    call kernel_main

.hang:
    hlt
    jmp .hang

SECTION .bss
align 16
stack_bottom:
    resb 16384      ; 16 KB stack
stack_top: