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



## Creating a simple memory allocator(bump allocator)


## Creation of multiple processes

### Process Control Block

```c
// Define a process object, also known as a Process Control Block (PCB)
struct process {
    int pid;                // Process ID
    int state;              // Process state: PROC_UNUSED or PROC_RUNNABLE
    vaddr_t sp;             // Stack Pointer
    uint8_t stack[8192];    // Kernel Stack
};
```
callee-saved registers - must be restored by the called function before returning.
In RISC-V, s0 - s11 are callee-saved registers. a0 and a1 are caller-saved registers
and are already saved on the stack by the caller


### Testing context switch
While testing context switching between `proc_a` and `proc_b`, I initially made the mistake of saving and restoring stacks incorrectly:

**Issue:** `switch_context(&proc_a->sp, &proc_b->sp)` was called, but `proc_b`’s stack was never properly initialized/saved.

**What happened:**
* `old_sp = &proc_a->sp` overwrote `proc_a`’s stack with the current SP (garbage)
* `new_sp = &proc_b->sp` loaded an uninitialized stack pointer
* `ret` popped a garbage return address $\to$ CPU jumped to `0x0`
* Kernel panic occurred (`scause=1`, `stval=0`, `sepc=0`)

It was fun to understand what happened in the background!
```c

void proc_a_entry(void)
{
    printf("Starting process A\n");
    while(1)
    {
        putchar('A');
        switch_context(&proc_a->sp, &proc_b->sp);
        delay();
    }
}

void proc_b_entry(void)
{
    printf("Starting process B\n");
    while(1)
    {
        putchar('B');
        switch_context(&proc_b->sp, &proc_a->sp);
        delay();
    }
}

void kernel_main(void)
{
    memset(__bss, 0, (size_t) __bss_end - (size_t) __bss);
    WRITE_CSR(stvec, (uint32_t) kernel_entry);

    proc_a = create_process((uint32_t) proc_a_entry);
    proc_b = create_process((uint32_t) proc_b_entry);
    proc_a_entry(); //start the first process and trigger context switch

    PANIC("unreachable here!");

}

Output: 
Starting process A
AStarting process B
BABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABAB

```