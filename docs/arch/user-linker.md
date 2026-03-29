# User Program Linker Script (user.ld)

## Purpose

Defines memory layout for user-space programs.

## Key Points

- Entry point: `_start`
- Load address: `0x00400000` (4 MB)

## Sections

### .text
- Executable code

### .rodata
- Constants

### .data
- Initialized variables

### .bss
- Uninitialized variables

## Behavior

- Separates user programs from kernel memory
- Enables controlled execution in user mode

## Notes

- Designed for ring 3 execution
- Works with paging and user mode setup
