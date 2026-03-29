# Global Descriptor Table (GDT)

## Files

- gdt.c
- gdt.h
- gdt_flush.S

## Purpose

Defines memory segments and privilege levels for the CPU.

## Entries Used

| Index | Segment        | Description        |
|------|---------------|--------------------|
| 0    | Null          | Required empty     |
| 1    | Kernel Code   | Ring 0 code        |
| 2    | Kernel Data   | Ring 0 data        |
| 3    | User Code     | Ring 3 code        |
| 4    | User Data     | Ring 3 data        |
| 5    | TSS           | Task State Segment |

## Key Functions

### gdt_set_gate
- Creates a GDT entry
- Sets base, limit, access, and flags

### gdt_init
- Initializes all GDT entries
- Loads GDT using `lgdt`
- Sets segment registers
- Initializes TSS

## Assembly (gdt_flush)

- Loads GDT using `lgdt`
- Reloads segment registers
- Performs far jump to update CS

## Segment Selectors

- Kernel Code: `0x08`
- Kernel Data: `0x10`
- User Code: `0x1B`
- User Data: `0x23`

## Notes

- Uses flat memory model (base = 0)
- Full 4GB limit for all segments
- Required before enabling user mode
