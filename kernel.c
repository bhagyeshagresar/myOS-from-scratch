#include "kernel.h"
#include "common.h"

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef uint32_t size_t;

extern char __bss[], __bss_end[], __stack_top[];
extern char __free_ram[], __free_ram_end[];

/*
 * Function to allocate memory in pages (1 page = 4KB) dynamically. Also knows as bump allocator or linear allocator.
 * This function does not perform deallocation of memory
 * __free_ram is placed on a 4KB boundary due to ALIGN(4096) in the linker script. 
 * Therefore, the alloc_pages function always returns an address aligned to 4KB.
 */
paddr_t alloc_pages(uint32_t n)
{
    //__free_ram and __free_ram_end represent the start and end addresses of the free ram
    static paddr_t next_paddr = (paddr_t)__free_ram; //this is a static variable so its returned after function calls
    paddr_t paddr = next_paddr;
    next_paddr += n*PAGE_SIZE; //allocate n pages

    //if it tried to allocate memory beyond __free_ram_end do a PANIC check
    if(next_paddr > (paddr_t)__free_ram_end)
    {
        PANIC("ran out of memory\n");
    }

    //ensure the allocated memory is initially to zero
    memset((void*)paddr, 0, n*PAGE_SIZE); 
    return paddr;
 
}


struct sbiret sbi_call(long arg0, long arg1, long arg2, long arg3, long arg4,
                        long arg5, long fid, long eid){
        register long a0 __asm__("a0") = arg0; 
        register long a1 __asm__("a1") = arg1;
        register long a2 __asm__("a2") = arg2;
        register long a3 __asm__("a3") = arg3;
        register long a4 __asm__("a4") = arg4;
        register long a5 __asm__("a5") = arg5;
        register long a6 __asm__("a6") = fid;
        register long a7 __asm__("a7") = eid;
                        
      //according to SBI specification, a0 and a7 
      //a0 and a1 are with =r constraint because a0 and a1 are output operands(registers)      
        __asm__ __volatile__("ecall"
                            : "=r"(a0), "=r"(a1)
                            : "r"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5),
                            "r"(a6), "r"(a7)
                            : "memory");
        
        return (struct sbiret){.error = a0, .value = a1}; /*according to SBI specifications, the ecall returns a pair of values in a0 and a1, a0 returning an error code*/
}


void putchar(char ch){
    sbi_call(ch, 0, 0, 0, 0, 0, 0, 1/* Console Putchar */);
}

//Kernel exception handler. Store the address of this function in stvec register
__attribute__((naked))
__attribute__((aligned(4)))
void kernel_entry(void){
    __asm__ __volatile(
        "csrw sscratch, sp\n"       //temporarily save user mode stack pointer at the time of exception occurrence
        "addi sp, sp, -4*31\n"      //move the stack pointer down by 4x31 = 124 bytes(allocation)
        "sw ra, 4*0(sp)\n"          //store the value of ra at an offset of 0bytes from the address held in sp. do the same for all the
        "sw gp,  4 * 1(sp)\n"       //these are the general purpose registers of the interrupted task and are stored in the newly created
        "sw tp,  4 * 2(sp)\n"       //kernel stack
        "sw t0,  4 * 3(sp)\n"
        "sw t1,  4 * 4(sp)\n"
        "sw t2,  4 * 5(sp)\n"
        "sw t3,  4 * 6(sp)\n"
        "sw t4,  4 * 7(sp)\n"
        "sw t5,  4 * 8(sp)\n"
        "sw t6,  4 * 9(sp)\n"
        "sw a0,  4 * 10(sp)\n"
        "sw a1,  4 * 11(sp)\n"
        "sw a2,  4 * 12(sp)\n"
        "sw a3,  4 * 13(sp)\n"
        "sw a4,  4 * 14(sp)\n"
        "sw a5,  4 * 15(sp)\n"
        "sw a6,  4 * 16(sp)\n"
        "sw a7,  4 * 17(sp)\n"
        "sw s0,  4 * 18(sp)\n"
        "sw s1,  4 * 19(sp)\n"
        "sw s2,  4 * 20(sp)\n"
        "sw s3,  4 * 21(sp)\n"
        "sw s4,  4 * 22(sp)\n"
        "sw s5,  4 * 23(sp)\n"
        "sw s6,  4 * 24(sp)\n"
        "sw s7,  4 * 25(sp)\n"
        "sw s8,  4 * 26(sp)\n"
        "sw s9,  4 * 27(sp)\n"
        "sw s10, 4 * 28(sp)\n"
        "sw s11, 4 * 29(sp)\n"

        "csrr a0, sscratch\n"      //the user mode's stack poiinter at the point of exception is stored in a0
        "sw a0, 4*30(sp)\n"        //store the user mode's stack pointer to the stack frame so now the whole context is saved, a0 may now hold a garbage value
        
        "mv a0, sp\n"              //move the curren address of the stack frame into a0, In the RISC-V calling convention, the first function argument must be placed in the a0 register
        "call handle_trap\n"
        
        "lw ra,  4 * 0(sp)\n"       //after the handle_trap returns the kernel resumes the interrupted process, loads the trap frame back to the GPRs
        "lw gp,  4 * 1(sp)\n"
        "lw tp,  4 * 2(sp)\n"
        "lw t0,  4 * 3(sp)\n"
        "lw t1,  4 * 4(sp)\n"
        "lw t2,  4 * 5(sp)\n"
        "lw t3,  4 * 6(sp)\n"
        "lw t4,  4 * 7(sp)\n"
        "lw t5,  4 * 8(sp)\n"
        "lw t6,  4 * 9(sp)\n"
        "lw a0,  4 * 10(sp)\n"
        "lw a1,  4 * 11(sp)\n"
        "lw a2,  4 * 12(sp)\n"
        "lw a3,  4 * 13(sp)\n"
        "lw a4,  4 * 14(sp)\n"
        "lw a5,  4 * 15(sp)\n"
        "lw a6,  4 * 16(sp)\n"
        "lw a7,  4 * 17(sp)\n"
        "lw s0,  4 * 18(sp)\n"
        "lw s1,  4 * 19(sp)\n"
        "lw s2,  4 * 20(sp)\n"
        "lw s3,  4 * 21(sp)\n"
        "lw s4,  4 * 22(sp)\n"
        "lw s5,  4 * 23(sp)\n"
        "lw s6,  4 * 24(sp)\n"
        "lw s7,  4 * 25(sp)\n"
        "lw s8,  4 * 26(sp)\n"
        "lw s9,  4 * 27(sp)\n"
        "lw s10, 4 * 28(sp)\n"
        "lw s11, 4 * 29(sp)\n"
        "lw sp,  4 * 30(sp)\n"  
        "sret\n"                //call the sret instruction to resume execution from the point where the exception occurred.
        
    );
}

//Handle the exception in C
void handle_trap(struct trap_frame *f){
    uint32_t scause = READ_CSR(scause);  //scause - type of exception. The kernel reads this to identify the type of exception
    uint32_t stval = READ_CSR(stval);    //stval - Additional information about the exception (e.g., memory address that caused the exception). Depends on the type of exception.
    uint32_t user_pc = READ_CSR(sepc);   //sepc - Program counter at the point where the exception occurred.

    PANIC("unexpected trap scause=%x, stval=%x, sepc=%x\n", scause, stval, user_pc);
}

void kernel_main(void){
    // const char *s = "Hello World\n";
    // for(int i = 0; s[i] != '\0'; i++){
    //     putchar(s[i]);
    // }
    //memset(__bss, 0, (size_t)__bss_end - (size_t)__bss);

    //printf("\n\nHello %s\n", "World!");
    //printf("1 + 2 = %d, %x\n", 1+2, 0x1234abcd);

    //PANIC("booted!\n"); //from chapter 07: Kernel Panic 
    //printf("unreachable here!\n");

    // memset(__bss, 0, (size_t)__bss_end - (size_t)__bss); //because dat in bss section is initialised to zero
    // WRITE_CSR(stvec, (uint32_t)kernel_entry); //tell the CPU where the exception handler is located
    // /*
    //     This reads and writes the cycle register into x0. Since cycle is a read-only register, 
    //     CPU determines that the instruction is invalid and triggers an illegal instruction exception.
    // */
    // __asm__ __volatile__("unimp");            //unimp is a pseudo instruction. the assembler translates this to : csrrw x0,cycle,x0


    // //create an infinite idle loop
    // for(;;){
    //     __asm__ __volatile__("wfi"); // This is the Wait For Interrupt instruction, embedded via inline assembly. It is a power-saving mechanism
    // }

    //Chapter 9 : Memory Allocation Testing
    memset(__bss, 0, (size_t)__bss_end - (size_t)__bss); 

    paddr_t paddr0 = alloc_pages(2);
    paddr_t paddr1 = alloc_pages(1);
    printf("alloc_pages test: paddr0=%x\n", paddr0);
    printf("alloc_pages test: paddr1=%x\n", paddr1);

    PANIC("booted!");

}


__attribute__((section(".text.boot")))
__attribute__((naked))
void boot(void){
    __asm__ __volatile__(
        "mv sp, %[stack_top]\n" //set the stack pointer
        "j kernel_main\n"       //jump to the kernel main function
        :
        : [stack_top] "r" (__stack_top) // Pass the stack top address as %[stack_top]
    );
}

