[BITS 32]

extern interrupt_dispatch

global syscall_stub

syscall_stub:
    push dword 0          ; fake error code
    push dword 128        ; interrupt number
    pusha

    mov eax, esp
    push eax

    call interrupt_dispatch

    add esp, 4
    popa
    add esp, 8
    iretd
