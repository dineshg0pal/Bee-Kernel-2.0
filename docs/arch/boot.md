# Boot Entry (boot.S)

## Purpose

Defines the kernel entry point and prepares the environment before calling C code.

## Key Steps

1. Disable interrupts
2. Set up stack
3. Pass Multiboot data to kernel
4. Call `kernel_main`

## Flow

- `_start` is the first executed symbol
- Stack is set using a fixed buffer in `.bss`
- Multiboot registers:
  - `eax` → magic value
  - `ebx` → multiboot info pointer
- Both are pushed and passed to `kernel_main`

## After Kernel Returns

- CPU is halted in an infinite loop

## Notes

- Runs in 32-bit protected mode
- No paging yet
- Stack size: 16 KB
