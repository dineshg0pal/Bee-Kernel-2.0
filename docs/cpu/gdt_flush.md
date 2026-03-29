# Task State Segment (TSS)

## Files

- gdt.c (TSS implementation)
- gdt_flush.S (tss_flush)

## Purpose

Provides stack switching when transitioning from user mode (ring 3) to kernel mode (ring 0).

## Key Fields

- esp0 → Kernel stack pointer
- ss0  → Kernel stack segment
- iomap_base → disables I/O access

## Setup

### write_tss

- Creates a TSS entry in GDT
- Initializes TSS structure
- Sets:
  - Kernel stack (ss0, esp0)
  - User segments (cs, ds, etc.)

### tss_set_kernel_stack

- Updates kernel stack dynamically
- Used during task switching

## Assembly (tss_flush)

- Loads TSS using `ltr`

## Behavior

- On interrupt/syscall from user mode:
  - CPU switches to kernel stack using TSS
- Prevents user processes from controlling kernel stack

## Notes

- Only one TSS is used (no hardware task switching)
- Required for safe ring 3 → ring 0 transitions
