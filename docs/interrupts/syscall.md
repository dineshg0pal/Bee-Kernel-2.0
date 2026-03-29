# System Calls

## Files

- isr.c
- syscall_stub.S (linked externally)

## Interrupt Number

- 128 (0x80)

## Purpose

Provides interface for user programs to request kernel services.

## Setup

- IDT entry 128:
  - selector: kernel code (0x08)
  - flags: 0xEE (user accessible)

## Flow

1. User triggers interrupt 0x80
2. Control enters kernel
3. `handle_syscall` is called

## Arguments

- eax → syscall number
- ebx → argument (if needed)

## Supported Syscalls

- SYS_PRINT → print string
- SYS_GETCHAR → read input
- SYS_MEMINFO → memory stats
- SYS_ALLOC / SYS_FREE → memory management
- SYS_UPTIME → timer ticks
- SYS_STATE → scheduler state
- SYS_KMSG → kernel logs
- SYS_TASKS → list tasks
- SYS_KILL → terminate task
- SYS_SPAWN → create task

## Notes

- Runs in kernel mode
- Uses same interrupt pipeline as IRQ/ISR
