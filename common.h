#pragma once

/*
    reference:https://operating-system-in-1000-lines.vercel.app/en/05-hello-world 
    va_list and related macros are defined in the C standard library's 
    <stdarg.h>. In this book, we use compiler builtins directly without relying on 
    the standard library. Specifically, we'll define them in common.h as follows:
*/

#define va_list     __builtin_va_list //these builtin features are part of the compiler (clang)
#define va_start    __builtin_va_start
#define va_end      __builtin_va_end
#define va_arg      __builtin_va_arg

void printf(const char *fmt, ...);