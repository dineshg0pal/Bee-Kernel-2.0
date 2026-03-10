# 🐝 Bee Kernel (v0.1)

Bee Kernel is a small educational **32-bit x86 operating system kernel** written in C and Assembly.
The goal of the project is to explore how operating systems work internally by building core kernel components from scratch.

This kernel is designed for learning purposes and focuses on low-level system programming concepts such as interrupts, memory management, and multitasking.

---

# ✨ Features

Current features implemented in **v0.1**:

* Multiboot compliant kernel
* VGA text mode console
* Interrupt Descriptor Table (IDT)
* CPU exception handling
* Programmable Interrupt Controller (PIC) support
* Programmable Interval Timer (PIT)
* PS/2 keyboard driver
* Physical Memory Manager
* Basic paging support
* Kernel logging system
* Simple task system
* Round-robin scheduler
* Basic syscall interface

---

# 🖥 Supported Architecture

* **x86 (32-bit)**
* Tested on **QEMU**

---

# 🧰 Requirements

You need the following tools to build and run the kernel:

* GCC (with 32-bit support)
* NASM
* Make
* QEMU
* GRUB (for multiboot)

On Debian/Kali/Ubuntu you can install them with:

```bash
sudo apt update
sudo apt install build-essential gcc-multilib nasm qemu-system-x86 grub-pc-bin xorriso
```

---

# ⚙️ Build the Kernel

Clone the repository:

```bash
git clone https://github.com/YOUR_USERNAME/bee-kernel.git
cd bee-kernel
```

Build the kernel:

```bash
make
```

This will generate the kernel binary.

---

# ▶️ Run the Kernel

Run the kernel in QEMU:

```bash
make run
```

or manually:

```bash
qemu-system-i386 -kernel kernel.bin
```

---

# 📂 Project Structure

```
bee-kernel/
│
├── kernel.c
├── idt.c
├── isr.c
├── keyboard.c
├── pit.c
├── scheduler.c
├── task.c
│
├── gdt.c
├── paging.c
├── pmm.c
│
├── boot.S
├── interrupts.S
├── exceptions.S
│
├── linker.ld
├── Makefile
│
└── README.md
```

---

# 🚀 Future Plans

Planned features for future versions:

* User mode support
* ELF program loader
* Virtual memory improvements
* IPC system
* Microkernel architecture
* Simple shell
* File system support

---

# 🎯 Learning Goals

This project explores:

* x86 architecture
* interrupt handling
* kernel scheduling
* memory management
* low-level systems programming

---

# 📜 License

This project is open source and available under the MPL License.

---

# 👨‍💻 Author

Dinesh
Operating system enthusiast building Bee Kernel for learning and experimentation.

---
