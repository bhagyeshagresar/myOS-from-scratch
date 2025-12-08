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





## Tools used:

1) QEMU Virtual Machine to emulate the hardware
2) OpenSBI - for the QEMU virtual machine, this acts as a BIOS


## Constrains (=r) and (r)

= modifier: This specifies that the operand is write-only. The compiler is informed that the inline assembly code will overwrite the contents of the registers without needing to read their previous values.
r constraint: This tells the compiler that the operand should be placed in a general-purpose register. The compiler is free to choose any available general-purpose register to fulfill this requirement.


## RISC-V Fundamentals

### RISC-V CPU modes
Each CPU mode has different privileges
M-mode -> OpenSBI (BIOS) operates in this mode
S-mode -> kernel mode
U-mode -> user mode (applicaton)

## Excepion Handling
PANIC: kernel.c:291: unexpected trap scause=00000002, stval=00000000, sepc=80200370
value of scause = 2 means illegal instruction
```c

const char *s = "Hello World\n";
    for(int i = 0; s[i] != '\0'; i++){
        putchar(s[i]);
    }
    
     memset(__bss, 0, (size_t)__bss_end - (size_t)__bss); //because dat in bss section is initialised to zero
     WRITE_CSR(stvec, (uint32_t)kernel_entry); //tell the CPU where the exception handler is located
    /*
        This reads and writes the cycle register into x0. Since cycle is a read-only register, 
        CPU determines that the instruction is invalid and triggers an illegal instruction exception.
    */
    __asm__ __volatile__("unimp");            //unimp is a pseudo instruction. the assembler translates this to : csrrw x0,cycle,x0

llvm-addr2line -e kernel.elf 80200370
/home/bhagyesh/myOS_project/kernel.c:313    //confirmed that this line matches with unimp instruction in kernel.c
```

## Creating a simple memory allocator(bump allocator)

### Testing our simple memory allocator
The kernel initializes the BSS section and then allocates the first blocks of physical memory using a simple bump allocator before panicking.

```c
void kernel_main(void) {
    // Zero out the entire BSS section
    memset(__bss, 0, (size_t) __bss_end - (size_t) __bss);

    // Allocate 2 pages (8KB) and 1 page (4KB)
    paddr_t paddr0 = alloc_pages(2);
    paddr_t paddr1 = alloc_pages(1);
    
    // Print the addresses of the allocated physical pages
    printf("alloc_pages test: paddr0=%x\n", paddr0);
    printf("alloc_pages test: paddr1=%x\n", paddr1);

    PANIC("booted!");
}

$ ./run.sh
alloc_pages test: paddr0=80221000
alloc_pages test: paddr1=80223000
PANIC: kernel.c:188: booted!


$ llvm-nm kernel.elf | grep __free_ram 
80221000 B __free_ram
84221000 B __free_ram_end

The symbol B means assigned to .bss section

```

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
While testing context switching between proc_a and proc_b, I initially made the mistake of saving and restoring stacks incorrectly:

Issue: switch_context(&proc_a->sp, &proc_b->sp) was called, but proc_b’s stack was never properly initialized/saved.

What happened:

old_sp = &proc_a->sp overwrote proc_a’s stack with the current SP (garbage).

new_sp = &proc_b->sp loaded an uninitialized stack pointer.

ret popped a garbage return address → CPU jumped to 0x0.

Kernel panic occurred (scause=1, stval=0, sepc=0).

Lesson learned: Always ensure the new process stack is properly initialized before switching, and save the old process stack correctly.

```c

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