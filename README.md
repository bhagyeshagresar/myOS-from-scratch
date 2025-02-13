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
2) OpenSBI - I don't quite understand what this means yet