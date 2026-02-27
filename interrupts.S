[BITS 32]

extern interrupt_dispatch

%macro IRQ 2
global irq%1
irq%1:
    cli
    push dword 0          ; fake error code
    push dword %2         ; interrupt number (32–47)
    jmp irq_common
%endmacro

; Hardware IRQs (after PIC remap to 32–47)
IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47

irq_common:
    pusha

    mov eax, esp
    push eax

    call interrupt_dispatch

    add esp, 4

    popa

    add esp, 8            ; remove int_no + err_code

    sti
    iretd
