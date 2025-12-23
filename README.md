# myOS from scratch

Writing my own OS for a 32bit RISC-V architecture. The implementation is based off of this project: 
[text](https://operating-system-in-1000-lines.vercel.app/en/)

In addition to building my own OS, the aim of the project is to implement the following features which are not covered in the above:
1) Interrupt handling instead of polling for new data on devices
2) Preemptive Multitasking
3) Inter-process communication
4) Memory Allocation to free memory

This project is implemented on Ubuntu 24.04


## Project Structure 
```text
    ├── common.c
    ├── common.h
    ├── kernel.c
    ├── kernel.elf
    ├── kernel.h
    ├── kernel.ld
    ├── kernel.map
    ├── opensbi-riscv32-generic-fw_dynamic.bin
    ├── README.md
    └── run.sh
```

## Prerequisites:
1) I am using Ubuntu 24.04
2) QEMU Virtual Machine to emulate the hardware
3) OpenSBI - for the QEMU virtual machine, this acts as a BIOS


## Getting Started

1) Install packages using apt: sudo apt update && sudo apt install -y clang llvm lld qemu-system-riscv32 curl
2) Get OpenSBI: curl -LO https://github.com/qemu/qemu/raw/v8.0.4/pc-bios/opensbi-riscv32-generic-fw_dynamic.bin

## How to build and run?
1) git clone https://github.com/bhagyeshagresar/myOS-from-scratch.git
2) Go to project repository and run the shell scrip: $ ./run.sh



## RISC-V Fundamentals

### RISC-V CPU modes
Each CPU mode has different privileges:

- **M-mode** → OpenSBI (BIOS) operates in this mode
- **S-mode** → Kernel mode
- **U-mode** → User mode (application)


## Excepion Handling
The exception hanling is configured to trigger in kernel mode(supervisor mode) and also serviced in kernel mode(supervisor mode).
OpenSBI performs the delegation (medeleg register) for which mode should handle the exception, U-mode and S-Mode exceptions are handled in
S-mode handler


## Interrupt Handling
The trap handling that I am trying to implement in this project is done using the sstc extension that adds S-level stimecmp csr.


## Creating my own memory allocator
Kernel.c contains a custom memory allocation algorithm that returns a pointer to the newly allocated size in RAM for n no. of pages.
This memory allocator efficiently handles metadata tracking. Furthermore, this kernel also has the feature to free the memory.


## Creation of multiple processes

### Process Control Block


### Testing context switch


### Preemptive Scheduler
The book deals with implementing a round-robin scheduler. The 