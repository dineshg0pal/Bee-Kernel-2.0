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

    add esp, 4            ; remove argument

    ; Restore registers manually except eax
    pop edi
    pop esi
    pop ebp
    add esp, 4            ; skip esp
    pop ebx
    pop edx
    pop ecx
    pop eax               ; THIS now contains updated eax from frame

    add esp, 8            ; remove int_no and error code

    iretd
