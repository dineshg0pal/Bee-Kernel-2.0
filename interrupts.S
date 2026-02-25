[BITS 32]

global idt_load
idt_load:
    mov eax, [esp+4]
    lidt [eax]
    ret

global irq1_stub
extern irq1_handler

irq1_stub:
    cli

    push ds
    push es
    push fs
    push gs
    pusha

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call irq1_handler

    popa
    pop gs
    pop fs
    pop es
    pop ds

    iretd
