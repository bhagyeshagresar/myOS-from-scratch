# myOS from scratch

Writing my own OS for a 32bit RISC-V architecture. The implementation is based off of this project: 
[text](https://operating-system-in-1000-lines.vercel.app/en/)

In addition to building my own OS, the aim of the project is to implement the following features which are not covered in the above:
1) Interrupt handling instead of polling for new data on devices
2) Preemptive Multitasking
3) Inter-process communication
4) Memory Allocation to free memory

This project is implemented on Ubuntu 24.04

## Tools used:

1) QEMU Virtual Machine to emulate the hardware
2) OpenSBI - for the QEMU virtual machine, this acts as a BIOS


## Constrains (=r) and (r)

= modifier: This specifies that the operand is write-only. The compiler is informed that the inline assembly code will overwrite the contents of the registers without needing to read their previous values.
r constraint: This tells the compiler that the operand should be placed in a general-purpose register. The compiler is free to choose any available general-purpose register to fulfill this requirement.


