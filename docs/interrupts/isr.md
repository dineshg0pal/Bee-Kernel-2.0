# CPU Exceptions (ISR)

## Files

- isr.c
- isr.h
- exceptions.S

## Purpose

Handles CPU-generated exceptions (0–31).

## Examples

- 0 → Divide by zero
- 13 → General protection fault
- 14 → Page fault

## Behavior

- Assembly stubs push:
  - registers
  - interrupt number
  - error code (real or fake)
- Control passed to `interrupt_dispatch`

## Special Handling

### Page Fault (14)
- Reads fault address from CR2
- System is halted safely

### Other Exceptions
- CPU is halted

## Notes

- No recovery implemented yet
- Designed to avoid unsafe operations during faults
