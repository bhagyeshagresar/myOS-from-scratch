#include "common.h"

void putchar(char ch); // for our minimal environment project, just including the function declaration should suffice. Ideally there is a header file included
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