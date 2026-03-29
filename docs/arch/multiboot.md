# Multiboot Header

## Purpose

Allows a Multiboot-compliant bootloader (e.g., GRUB) to recognize and load the kernel.

## Structure

- Magic number: identifies Multiboot header
- Flags: request features from bootloader
- Checksum: ensures header validity

## Values Used

- Magic: `0x1BADB002`
- Flags: `0x00000003`
- Checksum: makes sum = 0

## Behavior

- Must be placed in the `.text` section (early in binary)
- Bootloader scans for this header
- If valid, kernel is loaded and executed

## Notes

- Required for GRUB compatibility
- Enables passing boot info to kernel
