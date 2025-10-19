#include "common.h"

void putchar(char ch); // for our minimal environment project, just including the function declaration should suffice. Ideally there is a header file included

/*
 * printf - A minimal custom implementation of the C printf function.
 *
 * This function handles variable arguments based on the format string 'fmt'.
 * It supports a limited set of format specifiers: '%%' (percent sign),
 * '%s' (null-terminated string), '%d' (signed decimal integer), and
 * '%x' (unsigned hexadecimal integer).
 *
 * Parameters:
 * fmt: The null-terminated format string containing plain characters and format specifiers.
 * ...: Variable list of arguments corresponding to the format specifiers.
 *
 * Dependencies:
 * - va_list, va_start, va_end: Standard macros for handling variable arguments.
 * - putchar: A function (defined in kernel.c) that outputs a single character.
 *
 * Returns:
 * none
 */
void printf(const char*fmt, ...)
{
    va_list vargs;
    va_start(vargs, fmt); //va_start takes the last fixed argument as the next pointer

    while(*fmt)
    {
        if(*fmt == '%')
        {
            fmt++;
            switch(*fmt)
            { //read the next character
                case '\0':
                {
                    // if % is at the end of the string
                    putchar('%');
                    goto end;
                } 
                case '%': 
                {//Print '%'
                    putchar('%');
                    break;
                }
                case 's':
                { //print the null-terminated string
                    const char* s = va_arg(vargs, const char*);
                    while(*s){
                        putchar(*s);
                        s++;
                    }
                    break;
                }
                case 'd': 
                {//print an integer in decimal
                    int value = va_arg(vargs, int);
                    unsigned magnitude = value;
                    if(value < 0){
                        putchar('-');
                        magnitude = -magnitude;
                    }

                    //determine the length of the number or number of digits
                    unsigned divisor = 1;
                    while(magnitude/divisor > 9){
                        divisor *= 10;
                    }

                    while(divisor > 0){
                        putchar('0' + magnitude/divisor); /*Note: This putchar expects a char, C converts '0' to an int and then the resulting integer is truncated to a char by the compiler*/
                        magnitude %= divisor; //Ex. magnitude = 456, the result of this step is 456%100 = 56
                        divisor /= 10;
                    }
                    break;
                }
                case 'x': 
                {//print an integer in hexadecimal
                    unsigned value = va_arg(vargs, unsigned);
                    for(int i = 7; i >= 0; i--){
                        unsigned nibble = (value >> (i*4)) & 0xf; //extract the most significant nibble
                        putchar("0123456789abcdef"[nibble]);      //lookup string: constant string literal. Ex If nibble is 10, it selects the 10th index, which is the character 'a'.
                    }
                    
                    break;
                } 
            }
        }  
        
            
        else
        {
            putchar(*fmt);
        }
        fmt++;
    }

    end:
        va_end(vargs);
}



/*
 * memcpy - Copy a block of memory.
 *
 * This function copies 'n' bytes of data from the memory area pointed to by 'src'
 * to the memory area pointed to by 'dst'.
 *
 * It is essential that the source and destination memory regions DO NOT OVERLAP.
 * If the regions overlap, the behavior is undefined, and 'memmove' should be used instead.
 *
 * Parameters:
 * dst: Pointer to the destination memory area where the content is to be copied.
 * src: Pointer to the source memory area to be copied.
 * n: The number of bytes to copy.
 *
 * Returns:
 * A pointer to the destination area 'dst'.
 */
void *memcpy(void *dst, const void* src, size_t n){
    uint8_t *d = (uint8_t *)dst;
    const uint8_t *s = (const uint8_t *) src;

    while(n--){
        *d++ = *s++;
    }
    return dst;

}



/*
 * memset - Fill a block of memory with a specified byte value.
 *
 * This function sets the first 'n' bytes of the memory area pointed to by 'buf'
 * to the value specified by 'c' (converted to an unsigned char).
 *
 * Parameters:
 * buf: Pointer to the memory area to be filled.
 * c: The character value (as a single byte) to set.
 * n: The number of bytes to fill.
 *
 * Returns:
 * A pointer to the memory area 'buf'.
 */
void *memset(void *buf, char c, size_t n){
    uint8_t *p = (uint8_t*)buf;
    while(n--){
        *p++ = c;
    }
    return buf;
}

/*
 * strcpy - Copy a null-terminated string.
 *
 * This function copies the null-terminated string pointed to by 'src' (including 
 * the null terminator itself) to the destination buffer pointed to by 'dst'.
 *
 * WARNING: The destination buffer 'dst' must be large enough to hold the entire 
 * source string and its null terminator. If not, this function will cause a 
 * buffer overflow, resulting in undefined behavior.
 *
 * Parameters:
 * dst: Pointer to the destination buffer.
 * src: Pointer to the null-terminated source string.
 *
 * Returns:
 * A pointer to the destination string 'dst'.
 */
char *strcpy(char *dst, const char* src){
    char* d = dst;
    while(*src){
        *d++ = *src++;
    }
    *d = '\0';
    return dst;
}

/*
 * strcmp - Lexicographically compare two null-terminated strings.
 *
 * This function compares the two strings 's1' and 's2' character by character.
 *
 * Parameters:
 * s1: The first null-terminated string.
 * s2: The second null-terminated string.
 *
 * Returns:
 * An integer less than, equal to, or greater than zero if 's1' is found, 
 * respectively, to be less than, to match, or to be greater than 's2'. 
 * The return value is the difference in ASCII/code values of the first differing characters.
 * for example for s1 = abc and s2 = abcd, at the end of the while loop, s1 = \0 and s2 = d, the difference is 0-100 = -100 (ASCII values of \0 and d respectively)
 */
int strcmp(const char *s1, const char *s2){
    while(*s1 && *s2){
        if(*s1 != *s2){
            break;
        }
        s1++;
        s2++;
    }

    return *(unsigned char*)s1 - *(unsigned char*)s2;
}