[BITS 32]

global switch_task

; void switch_task(uint32_t *old_esp, uint32_t new_esp)

switch_task:

    ; stack layout
    ; [esp+4] = old_esp pointer
    ; [esp+8] = new_esp value

    mov eax, [esp+4]     ; eax = &old_esp
    mov edx, [esp+8]     ; edx = new_esp

    mov [eax], esp       ; save current esp into old task

    mov esp, edx         ; switch stack to new task

    ret
