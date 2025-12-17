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


## Constrains (=r) and (r)

= modifier: This specifies that the operand is write-only. The compiler is informed that the inline assembly code will overwrite the contents of the registers without needing to read their previous values.
r constraint: This tells the compiler that the operand should be placed in a general-purpose register. The compiler is free to choose any available general-purpose register to fulfill this requirement.


## RISC-V Fundamentals

### RISC-V CPU modes
Each CPU mode has different privileges:

- **M-mode** → OpenSBI (BIOS) operates in this mode
- **S-mode** → Kernel mode
- **U-mode** → User mode (application)


## Excepion Handling



## Creating a simple memory allocator


## Creation of multiple processes

### Process Control Block


### Testing context switch
