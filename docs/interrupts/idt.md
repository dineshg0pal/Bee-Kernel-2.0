# Interrupt Descriptor Table (IDT)

## Files

- idt.c
- idt.h
- idt_load.S

## Purpose

Stores pointers to interrupt handlers used by the CPU.

## Structure

- 256 entries
- Each entry contains:
  - handler address
  - segment selector
  - type and privilege flags

## Key Functions

### idt_set_gate
- Sets a single IDT entry

### idt_init
- Clears all entries
- Remaps PIC

### idt_load_now
- Loads IDT using `lidt`

## PIC Remapping

- IRQs are moved:
  - IRQ0 → 32
  - IRQ15 → 47

This avoids conflict with CPU exceptions (0–31).

## Notes

- IDT must be loaded before enabling interrupts
- Uses interrupt gates (0x8E)
