[BITS 32]

extern interrupt_dispatch

%macro ISR_NOERR 1
global isr%1
isr%1:
    cli
    push dword 0          ; fake error code
    push dword %1         ; interrupt number
    jmp isr_common
%endmacro

%macro ISR_ERR 1
global isr%1
isr%1:
    cli
    push dword %1         ; interrupt number
    jmp isr_common
%endmacro

; Exceptions without error code
ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7
ISR_NOERR 9
ISR_NOERR 15
ISR_NOERR 16
ISR_NOERR 18
ISR_NOERR 19
ISR_NOERR 20
ISR_NOERR 21
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_NOERR 29
ISR_NOERR 30
ISR_NOERR 31

; Exceptions WITH error code
ISR_ERR 8
ISR_ERR 10
ISR_ERR 11
ISR_ERR 12
ISR_ERR 13
ISR_ERR 14
ISR_ERR 17

isr_common:
    pusha

    mov eax, esp
    push eax              ; pass pointer to interrupt_frame_t

    call interrupt_dispatch

    add esp, 4            ; remove argument

    popa

    add esp, 8            ; remove int_no + err_code

    sti
    iretd
