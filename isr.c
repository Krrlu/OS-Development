#include "print.h"
#include "isr.h"
#include "kernel.h"
/*
    This file contain Interrupt Service Routine
*/
__attribute__((interrupt)) void interrupt_handler_0(interrupt_frame* frame){
    print_string("Error, divide by zero");
    hlt();
}

__attribute__((interrupt)) void interrupt_handler_6(interrupt_frame* frame){
    print_string("Error, invalid Opcode");
    hlt();
}

__attribute__((interrupt)) void interrupt_handler_13(interrupt_frame* frame){
    print_string("error, general protection");
    hlt();
}

/**
* A Handler for system calls
* Using value in eax to select a system call
* ebx and edx are paramaters for a system call
*
*/
__attribute__((interrupt)) void interrupt_handler_55(interrupt_frame* frame){
    uint32_t eax;
    uint32_t ebx;
    uint16_t ds;
    asm volatile ("mov %%eax, %0\n\t"
                 "mov %%ebx, %1\n\t"
                 "mov %%ds, %2\n\t"
                 "movw %3, %%ax\n\t"
                 "movw %%ax, %%ds"
        :"=r"(eax), "=r"(ebx), "=r"(ds): "i"(KERNEL_DATA_SEL):"eax","ebx"); 

     switch(eax)
    {
        case 0:
            print_string("System call test\n");
            break;

        case 1:
            print_char((char)(ebx & 0xff));
            break;

        case 2:
            break;

        default:
            print_string("Error! unknown system call number");
    }
        asm ("movw %0, %%ds" ::"r"(ds):); 
}