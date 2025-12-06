
/*declares that the boot function is the entry point of the program */
ENTRY(boot)

SECTIONS {
    . = 0x80200000; /* This is the base address. the . symbol represents the current address*/

    /* This section contains the code of the program */
    .text :{
        KEEP(*(.text.boot));
        *(.text .text.*);
    }

    /*This section contains constant data that is read-only*/
    .rodata : ALIGN(4) {
        *(.rodata .rodata.*);
    }

    /*This section contains initialized global and static data..*/
    .data : ALIGN(4) {
        *(.data .data.*);
    }

    /*This section contains uninitialized global and static data. These variables are initialized to zero on startup */
    /* Note: In the kernel_main function, the .bss section is first initialized to zero using the memset function. */
    .bss : ALIGN(4) {
        __bss = .;
        *(.bss .bss.* .sbss .sbss.*);
        __bss_end = .;
    }

    /* The kernel stack comes after the .bss section, and its size is 128KB. */
    /* The statement . += 128 * 1024 means "advance the current address by 128KB". */
    . = ALIGN(4);
    . += 128 * 1024; /* 128KB */
    __stack_top = .;

    /*define memory area after stack space*/
    . = ALIGN(4096); /*this ensures that the memory area is aligned to 4KB boundary */
    __free_ram = .;
    . += 64*1024*1024;
    __free_ram_end = .;

}