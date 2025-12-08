#pragma once
#include "common.h"

#define PROCS_MAX           8         // Max number of processes
#define PROC_UNUSED         0         // Unused process control strucuture
#define PROC_RUNNABLE       1         // runnable process

struct sbiret{
    long error;
    long value;
};

//This structs represents the program state saved in kernel_entry function
struct trap_frame {
    uint32_t ra;
    uint32_t gp;
    uint32_t tp;
    uint32_t t0;
    uint32_t t1;
    uint32_t t2;
    uint32_t t3;
    uint32_t t4;
    uint32_t t5;
    uint32_t t6;
    uint32_t a0;
    uint32_t a1;
    uint32_t a2;
    uint32_t a3;
    uint32_t a4;
    uint32_t a5;
    uint32_t a6;
    uint32_t a7;
    uint32_t s0;
    uint32_t s1;
    uint32_t s2;
    uint32_t s3;
    uint32_t s4;
    uint32_t s5;
    uint32_t s6;
    uint32_t s7;
    uint32_t s8;
    uint32_t s9;
    uint32_t s10;
    uint32_t s11;
    uint32_t sp;
} __attribute__((packed));


//define a process object, also known as a Process Control Block(PCB)
struct process
{
    int pid;                // Process ID
    int state;              // Process state: PROC_UNUSED or PROC_RUNNABLE
    vaddr_t sp;             // Stack Pointer
    uint8_t stack[8192];    // Kernel Stack (8KB size)
};


/*
    __FILE__ and __LINE__ are standard C predefined macros and are handled by the C preprocessor phase of compilation
    __VA_ARGS__ is a special identifier that is expanded by the C preprocessor to become all the arguments that are passed to the macro after the last named argument.
*/
#define PANIC(fmt, ...)         \
    do{                            \
        printf("PANIC: %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        while(1){} \
    }while(0)



//These can't be regular C functions because the inline assembly instruction expects a literal string and it would be a runtime address which is illegal for the assembler.
//Macro to read from a specified CSR register
//#reg is a strigification operator from the C preprocessor. It converts the macro argument into a string literal(ex. "stvec")
//({...}) - this is called statement expression, the value of the expression is the value of the last statement in the block.
// "=r(__tmp)" -> this is the output operand
#define READ_CSR(reg)                                                   \
({                                                                      \
    unsigned long __tmp;                                                \
    __asm__ __volatile__("csrr %0, " #reg : "=r"(__tmp));               \
    __tmp;                                                              \
})                                                                      \

//Macro to write to a specified CSR register
//:: Separates the empty output section from the input operands.
#define WRITE_CSR(reg, value)                                           \
    do{                                                                 \
        uint32_t __tmp = (value);                                       \
        __asm__ __volatile__("csrw " #reg ", %0" ::"r"(__tmp));         \
    }while(0)                                                           




