# Kernel Linker Script (linker.ld)

## Purpose

Defines how the kernel binary is laid out in memory.

## Key Points

- Entry point: `_start`
- Kernel loaded at: `1 MB` (0x00100000)

## Sections

### .text
- Code + Multiboot header

### .rodata
- Read-only data (constants)

### .data
- Initialized global variables

### .bss
- Uninitialized data
- Stack is placed here
- `end` symbol marks memory end

## Behavior

- Ensures correct memory alignment
- Controls final ELF layout

## Notes

- Critical for boot correctness
- Must match assumptions in boot code
