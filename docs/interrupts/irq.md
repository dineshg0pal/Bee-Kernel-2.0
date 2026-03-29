# Hardware Interrupts (IRQ)

## Files

- interrupts.S
- isr.c

## IRQ Range

- 32–47 (after PIC remap)

## Examples

- 32 → PIT (timer)
- 33 → Keyboard

## Flow

1. IRQ triggered by hardware
2. Assembly stub runs:
   - pushes registers
   - pushes interrupt number
3. Calls `interrupt_dispatch`

## Handling

Inside dispatcher:

- IRQ0 → `pit_callback()`
- IRQ1 → `keyboard_callback()`

## End of Interrupt (EOI)

- Sent to PIC after handling:
  - Master PIC (always)
  - Slave PIC (if IRQ ≥ 40)

## Notes

- Only PIT and keyboard are actively handled
- Others are currently ignored
