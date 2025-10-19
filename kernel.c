#include "kernel.h"
#include "common.h"

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef uint32_t size_t;

extern char __bss[], __bss_end[], __stack_top[];


void *memset(void *buf, char c, size_t n){
    uint8_t *p = (uint8_t*)buf;
    while(n--){
        *p++ = c;
    }
    return buf;
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



void kernel_main(void){
    // const char *s = "Hello World\n";
    // for(int i = 0; s[i] != '\0'; i++){
    //     putchar(s[i]);
    // }
    //memset(__bss, 0, (size_t)__bss_end - (size_t)__bss);

    printf("\n\nHello %s\n", "World!");
    printf("1 + 2 = %d, %x\n", 1+2, 0x1234abcd);


    //create an infinite idle loop
    for(;;){
        __asm__ __volatile__("wfi"); // This is the Wait For Interrupt instruction, embedded via inline assembly. It is a power-saving mechanism
    }
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