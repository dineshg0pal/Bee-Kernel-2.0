[BITS 32]

global switch_to_user_mode
extern user_stack_top
extern user_main

switch_to_user_mode:

    mov eax, user_stack_top

    push 0x23            ; User SS
    push eax             ; User ESP
    pushfd               ; EFLAGS
    push 0x1B            ; User CS
    push user_mode_entry ; User EIP
    iret


user_mode_entry:

    ; Now in Ring 3
    mov ax, 0x23
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call user_main

.hang:
    jmp .hang
