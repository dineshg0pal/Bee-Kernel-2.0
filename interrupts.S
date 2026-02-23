; interrupts.S

global idt_load
global irq1_stub

extern irq1_handler

section .text

idt_load:
    mov eax, [esp + 4]
    lidt [eax]
    ret

irq1_stub:
    cli
    pusha

    call irq1_handler

    popa
    sti
    iretd
