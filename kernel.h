#pragma once
#include "common.h"



struct sbiret{
    long error;
    long value;
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
