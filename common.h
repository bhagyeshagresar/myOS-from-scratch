#pragma once

/*
    reference:https://operating-system-in-1000-lines.vercel.app/en/05-hello-world 
    va_list and related macros are defined in the C standard library's 
    <stdarg.h>. In this book, we use compiler builtins directly without relying on 
    the standard library. Specifically, we'll define them in common.h as follows:
*/

typedef int bool;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef uint32_t size_t;
typedef uint32_t paddr_t; // A type representing physical memory addresses.
typedef uint32_t vaddr_t; // A type representing virtual memory addresses.

#define true 1
#define false 0
#define NULL ((void *)0)   /*note: An integer constant expression with the value 0, or such an expression cast to type void *, is called a null pointer constant.*/
#define align_up(value, align)      __builtin_align_up(value, align) //Rounds up value to the nearest multiple of align. align must be a power of 2.
#define is_aligned(value, align)    __builtin_is_aligned(value, align) //Checks if value is a multiple of align. align must be a power of 2.
#define offsetof(type, member)      __builtin_offsetof(type, member) //Returns the offset of a member within a structure (how many bytes from the start of the structure).
#define va_list     __builtin_va_list //these builtin features are part of the compiler (clang)
#define va_start    __builtin_va_start
#define va_end      __builtin_va_end
#define va_arg      __builtin_va_arg

void *memset(void *buf, char c, size_t n);
void *memcpy(void *dst, const void *src, size_t n);
char *strcpy(char *dst, const char *src);
int strcmp(const char *s1, const char *s2);
void printf(const char *fmt, ...);