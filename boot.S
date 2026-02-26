[BITS 32]

global _start
extern kernel_main

_start:
    cli
    mov esp, stack_top

    push ebx        ; multiboot info addr
    push eax        ; multiboot magic
    call kernel_main

.hang:
    cli
    hlt
    jmp .hang

section .bss
align 16
stack_bottom:
    resb 16384
stack_top:
