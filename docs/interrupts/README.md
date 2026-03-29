# Interrupt System

This section documents how the kernel handles CPU exceptions, hardware interrupts, and system calls.

## Components

- IDT (Interrupt Descriptor Table)
- ISRs (CPU exceptions)
- IRQs (hardware interrupts)
- Syscall interface

All interrupts are routed through a central dispatcher.
