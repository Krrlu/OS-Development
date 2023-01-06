#include "kernel.h"
__asm__ ("jmpl  $0x8,$main\n");

void main(){
    asm volatile("mov %%ax, %%ds"::"a"(KERNEL_DATA_SEL):); //load ds register
    asm volatile("mov %0, %%esp"::"i"(0x7000):); //load ds register
    
    char *p = (char*)0xb8000;
    *p = 'H';
    *(p+1) = 7;
    *(p+2) = 'e';
    *(p+3) = 7;
    *(p+4) = 'l';
    *(p+5) = 7;
    *(p+6) = 'l';
    *(p+7) = 7;
    *(p+8) = 'o';
    *(p+9) = 7;
    __asm__ __volatile__ ("hlt");
}